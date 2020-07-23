#include "lib/archivo.h"
#include "lib/lexer.h"
#include <cstring>
#include <iostream>
#include <iterator>

int main(int argc, char *argv[]) {
   if (argc < 2) {
      std::cerr << "Archivo de entrada no especificado\n";
      return 0;
   }

   std::ifstream entrada(argv[1]);
   if (!entrada.is_open()) {
      std::cerr << "No fue posible abrir " << argv[1] << "\n";
      return 0;
   }

   bool debug = (argc >= 3 && std::strcmp(argv[2], "debug") == 0);
   bool secuencial = (debug || argc >= 3 && std::strcmp(argv[2], "secuencial") == 0);

   using char_iterator = concurrent_vector<char>::const_iterator;
   concurrent_vector<char> archivo;
   concurrent_vector<token_anotada<char_iterator>> tokens;

   try{
      lee_archivo(entrada, append_inserter(archivo));
      lexer(archivo.cbegin(), std::back_inserter(tokens));
   }catch(const std::pair<token_anotada<char_iterator>, const char*>& e){
      std::cout << "Error: " << e.second << "\n";
   }



   /* for(auto it : tokens){
      while(it.ini != it.fin){
         std::cout << *(it.ini++);
      }
      std::cout << '\t' << it.tipo << '\n';
   }

   std::cout << '\n'; */

}
