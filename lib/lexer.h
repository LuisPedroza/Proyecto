#ifndef LEXER_H
#define LEXER_H

#include <cctype>

enum token {
    FUNCION,
    ARREGLO,
    NUMERO,
    PARENTESIS_IZQ,
    PARENTESIS_DER,
    LLAVE_IZQ,
    LLAVE_DER,
    CORCHETE_IZQ,
    CORCHETE_DER,
    COMA,
    IDENTIFICADOR,
    LITERAL_NUMERICA,
    PUNTO_COMA,
    DOS_PUNTOS,
    SUMA,
    RESTA,
    MULTIPLICACION,
    DIVISION,
    RESIDUO,
    PISO,
    POTENCIA,
    TAMAÑO_ARREGLO,
    IGUAL,
    DIFERENTE,
    MENOR,
    MENOR_IGUAL,
    MAYOR,
    MAYOR_IGUAL,
    AND,
    OR,
    NOT,
    SLICE,
    IF,
    ELSE,
    RETURN,
    FIN_ARCHIVO
};

std::vector<std::pair<std::string, token>> reservado = {
    {"function", FUNCION},
    {"array", ARREGLO},
    {"number", NUMERO},    
    {"(", PARENTESIS_IZQ},
    {")", PARENTESIS_DER},
    {"{", LLAVE_IZQ},
    {"}", LLAVE_DER},
    {"[", CORCHETE_IZQ},
    {"]", CORCHETE_DER},
    {",", COMA},
    {";", PUNTO_COMA},
    {":", DOS_PUNTOS},
    {"+", SUMA},
    {"-", RESTA},
    {"*", MULTIPLICACION},
    {"/", DIVISION},
    {"%", RESIDUO},
    {"//", PISO},
    {"^", POTENCIA},
    {"#", TAMAÑO_ARREGLO},
    {"=", IGUAL},
    {"!=", DIFERENTE},
    {"<", MENOR},
    {"<=", MENOR_IGUAL},
    {">", MAYOR},
    {">=", MAYOR_IGUAL},
    {"&", AND},
    {"|", OR},
    {"!", NOT},
    {"..", SLICE},
    {"if", IF},
    {"else", ELSE},
    {"return", RETURN}    
};

bool sigue_comentario_linea(char c){
    return c != '\n';
}

bool sigue_comentario_bloque(char c){
    return c != '"';
}

bool inicia_id(char c){
    return std::isalpha(c) || c == '_';
}

bool sigue_id(char c){
    return std::isalpha(c) || std::isdigit(c) || c == '_';
}

bool es_punto(char c){
    return c == '.';
}

template<typename RI>
struct token_anotada {
    token tipo;
    RI ini, fin;
};

template<typename RI, typename P>
void esquiva(RI& iter, P pred){    
    while(pred(*iter)){                
        iter++;        
    }        
}

template<typename RI>
bool consume(const std::string& s, RI& iter){
    if(std::equal(s.begin(), s.end(), iter)){
        iter += s.size();
        return true;
    }
    return false;
}

template<typename RI>
bool consume_id(RI& iter){
    if(inicia_id(*iter)){
        esquiva(++iter, sigue_id);
        return true;
    }
    return false;
}

template<typename RI>
bool consume_literal(RI& iter){
    auto aux = iter;
    esquiva(iter, isdigit);
    if(es_punto(*iter) && std::isdigit(*(iter + 1))){
        esquiva(iter += 2, isdigit);
    }
    return !(aux == iter);
}

template<typename RI>
bool busca_reservada(const std::string& s,RI& iter){    
    if(std::equal(s.begin(), s.end(), iter)){
        iter += s.size();
        return true;
    }
    return false;
}

template<typename RI>
void lexer(concurrent_vector<token_anotada<RI>>& tokens, RI iter) {
    for( ; ; ){
        if(std::isspace(*iter)){
            esquiva(iter, isspace);    
        }        
        if(*iter == '\0'){
            tokens.push_back({FIN_ARCHIVO,iter,iter});
            break;
        }
        auto ini = iter;
        if(*iter == '\''){
            esquiva(iter, sigue_comentario_linea);
        }else if(*iter == '"'){
            esquiva(++iter, sigue_comentario_bloque);
            iter++;            
        }else{
            auto aux = std::find_if(reservado.begin(), reservado.end(), [&](const auto& x){
                return consume(x.first, iter);
            });            
            if(aux != reservado.end()){
                tokens.push_back({aux->second, ini, iter});
            }else if(consume_id(iter)){
                tokens.push_back({IDENTIFICADOR, ini, iter});
            }else if(consume_literal(iter)){
                tokens.push_back({LITERAL_NUMERICA, ini, iter});
            }
        }                
    }
}

#endif