#ifndef LIB_SEMANTICO_H
#define LIB_SEMANTICO_H

#include <map>
#include <string>
#include <string_view>
#include <vector>
#include <type_traits> 
#include <iostream>

#include "lexer.h"
#include "parser.h"

namespace lib {

    void espera(bool v, const char* mensaje, const token_anotada* pos) {
        if (!v) {
            throw std::make_pair(*pos, mensaje);
        }
    }

    struct datos_funcion {
        std::vector<token> parametros;
        token retorno;
    };

    struct tipo_expresion {
        token tipo;          // NUMBER, ARRAY o FUNCTION
        std::string_view f;  // si el token fue FUNCTION, el nombre de la función
    };

    using funciones = std::map<std::string_view, datos_funcion>;
    using ambitos = std::vector<std::map<std::string_view, token>>;

    tipo_expresion analiza_expresion(const expresion& nodo, funciones& f, ambitos& a, const token& retorno);
    tipo_expresion analiza_expresion_terminal(const expresion_terminal& nodo, funciones& f, ambitos& a, const token& retorno) {
        if (nodo.t->tipo == IDENTIFICADOR) {
            bool declarada_v = false;
            std::map<std::string_view, token>::iterator it;
            for (auto& m : a) {
                it = m.find(*nodo.t);
                if (it != m.end()) {
                    declarada_v = true;
                    break;
                }
            }
            bool declarada_f = f.count(*nodo.t) != 0;
            espera(declarada_v || declarada_f, "Variable no declarada.", nodo.get_token());                        
            if(declarada_f){
                return tipo_expresion{FUNCION, *nodo.t};
            }
            if(declarada_v){
                return tipo_expresion{it->second};
            }
        } else {
            return tipo_expresion{NUMERO};
        }
    }

    tipo_expresion analiza_expresion_op_prefijo(const expresion_op_prefijo& nodo, funciones& f, ambitos& a, const token& retorno) {
        tipo_expresion t = analiza_expresion(*nodo.sobre, f, a, retorno);
        token tipo = nodo.operador->tipo;
        if (tipo == TAMANYO_ARREGLO) {
            espera(t.tipo == ARREGLO, "El operador se debe usar sobre un arreglo.", nodo.get_token());
            return tipo_expresion{NUMERO};
        } else if (tipo == RESTA) {
            espera(t.tipo == NUMERO, "El operador se debe usar sobre un tipo numerico.", nodo.get_token());
            return tipo_expresion{NUMERO};
        } else {
            espera(t.tipo == NUMERO, "El operador se debe usar sobre un tipo numerico.", nodo.get_token());
            return tipo_expresion{NUMERO};
        }
    }

    tipo_expresion analiza_expresion_op_binario(const expresion_op_binario& nodo, funciones& f, ambitos& a, const token& retorno){
        tipo_expresion izq = analiza_expresion(*nodo.izq, f, a, retorno);
        tipo_expresion der = analiza_expresion(*nodo.der, f, a, retorno);
        if(es_operador_rel(nodo.operador->tipo) || es_operador_log(nodo.operador->tipo) || (es_operador_arit(nodo.operador->tipo) && nodo.operador->tipo != MULTIPLICACION)){
            espera(izq.tipo == NUMERO, "Debe ser de tipo numerico.", nodo.get_token(0));
            espera(der.tipo == NUMERO, "Debe ser de tipo numerico.", nodo.get_token(1));
            return tipo_expresion{NUMERO};
        }else{
            if(izq.tipo == ARREGLO){
                espera(der.tipo == ARREGLO, "Debe ser un arreglo.", nodo.get_token(1));
                return tipo_expresion{ARREGLO};
            }else if(izq.tipo == NUMERO){
                espera(der.tipo == NUMERO, "Debe ser de tipo numerico.", nodo.get_token(1));
                return tipo_expresion{NUMERO};
            }else{
                espera(false, "Tipos incorrectos.", nodo.get_token());
            }
        }
    }

    tipo_expresion analiza_expresion_parentesis_posfijo(const expresion_parentesis_posfijo& nodo, funciones& f, ambitos& a, const token& retorno) {
        tipo_expresion t = analiza_expresion(*nodo.func, f, a, retorno);
        auto ini_a = nodo.parametros.begin();
        auto fin_a = nodo.parametros.end();
        int tam =  fin_a - ini_a;
        if (t.tipo == FUNCION) {
            auto ini_p = f[t.f].parametros.begin();
            auto fin_p = f[t.f].parametros.end();
            espera(((fin_p - ini_p) == (fin_a - ini_a)), "Numero incorrecto de argumentos.", nodo.get_token());
            while (ini_p != fin_p) {
                tipo_expresion e = analiza_expresion(**ini_a, f, a, retorno);
                espera((*ini_p == e.tipo), "Tipo de argumento incorrecto.", nodo.get_token(tam - (fin_a - ini_a)));
                ini_a++;
                ini_p++;
            }
            return tipo_expresion{f[t.f].retorno};
        } else {            
            espera(tam == 1, "Error al indicar el tamaño del arreglo.", nodo.get_token(tam - (fin_a - ini_a)));
            tipo_expresion e = analiza_expresion(**ini_a, f, a, retorno);
            espera(e.tipo == NUMERO, "Tamaño de arreglo no valido.", nodo.get_token(tam - (fin_a - ini_a)));
            return tipo_expresion{NUMERO};
        }
    }

