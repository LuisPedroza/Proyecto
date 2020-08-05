#ifndef LIB_CONCURRENT_H
#define LIB_CONCURRENT_H

#include <tbb/concurrent_vector.h>
#include <atomic>
#include <iterator>
#include <memory>
#include <utility>

namespace lib {
   template<typename T>
   class concurrent_buffer {
      std::unique_ptr<T[]> mem;
      std::atomic<T*> iter_fin;
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

      value_type& operator[](std::size_t i) {
         return begin( )[i];
      }

      const value_type& operator[](std::size_t i) const {
         return begin( )[i];
      }

      value_type& front( ) {
         return *begin( );
      }

      const value_type& front( ) const {
         return *begin( );
      }

      value_type& back( ) {
         return *std::prev(end( ));
      }

      const value_type& back( ) const {
         return *std::prev(end( ));
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

      std::atomic<iterator>& resizable_end( ) {
         return iter_fin;
      }

      const std::atomic<iterator>& concurrent_end( ) const {
         return iter_fin;
      }
   };

   template<typename T>
   class concurrent_vector : private tbb::concurrent_vector<T> {
      std::atomic<std::size_t> tam;
   public:
      using value_type = T;
      using iterator = tbb::concurrent_vector<T>::iterator;
      using const_iterator = tbb::concurrent_vector<T>::const_iterator;

      concurrent_vector( )
      : tam(tbb::concurrent_vector<T>::size( )) {
      }

      concurrent_vector(concurrent_vector&& v)
      : tbb::concurrent_vector<T>(std::move(v)), tam(tbb::concurrent_vector<T>::size( )) {
      }

      concurrent_vector(const concurrent_vector& v)
      : tbb::concurrent_vector<T>(v), tam(tbb::concurrent_vector<T>::size( )) {
      }

      concurrent_vector& operator=(concurrent_vector&& v) {
         if (this != &v) {
            tbb::concurrent_vector<T>::operator=(std::move(v));
            tam = tbb::concurrent_vector<T>::size( );
         }
         return *this;
      }

      concurrent_vector& operator=(const concurrent_vector& v) {
         if (this != &v) {
            tbb::concurrent_vector<T>::operator=(v);
            tam = tbb::concurrent_vector<T>::size( );
         }
         return *this;
      }

      std::size_t size( ) const {
         return tam;
      }

      void push_back(T v) {
         tbb::concurrent_vector<T>::push_back(std::move(v));
         tam = tbb::concurrent_vector<T>::size( );
      }

      value_type& operator[](std::size_t i) {
         return begin( )[i];
      }

      const value_type& operator[](std::size_t i) const {
         return begin( )[i];
      }

      value_type& front( ) {
         return *begin( );
      }

      const value_type& front( ) const {
         return *begin( );
      }

      value_type& back( ) {
         return *std::prev(end( ));
      }

      const value_type& back( ) const {
         return *std::prev(end( ));
      }

      iterator begin( ) {
         return tbb::concurrent_vector<T>::begin( );
      }

      iterator end( ) {
         return tbb::concurrent_vector<T>::begin( ) + size( );
      }

      const_iterator begin( ) const {
         return tbb::concurrent_vector<T>::begin( );
      }

      const_iterator end( ) const {
         return tbb::concurrent_vector<T>::begin( ) + size( );
      }
   };

   template<typename T>
   class concurrent_inspect_iterator {
      typename T::const_iterator iter;
      const std::atomic<typename T::iterator>* fin;
   public:
      using value_type = const T::value_type;
      using pointer = const value_type*;
      using reference = const value_type&;
      using difference_type = std::ptrdiff_t;
      using iterator_category = std::forward_iterator_tag;

      concurrent_inspect_iterator(const T& c)
      : iter(c.begin( )), fin(&c.concurrent_end( )) {
      }

      value_type& operator*( ) const {
         while (iter >= *fin) {
            continue;
         }
         return *iter;
      }

      pointer operator->( ) const {
         return &**this;
      }

      concurrent_inspect_iterator& operator++( ) {
         ++iter;
         return *this;
      }

      concurrent_inspect_iterator operator++(int) {
         auto temp = *this;
         ++*this;
         return temp;
      }

      operator typename T::const_iterator( ) const {
         return iter;
      }
   };
}

#endif
