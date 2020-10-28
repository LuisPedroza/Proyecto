#ifndef LIB_CONCURRENT_H
#define LIB_CONCURRENT_H

#include <atomic>
#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>

namespace lib {
   template<typename IT>
   class concurrent_output_iterator {
      std::atomic<IT> iter;
   public:
      using value_type = std::iterator_traits<IT>::value_type;
      using pointer = value_type*;
      using reference = value_type&;
      using difference_type = std::ptrdiff_t;
      using iterator_category = std::iterator_traits<IT>::iterator_category;

      concurrent_output_iterator(IT p)
      : iter(p) {
      }

      value_type& operator*( ) {
         return *iter.load(std::memory_order_relaxed);
      }

      const value_type& operator*( ) const {
         return *iter.load(std::memory_order_relaxed);
      }

      concurrent_output_iterator& operator++( ) {
         iter.store(iter.load(std::memory_order_relaxed) + 1, std::memory_order_release);
         return *this;
      }

      auto operator++(int) {
         struct postincrement_proxy {
            concurrent_output_iterator& actual;
         public:
            ~postincrement_proxy( ) {
               ++actual;
            }

            value_type& operator*( ) {
               return *actual;
            }
         };
         return postincrement_proxy(*this);
      }

      concurrent_output_iterator& operator+=(std::ptrdiff_t i) {
         iter.store(iter.load(std::memory_order_relaxed) + i, std::memory_order_release);
         return *this;
      }

      const std::atomic<IT>& atomic_iterator( ) const {
         return iter;
      }

      operator IT( ) const {
         return iter.load(std::memory_order_relaxed);
      }
   };

   template<typename IT>
   class concurrent_inspect_iterator {
      IT iter;
      std::reference_wrapper<const concurrent_output_iterator<IT>> fin;
   public:
      using value_type = const std::iterator_traits<IT>::value_type;
      using pointer = const value_type*;
      using reference = const value_type&;
      using difference_type = std::ptrdiff_t;
      using iterator_category = std::forward_iterator_tag;

      concurrent_inspect_iterator(IT i, const concurrent_output_iterator<IT>& f)
      : iter(i), fin(f) {
      }

      value_type& operator*( ) const {
         while (iter >= fin.get( ).atomic_iterator( ).load(std::memory_order_acquire)) {
            continue;
         }
         return *iter;
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

      operator IT( ) const {
         return iter;
      }
   };

   template<typename T>
   class concurrent_buffer {
      std::unique_ptr<T[]> mem;
      concurrent_output_iterator<T*> iter_fin;
      std::size_t cap;
   public:
      using value_type = T;
      using iterator = T*;
      using const_iterator = T*;

      concurrent_buffer(std::size_t n)
      : mem(new T[n]), iter_fin(mem.get( )), cap(n) {
      }

      std::size_t size( ) const {
         return iter_fin - mem.get( );
      }

      std::size_t capacity( ) const {
         return cap;
      }

      iterator begin( ) {
         return mem.get( );
      }

      iterator end( ) {
         return mem.get( ) + size( );
      }

      const_iterator begin( ) const {
         return mem.get( );
      }

      const_iterator end( ) const {
         return mem.get( ) + size( );
      }

      auto& output_iterator( ) {
         return iter_fin;
      }

      auto inspect_iterator( ) const {
         return concurrent_inspect_iterator<T*>(begin( ), iter_fin);
      }
   };
}

#endif
