#ifndef LIB_EXPRESION_H
#define LIB_EXPRESION_H

#include "parser_aux.h"
#include<memory>

namespace lib{
    struct expresion{
        virtual ~expresion() = 0;
        virtual const token_anotada* get_token() const = 0;
    };
    expresion::~expresion(){
        return;
    }

    struct expresion_terminal : expresion{
        const token_anotada* t;

        expresion_terminal(const token_anotada* tp): t(tp){}

        const token_anotada* get_token() const {
            return t;
        }
    };

    struct expresion_op_prefijo: expresion{
        const token_anotada* operador;
        std::unique_ptr<expresion> sobre;

        expresion_op_prefijo(const token_anotada* op, std::unique_ptr<expresion>&& s): operador(op), sobre(std::move(s)){}

        const token_anotada* get_token() const {
            return operador;
        }
    };

    struct expresion_op_binario : expresion{
        std::unique_ptr<expresion> izq;
        const token_anotada* operador;
        std::unique_ptr<expresion> der;

        expresion_op_binario(std::unique_ptr<expresion>&& i, const token_anotada* op, std::unique_ptr<expresion>&& d): izq(std::move(i)), operador(op), der(std::move(d)){}

        const token_anotada* get_token() const {
            return operador;
        }

        const token_anotada* get_token(int lado) const {
            return (lado == 0 ? izq->get_token() : der->get_token());
        }
    };

    struct expresion_parentesis_posfijo : expresion{
        std::unique_ptr<expresion> func;
        std::vector<std::unique_ptr<expresion>> parametros;

        expresion_parentesis_posfijo(std::unique_ptr<expresion>&& f, std::vector<std::unique_ptr<expresion>>&& p): func(std::move(f)), parametros(std::move(p)){}

        const token_anotada* get_token() const {
            return func->get_token();
        }

        const token_anotada* get_token(int ind) const {
            auto ini = parametros.begin();
            ini += ind;
            return (*ini)->get_token();
        }
    };

    struct expresion_corchetes_posfijo: expresion{
        std::unique_ptr<expresion> ex, izq, der;

        expresion_corchetes_posfijo(std::unique_ptr<expresion>&& e, std::unique_ptr<expresion>&& i, std::unique_ptr<expresion>&& d): ex(std::move(e)), izq(std::move(i)), der(std::move(d)){}

        const token_anotada* get_token() const{
            return nullptr;
        }

        const token_anotada* get_token(int op) const {
            if(op == 0){
                return ex->get_token();
            }else if(op == 1){
                return izq->get_token();
            }else{
                return der->get_token();
            }
        }
    };

    struct expresion_arreglo : expresion{
        std::vector<std::unique_ptr<expresion>> elementos;

        expresion_arreglo(std::vector<std::unique_ptr<expresion>>&& e) : elementos(std::move(e)){}

        const token_anotada* get_token() const{
            return (*elementos.begin())->get_token();
        }

        const token_anotada* get_token(int ind) const {
            auto ini = elementos.begin();
            std::advance(ini, ind);
            return (*ini)->get_token();
        }
    };

    template<typename FI>
    std::unique_ptr<expresion> parsea_expresion(FI& iter);

    template<typename FI>
    std::unique_ptr<expresion> parsea_expresion_primaria(FI& iter){
        if(es_terminal(iter->tipo)){
            return std::make_unique<expresion_terminal>(iter++);
        }else if(iter->tipo == PARENTESIS_IZQ){
            std::unique_ptr<expresion> ex = parsea_expresion(++iter);
            espera(iter, PARENTESIS_DER);
            return ex;
        }else{
            std::vector<std::unique_ptr<expresion>> elem;
            espera(iter, CORCHETE_IZQ);
            while(iter->tipo != CORCHETE_DER){
                elem.push_back(parsea_expresion(iter));
                if(iter->tipo != CORCHETE_DER){
                   espera(iter, COMA);
                }
            }
            espera(iter, CORCHETE_DER);
            return std::make_unique<expresion_arreglo>(std::move(elem));
        }
    }

    template<typename FI>
    std::unique_ptr<expresion> parsea_expresion_unaria(FI& iter){
        if(es_operador_prefijo(iter->tipo)){
            auto operador = iter++;
            return std::make_unique<expresion_op_prefijo>(operador, parsea_expresion_unaria(iter));
        }
        auto ex = parsea_expresion_primaria(iter);
        while(iter->tipo == PARENTESIS_IZQ || iter->tipo == CORCHETE_IZQ){
            if(iter->tipo == PARENTESIS_IZQ){
                ++iter;
                std::vector<std::unique_ptr<expresion>> parametros;
                while(iter->tipo != PARENTESIS_DER){
                    parametros.push_back(parsea_expresion(iter));
                    if(iter->tipo != PARENTESIS_DER){
                        espera(iter, COMA);
                    }
                }
                espera(iter, PARENTESIS_DER);
                ex = std::make_unique<expresion_parentesis_posfijo>(std::move(ex), std::move(parametros));
            }else if(iter->tipo == CORCHETE_IZQ){
                ++iter;
                auto izq = parsea_expresion(iter);
                if(iter->tipo == CORCHETE_DER){
                    ++iter;
                    ex = std::make_unique<expresion_corchetes_posfijo>(std::move(ex), std::move(izq), nullptr);
                }else{
                    espera(iter, SLICE);
                    auto der = parsea_expresion(iter);
                    espera(iter, CORCHETE_DER);
                    ex = std::make_unique<expresion_corchetes_posfijo>(std::move(ex), std::move(izq), std::move(der));
                }
            }
        }
        return ex;
    }

    template<typename FI>
    std::unique_ptr<expresion> parsea_expresion_n_aria(FI& iter, int prec){
        std::unique_ptr<expresion> ex = parsea_expresion_unaria(iter);
        while((es_operador_binario(iter->tipo) || es_operador_rel(iter->tipo)) && precedencia(iter->tipo) >= prec){
            auto op = iter++;
            ex = std::make_unique<expresion_op_binario>(std::move(ex), op, parsea_expresion_n_aria(iter, precedencia(op->tipo) + asociatividad(op->tipo)));
        }
        return ex;
    }

    template<typename FI>
    std::unique_ptr<expresion> parsea_expresion(FI& iter){
        return parsea_expresion_n_aria(iter, 0);
    }
};

#endif
