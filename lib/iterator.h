#ifndef LIB_ITERATOR_H
#define LIB_ITERATOR_H

#include <iterator>
#include <type_traits>
#include <utility>

namespace lib {
   template<typename T, typename II>
   concept has_insert = requires(T& v, II ini, II fin) {
      v.insert(v.end( ), ini, fin);
   };

   template<typename T, typename II>
   concept has_append = requires(T& v, II ini, II fin) {
      v.append(ini, fin);
   };

   template<typename T>
   class back_insert_iterator {
      T& contenedor;
   public:
      back_insert_iterator(T& c)
      : contenedor(c) {
      }

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


   template<typename T>
   class input_iterator {
      T& contenedor;
      std::size_t offset;
      mutable std::size_t tam_cache_;
   public:
      using value_type = typename T::value_type;

      input_iterator(T& c)
      : contenedor(c), offset(0), tam_cache_(0) {
      }

      const auto& operator*( ) const {
         return accesa(offset);
      }

      const auto& operator[](std::size_t i) const {
         return accesa(offset + i);
      }

      input_iterator& operator++( ) {
         ++offset;
         return *this;
      }

      input_iterator operator++(int) {
         auto temp = *this;
         return ++*this, temp;
      }

   private:
      const value_type& accesa(std::size_t i) const {
         while (i >= tam_cache_) {
            tam_cache_ = contenedor.size( );
         }
         return contenedor[i];
      }
   };

   template<typename T>
   input_iterator<T> make_input_iterator(T& v) {
      return input_iterator<T>{v};
   }
}

#endif
