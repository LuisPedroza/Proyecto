#ifndef LIB_ITERATOR_H
#define LIB_ITERATOR_H

#include <iterator>
#include <type_traits>
#include <utility>

namespace lib {
   template<typename T, typename II>
   concept has_insert = requires(T& v, II ini, II fin) {
      v.insert(v.end( ), v.begin( ), v.end( ));
   };

   template<typename T, typename II>
   concept has_append = requires(T& v, II ini, II fin) {
      v.append(ini, fin);
   };

   template<typename T>
   struct back_insert_iterator {
      T& contenedor;

      back_insert_iterator& operator*( ) {
         return *this;
      }

      back_insert_iterator& operator++( ) {
         return *this;
      }

      back_insert_iterator& operator++(int) {
         return *this;
      }

      back_insert_iterator& operator=(typename T::value_type v) {
         contenedor.push_back(std::move(v));
         return *this;
      }

      template<typename II> requires std::is_same_v<typename std::iterator_traits<II>::value_type, typename T::value_type>
      back_insert_iterator& operator=(typename std::pair<II, II> rango) {
         if constexpr(has_insert<T, II>) {
            contenedor.insert(contenedor.end( ), rango.first, rango.second);
         } else if constexpr(has_append<T, II>) {
            contenedor.append(rango.first, rango.second);
         } else {
            while (rango.first != rango.second) {
               contenedor.push_back(*rango.first++);
            }
         }
         return *this;
      }
   };

   template<typename T>
   back_insert_iterator<T> back_inserter(T& v) {
      return back_insert_iterator<T>{v};
   }
}

#endif
