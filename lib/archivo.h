#ifndef ARCHIVO_H
#define ARCHIVO_H

#include "vector.h"
#include <fstream>

void lee_archivo(concurrent_vector<char>& v, std::ifstream& ifs){
   int tam = 4096;
   char pagina[tam];
   while (ifs) {
      ifs.read(pagina, tam);
      int leidos = ifs.gcount();
      if(leidos == 0){
         break;
      }else{
         v.append(pagina, pagina + leidos);
      }
   }
   v.push_back('\0');
}

#endif