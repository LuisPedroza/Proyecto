#include <iostream>
#include <chrono>
#include <deque>
#include <fstream>
#include <filesystem>
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
   auto archivo = std::fopen(ruta, "rb");
   constexpr int tam = 4096;
   char pagina[tam];
   for (;;) {
      auto leidos = std::fread(oi, 1, tam, archivo);
      if (leidos == 0) {
         break;
      }
      oi = std::copy(pagina, pagina + leidos, oi);
   }
   std::fclose(archivo);
}

void lee_archivo2(const char* ruta, char* oi, int len){
   auto archivo = std::fopen(ruta, "rb");
   constexpr int tam = 4096 * 8;
   for (;;) {
      auto leidos = fread(oi, 1, tam, archivo);
      if (leidos == 0) {
         break;
      }
      oi += leidos;
   }
   std::fclose(archivo);
}

void lee_archivo3(const char* ruta, char* oi, int len){
   auto archivo = std::fopen(ruta, "rb");
   std::fread(oi, 1, len, archivo);
   std::fclose(archivo);
}

int main(int argc, char *argv[]) {
   const char* ruta = "test.txt";

   int n = 1000;
   int len = std::filesystem::file_size(ruta);
   auto buffer = new char[len + 1];

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
