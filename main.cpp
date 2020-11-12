#include "lib/archivo.h"
#include "lib/concurrent.h"
#include "lib/lexer.h"
#include "lib/parser.h"
#include "lib/semantico.h"
#include "lib/codegen.h"
#include "lib/debug.h"
#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <future>
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

template<typename ET, typename EH>
void parallel_invoke(std::initializer_list<std::function<void( )>> funcs, EH&& error_handler) {
   std::future<void> hilos[funcs.size( )];
   for (int i = 0; i < funcs.size( ); ++i) {
      hilos[i] = std::async(std::launch::async, [&, i]{
         try {
            funcs.begin( )[i]( );
         } catch(const ET& e){
            error_handler(e);
         }
      });
   }

   for (int i = 0; i < funcs.size( ); ++i) {
      hilos[i].get( );
   }
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

   std::string_view ruta(argv[1]);
   auto pos = ruta.find('.');
   ruta.remove_suffix(ruta.size() - pos);

   std::size_t tam_archivo = std::filesystem::file_size(argv[1]);
   bool debug = (argc >= 3 && std::strcmp(argv[2], "debug") == 0);
   bool debug_tiempo = (argc >= 4 && std::strcmp(argv[3], "-t") == 0);
   bool secuencial = (debug || argc >= 3 && std::strcmp(argv[2], "secuencial") == 0);

   auto t0 = std::chrono::high_resolution_clock::now( );
   if (secuencial) {
      std::vector<char> archivo(tam_archivo + 1 + lib::read_size);
      std::vector<lib::token_anotada> tokens;
      std::vector<lib::declaracion_funcion> arbol;
      std::map<std::string_view, lib::datos_funcion> funciones;
      std::ostringstream codigo;

      try{
         auto t_ini = std::chrono::high_resolution_clock::now( );
         lib::lee_archivo(entrada, archivo.data( ));
         auto t_fin = std::chrono::high_resolution_clock::now( );
         if (debug) {
            std::cerr << "Archivo leido: " << std::chrono::duration_cast<std::chrono::milliseconds>(t_fin - t_ini).count( ) / 1000.0 << "\n";
            if(!debug_tiempo){
               for(char c : archivo) {
                  if (c == '\0') {
                     break;
                  }
                  std::cerr << c;
               }
            }
         }
         t_ini = std::chrono::high_resolution_clock::now( );
         lib::lexer(archivo.data( ), std::back_inserter(tokens));
         t_fin = std::chrono::high_resolution_clock::now( );
         if (debug) {
            std::cerr << "Tokens leidos: " << std::chrono::duration_cast<std::chrono::milliseconds>(t_fin - t_ini).count( ) / 1000.0 << "\n";
            if(!debug_tiempo){
               for(auto i : tokens){
                  while(i.ini != i.fin){
                     std::cerr << *i.ini++;
                  }
                  std::cerr << '\t' << i.tipo << '\n';
               }
            }
         }
         t_ini = std::chrono::high_resolution_clock::now( );
         lib::parser(tokens.data(), std::back_inserter(arbol));
         t_fin = std::chrono::high_resolution_clock::now( );
         if(debug){
            std::cerr << "Arbol generado: " << std::chrono::duration_cast<std::chrono::milliseconds>(t_fin - t_ini).count( ) / 1000.0 << "\n";
            if(!debug_tiempo){
               for(auto const& i : arbol){
                  std::cerr << i;
               }
               std::cerr << '\n';
            }
         }
         t_ini = std::chrono::high_resolution_clock::now( );
         lib::analiza_funcion(arbol.data(), funciones);
         t_fin = std::chrono::high_resolution_clock::now( );
         if(debug){
            std::cerr << "Arbol analizado: " << std::chrono::duration_cast<std::chrono::milliseconds>(t_fin - t_ini).count( ) / 1000.0 << "\n";
         }
         t_ini = std::chrono::high_resolution_clock::now( );
         lib::escribe_funcion(arbol.data(), codigo);
         t_fin = std::chrono::high_resolution_clock::now( );
         if(debug){
            std::cerr << "Codigo generado: " << std::chrono::duration_cast<std::chrono::milliseconds>(t_fin - t_ini).count( ) / 1000.0 << "\n";
         }

         std::ofstream(std::string(ruta) + ".cpp") << std::move(codigo).str();
      }catch(const std::pair<lib::token_anotada, const char*>& e){
         reporta_error(std::cerr, archivo.data( ), archivo.data( ) + archivo.size( ), e);
         std::exit(0);
      }
   } else {
      lib::concurrent_buffer<char> archivo(tam_archivo + 1 + lib::read_size);
      lib::concurrent_buffer<lib::token_anotada> tokens(tam_archivo + 1);
      lib::concurrent_buffer<lib::declaracion_funcion> arbol((tam_archivo / 8) + bool(tam_archivo % 8) + 1);
      std::map<std::string_view, lib::datos_funcion> funciones;
      std::ostringstream codigo;

      parallel_invoke<const std::pair<lib::token_anotada, const char*>>({
         [&] { lib::lee_archivo(entrada, archivo.output_iterator( )); },
         [&] { lib::lexer(archivo.inspect_iterator( ), tokens.output_iterator( )); },
         [&] { lib::parser(tokens.inspect_iterator( ), arbol.output_iterator( )); },
         [&] { lib::analiza_funcion(arbol.inspect_iterator(), funciones); },
         [&] { lib::escribe_funcion(arbol.inspect_iterator(), codigo); }
      }, [&](auto e) {
         reporta_error(std::cerr, archivo.begin( ), archivo.end( ), e);
         std::exit(0);
      });

      std::ofstream(std::string(ruta) + ".cpp") << std::move(codigo).str();
   }

   auto t1 = std::chrono::high_resolution_clock::now( );
   std::cerr << "Tiempo Total: " << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count( ) / 1000.0 << "\n";
}
