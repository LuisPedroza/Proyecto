#include "lib/archivo.h"
#include "lib/concurrent_vector.h"
#include "lib/iterator.h"
#include "lib/lexer.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

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

   if (secuencial) {
      using char_iterator = std::vector<char>::const_iterator;
      std::vector<char> archivo;
      std::vector<lib::token_anotada<char_iterator>> tokens;

      try{
         lib::lee_archivo(entrada, lib::back_inserter(archivo));
         if (debug) {
            std::cout << "Archivo leido." << '\n';
            for(char c : archivo){
               std::cout << c;
            }
         }
         lib::lexer(archivo.cbegin(), lib::back_inserter(tokens));
         if (debug) {
            std::cout << "Tokens leidos." << '\n';
            for(auto i : tokens){
               while(i.ini != i.fin){
                  std::cout << *i.ini++;
               }
               std::cout << '\t' << i.tipo << '\n';
            }
         }
      }catch(const std::pair<lib::token_anotada<char_iterator>, const char*>& e){
         std::cout << "Error: " << e.second << "\n";
      }
   } else {
      using char_iterator = lib::concurrent_vector<char>::const_iterator;
      lib::concurrent_vector<char> archivo;
      lib::concurrent_vector<lib::token_anotada<char_iterator>> tokens;

      try{
         lib::lee_archivo(entrada, lib::back_inserter(archivo));
         lib::lexer(archivo.cbegin(), lib::back_inserter(tokens));
      }catch(const std::pair<lib::token_anotada<char_iterator>, const char*>& e){
         std::cout << "Error: " << e.second << "\n";
      }
   }

}
