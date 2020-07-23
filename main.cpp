#include "lib/archivo.h"
#include "lib/lexer.h"
#include <iostream>

using char_iterator = concurrent_vector<char>::const_iterator;

int main(int argc, char *argv[]) {

   if (argc < 2) {
      std::cerr << "Archivo de entrada no especificado." << '\n';
      return 0;
   }

   std::ifstream entrada;
   entrada.open(argv[1], std::ifstream::in);
   if (!entrada.is_open()) {
      std::cerr << "No fue posible abrir " << argv[1] << '\n';
      return 0;
   }

   concurrent_vector<char> archivo;
   lee_archivo(archivo, entrada);

   concurrent_vector<token_anotada<char_iterator>> tokens;
   lexer(tokens, archivo.cbegin());

   for(auto it : tokens){
      while(it.ini != it.fin){
         std::cout << *(it.ini++);         
      }
      std::cout << '\t' << it.tipo << '\n';
   } 
      
   std::cout << '\n';

}
