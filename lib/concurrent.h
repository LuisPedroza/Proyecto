#ifndef LIB_CONCURRENT_H
#define LIB_CONCURRENT_H

#include <atomic>
#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <type_traits>

namespace lib {
   template<typename IT, typename T = std::iterator_traits<IT>::value_type>
   class concurrent_output_iterator {
      std::atomic<IT> iter;
   public:
      using value_type = T;
      using pointer = value_type*;
      using reference = value_type&;
      using difference_type = std::ptrdiff_t;
      using iterator_category = std::iterator_traits<IT>::iterator_category;

      concurrent_output_iterator(IT p)
      : iter(p) {
      }

      concurrent_output_iterator& operator++( ) {
         iter.store(iter.load(std::memory_order_relaxed) + 1, std::memory_order_release);
         return *this;
      }

      auto operator++(int) {
         struct postincrement_proxy {
            concurrent_output_iterator& oi;

            ~postincrement_proxy( ) {
               ++oi;
            }

            auto& operator*( ) {
               return *oi.iterator( ).load(std::memory_order_relaxed);
            }
         };
         return postincrement_proxy(*this);
      }

      concurrent_output_iterator& operator+=(std::ptrdiff_t i) {
         iter.store(iter.load(std::memory_order_relaxed) + i, std::memory_order_release);
         return *this;
      }

      const std::atomic<IT>& iterator( ) const {
         return iter;
      }

      explicit operator pointer( ) {
         static_assert(std::is_trivial_v<T>);
         return &static_cast<T&>(*iter.load(std::memory_order_relaxed));
      }
   };

   template<typename IT, typename T = std::iterator_traits<IT>::value_type>
   class concurrent_inspect_iterator {
      IT iter;
      std::reference_wrapper<const concurrent_output_iterator<IT, T>> fin;
   public:
      using value_type = const T;
      using pointer = const value_type*;
      using reference = const value_type&;
      using difference_type = std::ptrdiff_t;
      using iterator_category = std::forward_iterator_tag;

      concurrent_inspect_iterator(IT i, const concurrent_output_iterator<IT, T>& f)
      : iter(i), fin(f) {
      }

      value_type& operator*( ) const {
         while (iter >= fin.get( ).iterator( ).load(std::memory_order_acquire)) {
            continue;
         }
         return static_cast<reference>(*iter);
      }

      pointer operator->( ) const {
         return &operator*( );
      }

      concurrent_inspect_iterator& operator++( ) {
         ++iter;
         return *this;
      }

      concurrent_inspect_iterator operator++(int) {
         auto temp = *this;
         operator++( );
         return temp;
      }

      operator pointer( ) const {
         static_assert(std::is_trivial_v<T>);
         return &static_cast<reference>(*iter);
      }
   };

   template<typename T>
   class concurrent_buffer {
      struct raw_storage {
         alignas(T) char mem[sizeof(T)];

         template<typename U>
         void operator=(U&& v) {
            new(this) T(std::forward<U>(v));
         }

         explicit operator T&( ) {
            return reinterpret_cast<T&>(*this);
         }

         explicit operator const T&( ) const {
            return reinterpret_cast<const T&>(*this);
         }
      };

      std::unique_ptr<raw_storage[]> mem;
      concurrent_output_iterator<raw_storage*, T> iter_fin;
      std::size_t cap;

   public:
      using value_type = T;
      using iterator = T*;
      using const_iterator = T*;

      concurrent_buffer(std::size_t n)
      : mem(new raw_storage[n]), iter_fin(mem.get( )), cap(n) {
      }

      ~concurrent_buffer( ) {
         std::destroy(begin( ), end( ));
      }

      std::size_t size( ) const {
         return iter_fin.iterator( ).load(std::memory_order_acquire) - mem.get( );
      }

      std::size_t capacity( ) const {
         return cap;
      }

      iterator begin( ) {
         return reinterpret_cast<T*>(mem.get( ));
      }

      iterator end( ) {
         return begin( ) + size( );
      }

      const_iterator begin( ) const {
         return reinterpret_cast<const T*>(mem.get( ));
      }

      const_iterator end( ) const {
         return begin( ) + size( );
      }

      auto& output_iterator( ) {
         return iter_fin;
      }

      auto inspect_iterator( ) const {
         return concurrent_inspect_iterator<raw_storage*, T>(mem.get( ), iter_fin);
      }
   };
}

#endif
