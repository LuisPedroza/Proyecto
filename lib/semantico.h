#ifndef LIB_SEMANTICO_H
#define LIB_SEMANTICO_H

#include <map>
#include <string>
#include <string_view>
#include <vector>
#include <type_traits>
#include <iostream>
#include <utility>

#include "lexer.h"
#include "parser.h"

namespace lib {

    template<typename T>
    void asercion(bool v, const T& elevar){
        if(!v){
            throw elevar;
        }
    }

    struct datos_funcion {
        std::vector<token> parametros;
        token retorno;
    };

    struct datos_expresion {
        token tipo;                  // NUMBER, ARRAY o FUNCTION
        std::string nombre;          // en el semántico: si el token fue FUNCTION, el nombre de la función; vacío en otro caso
    };                               // en el codegen: el identificador del temporal

    using funciones = std::map<std::string_view, datos_funcion>;
    using ambitos = std::vector<std::map<std::string_view, token>>;
    using ambito = std::map<std::string_view, token>;

    void declara(ambito& a, const token_anotada& id, const token& tipo){
        auto insertado = a.emplace(id, tipo).second;
        asercion(insertado, std::make_pair(id, "No se puede volver a declarar dentro del mismo ambito."));
    }

    datos_expresion analiza_expresion(const expresion& nodo, const funciones& f, ambitos& a, const token& retorno);
    datos_expresion analiza_expresion_terminal(const expresion_terminal& nodo, const funciones& f, ambitos& a, const token& retorno) {
        if (nodo.t->tipo == IDENTIFICADOR) {
            bool declarada_v = false, declarada_f = (f.count(*nodo.t) != 0);
            std::map<std::string_view, token>::iterator it;
            for (auto& m : a) {
                it = m.find(*nodo.t);
                if (it != m.end()) {
                    declarada_v = true;
                    break;
                }
            }
            asercion(declarada_v || declarada_f, std::make_pair(*nodo.get_token(), "Variable no declarada."));
            std::string_view nombre = *nodo.t;
            return (declarada_v ? datos_expresion{it->second} : datos_expresion{FUNCION, {nombre.begin(), nombre.end()}});
        } else {
            return datos_expresion{NUMERO};
        }
    }

    datos_expresion analiza_expresion_op_prefijo(const expresion_op_prefijo& nodo, const funciones& f, ambitos& a, const token& retorno) {
        datos_expresion t = analiza_expresion(*nodo.sobre, f, a, retorno);
        token tipo = nodo.operador->tipo;
        if (tipo == TAMANYO_ARREGLO) {
            asercion(t.tipo == ARREGLO, std::make_pair(*nodo.get_token(), "El operador se debe usar sobre un arreglo."));
            return datos_expresion{NUMERO};
        } else {
            asercion(t.tipo == NUMERO, std::make_pair(*nodo.get_token(), "El operador se debe usar sobre un tipo numerico."));
            return datos_expresion{NUMERO};
        }
    }

    datos_expresion analiza_expresion_op_binario(const expresion_op_binario& nodo, const funciones& f, ambitos& a, const token& retorno){
        datos_expresion izq = analiza_expresion(*nodo.izq, f, a, retorno);
        datos_expresion der = analiza_expresion(*nodo.der, f, a, retorno);
        if (izq.tipo == ARREGLO && nodo.operador->tipo == MULTIPLICACION) {
            asercion(der.tipo == ARREGLO, std::make_pair(*nodo.der->get_token(), "Debe ser un arreglo."));
            return datos_expresion{ARREGLO};
        }else{
            asercion(izq.tipo == NUMERO, std::make_pair(*nodo.izq->get_token(), "Debe ser de tipo numerico."));
            asercion(der.tipo == NUMERO, std::make_pair(*nodo.der->get_token(), "Debe ser de tipo numerico."));
            return datos_expresion{NUMERO};
        }
    }

    datos_expresion analiza_expresion_parentesis_posfijo(const expresion_parentesis_posfijo& nodo, const funciones& f, ambitos& a, const token& retorno) {
        datos_expresion t = analiza_expresion(*nodo.func, f, a, retorno);
        auto ini_a = nodo.parametros.begin();
        auto fin_a = nodo.parametros.end();
        int tam =  fin_a - ini_a;
        if (t.tipo == FUNCION) {
            const auto& llamar = f.find(t.nombre)->second;
            asercion(llamar.parametros.size() == nodo.parametros.size(), std::make_pair(*nodo.get_token(), "Numero incorrecto de argumentos."));
            auto ini_p = llamar.parametros.begin();
            for(const auto& actual : nodo.parametros){
                datos_expresion t = analiza_expresion(*actual, f, a, retorno);
                asercion(*ini_p++ == t.tipo, std::make_pair(actual->get_token(), "Tipo de argumento incorrecto."));
            }
            return datos_expresion{llamar.retorno};
        } else {
            asercion(t.tipo == ARREGLO, std::make_pair(*nodo.get_token(), "Debe ser de tipo array."));
            asercion(tam == 1, std::make_pair(*nodo.get_token(), "Error al indicar el tamaño del arreglo."));
            datos_expresion e = analiza_expresion(**ini_a, f, a, retorno);
            asercion(e.tipo == NUMERO, std::make_pair(*nodo.parametros[0]->get_token(), "Tamaño de arreglo no valido."));
            return datos_expresion{NUMERO};
        }
    }

