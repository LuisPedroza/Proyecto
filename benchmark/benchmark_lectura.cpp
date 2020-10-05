#include <iostream>
#include <chrono>
#include <deque>
#include <fstream>
#include "lib/concurrent_vector.h"
#include "tbb/concurrent_queue.h"
#include <cstring>
#include <fstream>
#include <iterator>
#include <vector>

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


void lee_archivo1(const char* ruta, char* oi, int len){
   std::ifstream ifs(ruta, std::ios::in|std::ios::binary);
   constexpr int tam = 4096;
   char pagina[tam];
   while (ifs) {
      ifs.read(pagina, tam);
      int leidos = ifs.gcount();
      if(leidos == 0){
         break;
      }else{
         oi = std::copy(pagina, pagina + leidos, oi);
      }
   }
}

void lee_archivo2(const char* ruta, char* oi, int len){
   std::ifstream ifs(ruta, std::ios::in|std::ios::binary);
   constexpr int tam = 4096 * 8;
   do {
      ifs.read(oi, tam);
      oi += ifs.gcount();
   } while (ifs);
}

void lee_archivo3(const char* ruta, char* oi, int len){
   std::ifstream ifs(ruta, std::ios::in|std::ios::binary);
   ifs.read(oi, len);
}

int main(int argc, char *argv[]) {

   int n = 1000;
   constexpr int len = 12582912;
   char buffer[len];

   const char* ruta = "test.txt";
   lee_archivo1(ruta, buffer, len);
lee_archivo1(ruta, buffer, len);
   {
      /*std::cout << "lee_archivo1 : \t";
      Timer t;
      for(int i = 0 ; i < n ; ++i){
         lee_archivo1(ruta, buffer, len);
      }*/
   }

   {
      std::cout << "lee_archivo2 : \t";
      Timer t;
      for(int i = 0 ; i < n ; ++i){
         lee_archivo2(ruta, buffer, len);
      }
   }

   {
      std::cout << "lee_archivo3 : \t";
      Timer t;
      for(int i = 0 ; i < n ; ++i){
         lee_archivo3(ruta, buffer, len);
      }
   }
}
