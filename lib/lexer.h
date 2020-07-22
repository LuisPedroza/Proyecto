#ifndef LEXER_H
#define LEXER_H

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
    {"return", RETURN}    
};

template<typename RI>
struct token_anotada {
    token tipo;
    RI ini, fin;
};

template<typename RI>
void esquiva(RI& iter, char fin){    
    while(*iter != fin){        
        iter++;
    }    
    iter++;
}

template<typename RI>
void lexer(concurrent_vector<token_anotada<RI>>& tokens, RI iter) {
    
}

#endif