    datos_expresion analiza_expresion_corchetes_posfijo(const expresion_corchetes_posfijo& nodo, const funciones& f, ambitos& a, const token& retorno){
        datos_expresion ex = analiza_expresion(*nodo.ex, f, a, retorno);
        asercion(ex.tipo == ARREGLO, std::make_pair(*nodo.ex->get_token(), "La variable debe ser de tipo array."));
        datos_expresion izq = analiza_expresion(*nodo.izq, f, a, retorno);
        asercion(izq.tipo == NUMERO, std::make_pair(*nodo.izq->get_token(), "Se esperaba un numero."));
        if(nodo.der != nullptr){
            datos_expresion der = analiza_expresion(*nodo.der, f, a, retorno);
            asercion(der.tipo == NUMERO, std::make_pair(*nodo.der->get_token(), "Se esperaba un numero"));
        }
        return datos_expresion{ARREGLO};
    }

    datos_expresion analiza_expresion_arreglo(const expresion_arreglo& nodo, const funciones& f, ambitos& a, const token& retorno) {
        for (const auto& actual : nodo.elementos) {
            datos_expresion t = analiza_expresion(*actual, f, a, retorno);
            asercion(t.tipo == NUMERO, std::make_pair(actual->get_token(), "No es un numero."));
        }
        return datos_expresion{ARREGLO};
    }

    datos_expresion analiza_expresion(const expresion& nodo, const funciones& f, ambitos& a, const token& retorno) {
        if (auto p = dynamic_cast<const expresion_terminal*>(&nodo); p != nullptr) {
            return analiza_expresion_terminal(*p, f, a, retorno);
        } else if (auto p = dynamic_cast<const expresion_op_prefijo*>(&nodo); p != nullptr) {
            return analiza_expresion_op_prefijo(*p, f, a, retorno);
        } else if (auto p = dynamic_cast<const expresion_op_binario*>(&nodo); p != nullptr) {
            return analiza_expresion_op_binario(*p, f, a, retorno);
        } else if (auto p = dynamic_cast<const expresion_parentesis_posfijo*>(&nodo); p != nullptr) {
            return analiza_expresion_parentesis_posfijo(*p, f, a, retorno);
        } else if (auto p = dynamic_cast<const expresion_corchetes_posfijo*>(&nodo); p != nullptr) {
            return analiza_expresion_corchetes_posfijo(*p, f, a, retorno);
        } else if (auto p = dynamic_cast<const expresion_arreglo*>(&nodo); p != nullptr) {
            return analiza_expresion_arreglo(*p, f, a, retorno);
        }
    }
    void analiza_sentencia(const sentencia& nodo, const funciones& f, ambitos& a, const token& retorno);
    void analiza_declaracion(const sentencia_declaracion& nodo, const funciones& f, ambitos& a, const token& retorno) {
        datos_expresion decl = analiza_expresion(*nodo.inicializador, f, a, retorno);
        asercion(decl.tipo == nodo.tipo->tipo, std::make_pair(*nodo.inicializador->get_token(), "Debe coincidir con el tipo de la declaracion."));
        declara(a.back(), *nodo.nombre, nodo.tipo->tipo);
    }

    void analiza_if(const sentencia_if& nodo, const funciones& f, ambitos& a, const token& retorno) {
        datos_expresion e_cond = analiza_expresion(*nodo.condicion, f, a, retorno);
        asercion(e_cond.tipo == NUMERO, std::make_pair(*nodo.condicion->get_token(), "Condicion no valida."));
        datos_expresion e_si, e_no;
        ambito m_si;
        a.push_back(m_si);
        for(auto& s : nodo.parte_si){
            analiza_sentencia(*s, f, a, retorno);
        }
        a.pop_back();
        if(!nodo.parte_no.empty()){
            ambito m_no;
            a.push_back(m_no);
            for(auto& s : nodo.parte_no){
                analiza_sentencia(*s, f, a, retorno);
            }
            a.pop_back();
        }
    }

    void analiza_return(const sentencia_return& nodo, const funciones& f, ambitos& a, const token& retorno) {
        datos_expresion t = analiza_expresion(*nodo.ex, f, a, retorno);
        asercion(t.tipo == retorno, std::make_pair(*nodo.ex->get_token(), "Tipo de retorno incorrecto."));
    }

    void analiza_sentencia(const sentencia& nodo, const funciones& f, ambitos& a, const token& retorno) {
        if (auto p = dynamic_cast<const sentencia_declaracion*>(&nodo); p != nullptr) {
            return analiza_declaracion(dynamic_cast<const sentencia_declaracion&>(nodo), f, a, retorno);
        } else if (auto p = dynamic_cast<const sentencia_if*>(&nodo); p != nullptr) {
            return analiza_if(dynamic_cast<const sentencia_if&>(nodo), f, a, retorno);
        } else if (auto p = dynamic_cast<const sentencia_return*>(&nodo); p != nullptr) {
            return analiza_return(dynamic_cast<const sentencia_return&>(nodo), f, a, retorno);
        }
    }

    template <typename FI>
    void analiza_funcion(FI iter, funciones& f) {
        while(iter->nombre != nullptr){
            std::vector<token> params;
            ambito ap;
            for (auto p : iter->parametros) {
                params.push_back(p.tipo->tipo);
                declara(ap, *p.nombre, p.tipo->tipo);
            }
            ambitos a;
            a.push_back(ap);
            auto insertado = f.emplace(*iter->nombre, datos_funcion{std::move(params), iter->retorno->tipo}).second;
            asercion(insertado, std::make_pair(*iter->nombre, "No se puede volver a declarar la funcion"));
            for (const auto& s : iter->sentencias) {
                analiza_sentencia(*s, f, a, iter->retorno->tipo);
            }
            iter++;
        }
    }

}

#endif
