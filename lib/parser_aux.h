#ifndef LIB_PARSER_AUX_H
#define LIB_PARSER_AUX_H

#include "lexer.h"

namespace lib{
    template<typename FI, typename P>
    FI espera(FI& iter, P pred, const char* mensaje = ""){
        if(!pred(iter->tipo)){
            throw std::make_pair(*iter, mensaje);
        }
        return iter++;
    }

    template<typename FI>
    FI espera(FI& iter, token esperada, const char* mensaje = ""){
        return espera(iter, [&](token tipo) {
            return tipo == esperada;
        }, mensaje);
    }

    int precedencia(token t) {
        if(t == OR){
            return 1;
        }else if(t == AND){
            return 2;
        }else if(t == IGUAL || t == DIFERENTE){
            return 3;
        }else if(t == MENOR || t == MENOR_IGUAL || t == MAYOR || t == MAYOR_IGUAL){
            return 4;
        }else if(t == SUMA || t == RESTA){
            return 5;
        }else if(t == MULTIPLICACION || t == DIVISION || t == PISO || t == RESIDUO){
            return 6;
        }else if(t == POTENCIA){
            return 7;
        }
        return 0;
    }

    enum asociatividad{
        DERECHA = 0,
        IZQUIERDA
    };

    int asociatividad(token t) {
        return (t == POTENCIA ? DERECHA : IZQUIERDA);
    }

    bool es_operador_prefijo(token t){
        return t == TAMANYO_ARREGLO || t == RESTA || t == NOT;
    }   

    bool es_operador_binario(token t){
        return t == IGUAL || t == SUMA || t == RESTA || t == DIVISION || t == MULTIPLICACION || t == RESIDUO || t == PISO || t == POTENCIA || t == AND || t == OR || t == DIFERENTE;
    }

    bool es_operador_relacional(token t){
        return t == MENOR || t == MENOR_IGUAL || t == MAYOR || t == MAYOR_IGUAL;
    }

    bool es_tipo(token t){
        return t == NUMERO || t == ARREGLO;
    }

    bool es_identificador(token t){
        return t == IDENTIFICADOR;
    }

    bool es_literal(token t){
        return t == LITERAL_NUMERICA;
    }

    bool es_terminal(token t){
        return es_identificador(t) || es_literal(t);
    }

    bool es_funcion(token t){
        return t == IDENTIFICADOR || t == MAIN;
    }
};

#endif
