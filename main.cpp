#include "lib/archivo.h"
#include "lib/concurrent.h"
#include "lib/lexer.h"
#include "tbb/parallel_invoke.h"
#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>

int linea_de(const char* ini, const lib::token_anotada& t) {
   return 1 + std::count(ini, t.ini, '\n');
}

int columna_de(const char* ini, const lib::token_anotada& t) {
   return 1 + (t.ini - std::find(std::reverse_iterator(t.ini), std::reverse_iterator(ini), '\n').base( ));
}

std::string_view vista_de(const lib::token_anotada& t, int len, const char* fin) {
   return { t.ini, std::size_t(std::find(t.ini, (fin - t.ini <= len ? fin : t.ini + len), '\n') - t.ini) };
}

void reporta_error(std::ostream& os, const char* ini, const char* fin, const std::pair<lib::token_anotada, const char*>& e) {
   os << "Error en linea " << linea_de(ini, e.first) << ", columna " << columna_de(ini, e.first) << ":\n"
      << "\t" << vista_de(e.first, 10, fin) << "\n"
      << "\t^\n"
      << e.second << "\n";
}

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

   std::size_t tam_archivo = std::filesystem::file_size(argv[1]);
   bool debug = (argc >= 3 && std::strcmp(argv[2], "debug") == 0);
   bool secuencial = (debug || argc >= 3 && std::strcmp(argv[2], "secuencial") == 0);

   auto t0 = std::chrono::high_resolution_clock::now( );
   if (secuencial) {
      std::vector<char> archivo(tam_archivo);
      std::vector<lib::token_anotada> tokens;

      try{
         lib::lee_archivo(entrada, archivo.data( ));
         if (debug) {
            std::cout << "Archivo leido." << '\n';
            for(char c : archivo){
               std::cout << c;
            }
         }
         lib::lexer(archivo.data( ), std::back_inserter(tokens));
         if (debug) {
            std::cout << "Tokens leidos." << '\n';
            for(auto i : tokens){
               while(i.ini != i.fin){
                  std::cout << *i.ini++;
               }
               std::cout << '\t' << i.tipo << '\n';
            }
         }
      }catch(const std::pair<lib::token_anotada, const char*>& e){
         reporta_error(std::cout, archivo.data( ), archivo.data( ) + archivo.size( ), e);
      }
   } else {
      lib::concurrent_buffer<char> archivo(tam_archivo);
      lib::concurrent_buffer<lib::token_anotada> tokens(tam_archivo);

      try{
         tbb::parallel_invoke(
            [&] { lib::lee_archivo(entrada, archivo.resizable_end( )); },
            [&] { lib::lexer(lib::concurrent_inspect_iterator(archivo), tokens.resizable_end( )); }
         );
         //lib::lee_archivo(entrada, archivo.resizable_end( ));
         //lib::lexer(lib::concurrent_inspect_iterator(archivo), std::back_inserter(tokens));
      }catch(const std::pair<lib::token_anotada, const char*>& e){
         reporta_error(std::cout, archivo.begin( ), archivo.end( ), e);
      }
   }

   auto t1 = std::chrono::high_resolution_clock::now( );
   std::clog << "Tiempo: " << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count( ) / 1000.0 << "\n";
}
