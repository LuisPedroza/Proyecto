#ifndef ARCHIVO_H
#define ARCHIVO_H

#include "vector.h"
#include <fstream>
#include <string_view>

template<typename OI>
void lee_archivo(std::ifstream& ifs, OI salida){
   constexpr int tam = 4096;
   char pagina[tam];
   while (ifs) {
      ifs.read(pagina, tam);
      int leidos = ifs.gcount();
      if(leidos == 0){
         break;
      }else{
         *salida++ = std::string_view(pagina, leidos);
      }
   }
   *salida++ = '\0';
}

#endif
