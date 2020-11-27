#ifndef LIB_PARSER_H
#define LIB_PARSER_H

#include "sentencia.h"
#include <vector>
#include <memory>

namespace lib{
    struct declaracion_funcion {
        struct parametro {
            const token_anotada* tipo;
            const token_anotada* nombre;
        };

        const token_anotada* nombre;
        std::vector<parametro> parametros;
        const token_anotada* retorno;
        std::vector<std::unique_ptr<sentencia>> sentencias;
    };

    template<typename FI>
    std::vector<declaracion_funcion::parametro> parsea_parametros(FI& iter){
        std::vector<declaracion_funcion::parametro> p;
        while(es_tipo(iter->tipo)){
            auto tipo = espera(iter, es_tipo, "Se esperaba un tipo de dato.");
            auto nombre = espera(iter, IDENTIFICADOR, "Se esperaba un identificador");
            p.push_back({tipo, nombre});
            if(iter->tipo != COMA){
                break;
            }
            espera(iter, COMA, "Se esperaba una ,");
        }
        return p;
    }

    template<typename FI>
    declaracion_funcion parsea_funcion(FI& iter){
        espera(iter, FUNCION, "Se esperaba la palabra reservada function.");
        auto nombre = espera(iter, es_funcion);
        espera(iter, PARENTESIS_IZQ, "Se esperaba un (");
        auto parametros = parsea_parametros(iter);
        espera(iter, PARENTESIS_DER, "Se esperaba un )");
        espera(iter, DOS_PUNTOS, "Se esperaba un :");
        auto retorno = espera(iter, es_tipo);
        espera(iter, LLAVE_IZQ, "Se esperaba una {");
        std::vector<std::unique_ptr<sentencia>> sentencias;
        while(iter->tipo != LLAVE_DER){
            sentencias.push_back(parsea_sentencia(iter));
        }
        espera(iter, LLAVE_DER);
        return declaracion_funcion{nombre, parametros, retorno, std::move(sentencias)};
    }

    template<typename FI, typename OI>
    void parser(FI iter, OI&& salida){
        while(iter->tipo != FIN_ARCHIVO){
            *salida++ = parsea_funcion(iter);
        }
        *salida++ = declaracion_funcion{ };           // el equivalente de FIN_ARCHIVO pero para declaraciones; será útil tenerlo en el semántico
    }
};

#endif
 