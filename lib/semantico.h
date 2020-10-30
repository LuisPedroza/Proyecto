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

    struct tipo_expresion {
        token tipo;          // NUMBER, ARRAY o FUNCTION
        std::string_view f;  // si el token fue FUNCTION, el nombre de la función
    };

    using funciones = std::map<std::string_view, datos_funcion>;
    using ambitos = std::vector<std::map<std::string_view, token>>;
    using ambito = std::map<std::string_view, token>;

    void declara(ambito& a, const token_anotada& id, const token& tipo){        
        asercion(a.count(id) == 0, std::make_pair(id, "No se puede volver a declarar dentro del mismo ambito."));

    }

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
            asercion(declarada_v || declarada_f, std::make_pair(*nodo.get_token(), "Variable no declarada."));                      
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
            asercion(t.tipo == ARREGLO, std::make_pair(*nodo.get_token(), "El operador se debe usar sobre un arreglo."));
            return tipo_expresion{NUMERO};
        } else if (tipo == RESTA) {            
            asercion(t.tipo == NUMERO, std::make_pair(*nodo.get_token(), "El operador se debe usar sobre un tipo numerico."));
            return tipo_expresion{NUMERO};
        } else {            
            asercion(t.tipo == NUMERO, std::make_pair(*nodo.get_token(), "El operador se debe usar sobre un tipo numerico."));
            return tipo_expresion{NUMERO};
        }
    }

    tipo_expresion analiza_expresion_op_binario(const expresion_op_binario& nodo, funciones& f, ambitos& a, const token& retorno){
        tipo_expresion izq = analiza_expresion(*nodo.izq, f, a, retorno);
        tipo_expresion der = analiza_expresion(*nodo.der, f, a, retorno);
        if(es_operador_rel(nodo.operador->tipo) || es_operador_log(nodo.operador->tipo) || (es_operador_arit(nodo.operador->tipo) && nodo.operador->tipo != MULTIPLICACION)){            
            asercion(izq.tipo == NUMERO, std::make_pair(*nodo.izq->get_token(), "Debe ser de tipo numerico."));            
            asercion(der.tipo == NUMERO, std::make_pair(*nodo.der->get_token(), "Debe ser de tipo numerico."));
            return tipo_expresion{NUMERO};
        }else{
            if(izq.tipo == ARREGLO){                
                asercion(der.tipo == ARREGLO, std::make_pair(*nodo.der->get_token(), "Debe ser un arreglo."));
                return tipo_expresion{ARREGLO};
            }else if(izq.tipo == NUMERO){                
                asercion(der.tipo == NUMERO, std::make_pair(*nodo.der->get_token(), "Debe ser de tipo numerico."));
                return tipo_expresion{NUMERO};
            }else{                
                asercion(false, std::make_pair(*nodo.get_token(), "Tipos incorrectos."));
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
            asercion((fin_p - ini_p) == (fin_a - ini_a), std::make_pair(*nodo.get_token(), "Numero incorrecto de argumentos."));
            while (ini_p != fin_p) {
                tipo_expresion e = analiza_expresion(**ini_a, f, a, retorno);                
                asercion(*ini_p == e.tipo, std::make_pair(*nodo.parametros[tam - (fin_a - ini_a)]->get_token(), "Tipo de argumento incorrecto."));
                ini_a++;
                ini_p++;
            }
            return tipo_expresion{f[t.f].retorno};
        } else {                        
            asercion(tam == 1, std::make_pair(*nodo.parametros[tam - (fin_a - ini_a)]->get_token(), "Error al indicar el tamaño del arreglo."));
            tipo_expresion e = analiza_expresion(**ini_a, f, a, retorno);            
            asercion(e.tipo == NUMERO, std::make_pair(*nodo.parametros[tam - (fin_a - ini_a)]->get_token(), "Tamaño de arreglo no valido."));
            return tipo_expresion{NUMERO};
        }
    }

    tipo_expresion analiza_expresion_corchetes_posfijo(const expresion_corchetes_posfijo& nodo, funciones& f, ambitos& a, const token& retorno){
        tipo_expresion ex = analiza_expresion(*nodo.ex, f, a, retorno);        
        asercion(ex.tipo == ARREGLO, std::make_pair(*nodo.ex->get_token(), "La variable debe ser de tipo array."));
        tipo_expresion izq = analiza_expresion(*nodo.izq, f, a, retorno);        
        asercion(izq.tipo == NUMERO, std::make_pair(*nodo.izq->get_token(), "No es terminal."));
        if(nodo.der != nullptr){
            tipo_expresion der = analiza_expresion(*nodo.der, f, a, retorno);        
            asercion(der.tipo == NUMERO, std::make_pair(*nodo.der->get_token(), "No es terminal"));
        }
        return tipo_expresion{ARREGLO};
    }

    tipo_expresion analiza_expresion_arreglo(const expresion_arreglo& nodo, funciones& f, ambitos& a, const token& retorno) {
        auto ini = nodo.elementos.begin();
        auto fin = nodo.elementos.end();
        int tam = fin - ini;
        while (ini != fin) {
            tipo_expresion t = analiza_expresion(**ini, f, a, retorno);            
            asercion(t.tipo == NUMERO, std::make_pair(*nodo.elementos[tam - (fin - ini)]->get_token(), "No es terminal."));
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
        asercion(a.back().count(*nodo.nombre) == 0, std::make_pair(*nodo.nombre, "No se puede volver a declarar dentro del mismo ambito."));
        declara(a.back(), *nodo.nombre, nodo.tipo->tipo);
        tipo_expresion decl = analiza_expresion(*nodo.inicializador, f, a, retorno);        
        asercion(decl.tipo == nodo.tipo->tipo, std::make_pair(nodo.nombre, "Error tipo (Mejorar esto)"));
        a.back()[*nodo.nombre] = decl.tipo;
    }

    void analiza_if(const sentencia_if& nodo, funciones& f, ambitos& a, const token& retorno) {
        tipo_expresion e_cond = analiza_expresion(*nodo.condicion, f, a, retorno);        
        asercion(e_cond.tipo == NUMERO, std::make_pair(*nodo.condicion->get_token(), "Condicion no valida."));
        tipo_expresion e_si, e_no;
        ambito m_si;
        a.push_back(m_si);
        for(auto& s : nodo.parte_si){
            analiza_sentencia(*s, f, a, retorno);
        }
        if(!nodo.parte_no.empty()){
            ambito m_no;
            a.push_back(m_no);
            for(auto& s : nodo.parte_no){
                analiza_sentencia(*s, f, a, retorno);
            }
        }
    }

    void analiza_return(const sentencia_return& nodo, funciones& f, ambitos& a, const token& retorno) {
        tipo_expresion t = analiza_expresion(*nodo.ex, f, a, retorno);        
        asercion(t.tipo == retorno, std::make_pair(*nodo.ex->get_token(), "Tipo de retorno incorrecto."));
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
            ambito ap;
            for (auto p : iter->parametros) {
                params.push_back(p.tipo->tipo);
                declara(ap, *p.nombre, p.tipo->tipo);
                ap[*p.nombre] = p.tipo->tipo;
            }
            ambitos a;
            a.push_back(ap);
            f[*iter->nombre] = datos_funcion{params, iter->retorno->tipo};
            for (const auto& s : iter->sentencias) {
                analiza_sentencia(*s, f, a, iter->retorno->tipo);
            }
            iter++;
        }
    }

}

#endif
