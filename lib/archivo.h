#ifndef LIB_ARCHIVO_H
#define LIB_ARCHIVO_H

#include <istream>

namespace lib {
   template<typename OI>
   void lee_archivo(std::istream& ifs, OI salida){
      constexpr int tam = 4096;
      char pagina[tam];
      while (ifs) {
         ifs.read(pagina, tam);
         int leidos = ifs.gcount();
         if(leidos == 0){
            break;
         }else{
            *salida++ = std::make_pair(pagina, pagina + leidos);
         }
      }
      *salida++ = '\0';
   }
}

#endif
