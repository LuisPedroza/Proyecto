#include "lib/archivo.h"
#include "lib/concurrent_vector.h"
#include "lib/iterator.h"
#include "lib/lexer.h"
#include "tbb/parallel_invoke.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <chrono>

class Timer{
public:
    Timer(){
        ini_t = std::chrono::high_resolution_clock::now();
    }

    ~Timer(){
        stop();    
    }

    void stop(){
        std::chrono::time_point<std::chrono::high_resolution_clock> fin_t;
        fin_t = std::chrono::high_resolution_clock::now();
        auto ini = std::chrono::time_point_cast<std::chrono::microseconds>(ini_t).time_since_epoch().count();
        auto fin = std::chrono::time_point_cast<std::chrono::microseconds>(fin_t).time_since_epoch().count();
        auto tiempo = fin - ini;
        double seg = tiempo * 0.000001;        
        std::cout << seg << '\n';
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> ini_t;
};

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
         Timer t;
         lib::lee_archivo(entrada, lib::back_inserter(archivo));         
         lib::lexer(archivo.cbegin(), lib::back_inserter(tokens));         
      }catch(const std::pair<lib::token_anotada<char_iterator>, const char*>& e){
         std::cout << "Error: " << e.second << "\n";
      }
   } else {
      using char_iterator = lib::input_iterator<lib::concurrent_vector<char>>;
      lib::concurrent_vector<char> archivo;
      lib::concurrent_vector<lib::token_anotada<char_iterator>> tokens;

      try{
         Timer t;
         tbb::parallel_invoke(
            [&] { lib::lee_archivo(entrada, lib::back_inserter(archivo)); },
            [&] { lib::lexer(lib::make_input_iterator(archivo), lib::back_inserter(tokens)); }
         );
         //lib::lee_archivo(entrada, lib::back_inserter(archivo));
         //lib::lexer(lib::make_input_iterator(archivo), lib::back_inserter(tokens));
      }catch(const std::pair<lib::token_anotada<char_iterator>, const char*>& e){
         std::cout << "Error: " << e.second << "\n";
      }
   }

}
