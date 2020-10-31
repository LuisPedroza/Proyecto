#ifndef LIB_ARCHIVO_H
#define LIB_ARCHIVO_H

#include <istream>

namespace lib {
   constexpr std::size_t read_size = 4096;

   template<typename OI>
   void lee_archivo(std::istream& is, OI&& salida) {
      do {
         is.read((char*)salida, read_size);
         salida += is.gcount( );
      } while (is);
      *salida++ = '\0';
   }
}

#endif
