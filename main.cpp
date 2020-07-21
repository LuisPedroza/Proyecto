#include "vector.h"
#include <iostream>
#include <fstream>

void lee_archivo(concurrent_vector<char>& v, std::ifstream& ifs){
   int tam = 4096;
   std::vector<char> pagina(tam);
   while (ifs) {
      ifs.read(pagina.data(), tam);
      int leidos = ifs.gcount();
      if(leidos == 0){
         break;
      }else{
         v.append(pagina.begin(), pagina.begin() + leidos);
      }
   }
   v.push_back('\0');
}

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
   
   for(char c : archivo){
      std::cout << c;
   }
   std::cout << '\n';

}