    tipo_expresion analiza_expresion_corchetes_posfijo(const expresion_corchetes_posfijo& nodo, funciones& f, ambitos& a, const token& retorno){
        tipo_expresion ex = analiza_expresion(*nodo.ex, f, a, retorno);
        espera(ex.tipo == ARREGLO, "La variable debe ser de tipo array.", nodo.get_token(0));
        tipo_expresion izq = analiza_expresion(*nodo.izq, f, a, retorno);
        espera(izq.tipo == NUMERO, "No es terminal.", nodo.get_token(1));
        if(nodo.der != nullptr){
            tipo_expresion der = analiza_expresion(*nodo.der, f, a, retorno);
            espera(der.tipo == NUMERO, "No es terminal.", nodo.get_token(2));
        }
        return tipo_expresion{ARREGLO};
    }

    tipo_expresion analiza_expresion_arreglo(const expresion_arreglo& nodo, funciones& f, ambitos& a, const token& retorno) {
        auto ini = nodo.elementos.begin();
        auto fin = nodo.elementos.end();
        int tam = fin - ini;
        while (ini != fin) {
            tipo_expresion t = analiza_expresion(**ini, f, a, retorno);
            espera(t.tipo == NUMERO, "No es terminal.", nodo.get_token(tam - (fin - ini)));
            ini++;
        }
        return tipo_expresion{ARREGLO};
    }

    tipo_expresion analiza_expresion(const expresion& nodo, funciones& f, ambitos& a, const token& retorno) {
        if (typeid(nodo) == typeid(expresion_terminal)) {
            return analiza_expresion_terminal(dynamic_cast<const expresion_terminal&>(nodo), f, a, retorno);
        } else if (typeid(nodo) == typeid(expresion_op_prefijo)) {
            return analiza_expresion_op_prefijo(dynamic_cast<const expresion_op_prefijo&>(nodo), f, a, retorno);
        } else if (typeid(nodo) == typeid(expresion_op_binario)) {
            return analiza_expresion_op_binario(dynamic_cast<const expresion_op_binario&>(nodo), f, a, retorno);
        } else if (typeid(nodo) == typeid(expresion_parentesis_posfijo)) {
            return analiza_expresion_parentesis_posfijo(dynamic_cast<const expresion_parentesis_posfijo&>(nodo), f, a, retorno);
        } else if (typeid(nodo) == typeid(expresion_corchetes_posfijo)) {
            return analiza_expresion_corchetes_posfijo(dynamic_cast<const expresion_corchetes_posfijo&>(nodo), f, a, retorno);
        } else if (typeid(nodo) == typeid(expresion_arreglo)) {
            return analiza_expresion_arreglo(dynamic_cast<const expresion_arreglo&>(nodo), f, a, retorno);
        }
    }

    void analiza_sentencia(const sentencia& nodo, funciones& f, ambitos& a, const token& retorno);
    void analiza_declaracion(const sentencia_declaracion& nodo, funciones& f, ambitos& a, const token& retorno) {
        espera(a.back().count(*nodo.nombre) == 0, "No se puede volver a declarar dentro del mismo ambito.", nodo.nombre);
        tipo_expresion decl = analiza_expresion(*nodo.inicializador, f, a, retorno);
        espera(decl.tipo == nodo.tipo->tipo, "Error tipo (Mejorar esto)", nodo.nombre);
        a.back()[*nodo.nombre] = decl.tipo;
    }

    void analiza_if(const sentencia_if& nodo, funciones& f, ambitos& a, const token& retorno) {
        tipo_expresion e_cond = analiza_expresion(*nodo.condicion, f, a, retorno);
        espera(e_cond.tipo == NUMERO, "Condicion no valida.", nodo.condicion->get_token());
        tipo_expresion e_si, e_no;
        std::map<std::string_view, token> m_si;
        a.push_back(m_si);
        for(auto& s : nodo.parte_si){
            analiza_sentencia(*s, f, a, retorno);
        }
        if(!nodo.parte_no.empty()){
            std::map<std::string_view, token> m_no;
            a.push_back(m_no);
            for(auto& s : nodo.parte_no){
                analiza_sentencia(*s, f, a, retorno);
            }
        }
    }

    void analiza_return(const sentencia_return& nodo, funciones& f, ambitos& a, const token& retorno) {
        tipo_expresion t = analiza_expresion(*nodo.ex, f, a, retorno);
        espera(t.tipo == retorno, "Tipo de retorno incorrecto.", nodo.ex->get_token());
    }

    void analiza_sentencia(const sentencia& nodo, funciones& f, ambitos& a, const token& retorno) {
        if (typeid(nodo) == typeid(sentencia_declaracion)) {
            return analiza_declaracion(dynamic_cast<const sentencia_declaracion&>(nodo), f, a, retorno);
        } else if (typeid(nodo) == typeid(sentencia_if)) {
            return analiza_if(dynamic_cast<const sentencia_if&>(nodo), f, a, retorno);
        } else if (typeid(nodo) == typeid(sentencia_return)) {
            return analiza_return(dynamic_cast<const sentencia_return&>(nodo), f, a, retorno);
        }
    }

    template <typename FI>
    void analiza_funcion(FI iter, funciones& f) {
        while(iter->nombre != nullptr){
            std::vector<token> params;
            std::map<std::string_view, token> ap;
            for (auto p : iter->parametros) {
                params.push_back(p.tipo->tipo);                        
                ap[{p.nombre->ini, std::size_t(p.nombre->fin - p.nombre->ini)}] = p.tipo->tipo;
            }
            ambitos a;
            a.push_back(ap);
            f[{iter->nombre->ini, std::size_t(iter->nombre->fin - iter->nombre->ini)}] = datos_funcion{params, iter->retorno->tipo};
            for (const auto& s : iter->sentencias) {
                analiza_sentencia(*s, f, a, iter->retorno->tipo);
            }
            iter++;
        }
    }

}

#endif
