#ifndef LIB_LEXER_H
#define LIB_LEXER_H

#include <cctype>
#include <string_view>
#include <utility>
#include <vector>

namespace lib {
   enum token {
       FUNCION,
       MAIN,
       RUNTIME,
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
       TAMANYO_ARREGLO,
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
       DESCONOCIDO,
       FIN_ARCHIVO
   };

   std::vector<std::pair<std::string_view, token>> reservado = {
       {"function", FUNCION},
       {"main", MAIN},
       {"runtime", RUNTIME},
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
       {"//", PISO},
       {"/", DIVISION},
       {"%", RESIDUO},
       {"^", POTENCIA},
       {"#", TAMANYO_ARREGLO},
       {"=", IGUAL},
       {"!=", DIFERENTE},
       {"<=", MENOR_IGUAL},
       {"<", MENOR},
       {">=", MAYOR_IGUAL},
       {">", MAYOR},
       {"&", AND},
       {"|", OR},
       {"!", NOT},
       {"..", SLICE},
       {"if", IF},
       {"else", ELSE},
       {"return", RETURN}
   };

   bool sigue_comentario_linea(char c){
       return c != '\n' && c != EOF;
   }

   bool sigue_comentario_bloque(char c){
       return c != '"' && c != EOF;
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

   struct token_anotada {
        token tipo;
        const char* ini;
        const char* fin;

        operator std::string_view() const{
            return {ini, std::size_t(fin - ini)};
        }
   };

   template<typename FI, typename P>
   void esquiva(FI& iter, P pred){
       while(pred(*iter)){
           ++iter;
       }
   }

   template<typename FI>
   bool consume_reservado(const std::string_view& s, FI& iter){
       auto res = std::mismatch(s.begin(), s.end(), iter);
       if (res.first == s.end() && (!inicia_id(*iter) || !sigue_id(*res.second))) {
          iter = res.second;
          return true;
       }
       return false;
   }

   template<typename FI>
   bool consume_id(FI& iter){
       if(inicia_id(*iter)){
           esquiva(++iter, sigue_id);
           return true;
       }
       return false;
   }

   template<typename FI>
   bool consume_literal(FI& iter){
       auto aux = iter;
       esquiva(iter, isdigit);
       if(aux == iter){
           return false;
       }
       if(es_punto(*iter) && std::isdigit(*std::next(iter))){
           std::advance(iter, 2);
           esquiva(iter, isdigit);
       }
       return !(aux == iter);
   }

   template<typename FI, typename OI>
   void lexer(FI iter, OI&& salida) {
       for( ; ; ){
           esquiva(iter, isspace);
           if(*iter == '\0'){
               *salida++ = token_anotada{FIN_ARCHIVO,iter,iter};
               break;
           }
           auto ini = iter;
           if(*iter == '\''){
               esquiva(++iter, sigue_comentario_linea);
           }else if(*iter == '"'){
               esquiva(++iter, sigue_comentario_bloque);
               ++iter;
           }else{
               auto aux = std::find_if(reservado.begin(), reservado.end(), [&](const auto& x){
                   return consume_reservado (x.first, iter);
               });
               if(aux != reservado.end()){
                   *salida++ = token_anotada{aux->second, ini, iter};
               }else if(consume_id(iter)){
                   *salida++ = token_anotada{IDENTIFICADOR, ini, iter};
               }else if(consume_literal(iter)){
                   *salida++ = token_anotada{LITERAL_NUMERICA, ini, iter};
               }else{
                   throw std::make_pair(token_anotada{DESCONOCIDO, ini, iter}, "Simbolo desconocido");
               }
           }
       }
   }
}

#endif
