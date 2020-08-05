#ifndef LIB_ARCHIVO_H
#define LIB_ARCHIVO_H

#include <istream>

namespace lib {
   template<typename OI>
   void lee_archivo(std::istream& is, OI&& salida) {
      do {
         is.read(salida, 4096);
         salida += is.gcount();
      } while (is);
      *salida++ = '\0';
   }
}

#endif
