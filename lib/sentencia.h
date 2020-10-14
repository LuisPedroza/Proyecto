#ifndef LIB_SENTENCIA_H
#define LIB_SENTENCIA_H

#include "expresion.h"
#include "parser_aux.h"

namespace lib{
    struct sentencia{
        virtual ~sentencia() = 0;
    };
    sentencia::~sentencia(){
        return;
    }

    struct sentencia_declaracion : sentencia{
        const token_anotada* tipo;
        const token_anotada* nombre;
        std::unique_ptr<expresion> inicializador;

        sentencia_declaracion(const token_anotada* t, const token_anotada* n, std::unique_ptr<expresion>&& i): tipo(t), nombre(n), inicializador(std::move(i)){}
    };

    struct sentencia_if : sentencia{
        std::unique_ptr<expresion> condicion;
        std::vector<std::unique_ptr<sentencia>> parte_si;
        std::vector<std::unique_ptr<sentencia>> parte_no;

        sentencia_if(std::unique_ptr<expresion>&& cond, std::vector<std::unique_ptr<sentencia>>&& si, std::vector<std::unique_ptr<sentencia>>&& no): condicion(std::move(cond)), parte_si(std::move(si)), parte_no(std::move(no)){}
    };

    struct sentencia_return : sentencia{
        std::unique_ptr<expresion> ex;

        sentencia_return(std::unique_ptr<expresion>&& e): ex(std::move(e)){}
    };

    template<typename FI>
    std::unique_ptr<sentencia> parsea_declaracion(FI& iter){
        auto tipo = espera(iter, es_tipo);
        auto nombre = espera(iter, es_identificador);
        espera(iter, IGUAL);
        auto ex = parsea_expresion(iter);
        espera(iter, PUNTO_COMA);
        return std::make_unique<sentencia_declaracion>(tipo, nombre, std::move(ex));
    }

    template<typename FI>
    std::unique_ptr<sentencia> parsea_if(FI& iter){
        espera(iter, IF);
        std::unique_ptr<expresion> cond = parsea_expresion(iter);
        espera(iter, LLAVE_IZQ);
        std::vector<std::unique_ptr<sentencia>> si;
        while(iter->tipo != LLAVE_DER){
            si.push_back(parsea_sentencia(iter));
        }
        espera(iter, LLAVE_DER);
        std::vector<std::unique_ptr<sentencia>> no;
        if(iter->tipo == ELSE){
            ++iter;
            if(iter->tipo == IF){
                no.push_back(parsea_if(iter));
            }else{
                espera(iter, LLAVE_IZQ);
                while(iter->tipo != LLAVE_DER){
                    no.push_back(parsea_sentencia(iter));
                }
                espera(iter, LLAVE_DER);
            }
        }
        return std::make_unique<sentencia_if>(std::move(cond), std::move(si), std::move(no));
    }

    template<typename FI>
    std::unique_ptr<sentencia> parsea_return(FI& iter){
        espera(iter, RETURN);
        auto ex = parsea_expresion(iter);
        espera(iter, PUNTO_COMA);
        return std::make_unique<sentencia_return>(std::move(ex));
    }

    template<typename FI>
    std::unique_ptr<sentencia> parsea_sentencia(FI& iter){
        if(es_tipo(iter->tipo)){
            return parsea_declaracion(iter);
        }else if(iter->tipo == IF){
            return parsea_if(iter);
        }else{
            return parsea_return(iter);
        }
    }

};

#endif
