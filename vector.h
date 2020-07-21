#ifndef VECTOR_H
#define VECTOR_H

#include <atomic>
#include <iterator>
#include <vector>
#include <tbb/concurrent_vector.h>

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

   template<typename II>
   void append(II ini, II fin) {
      tbb::concurrent_vector<T>::grow_by(ini, fin);
      tam = tbb::concurrent_vector<T>::size( );
   }

   void clear( ) {
      tbb::concurrent_vector<T>::clear( );
      tam = tbb::concurrent_vector<T>::size( );
   }

   T& operator[](std::size_t i) {
      return begin( )[i];
   }

   const T& operator[](std::size_t i) const {
      return begin( )[i];
   }

   T& front( ) {
      return *begin( );
   }

   const T& front( ) const {
      return *begin( );
   }

   T& back( ) {
      return *std::prev(end( ));
   }

   const T& back( ) const {
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

   const_iterator cbegin( ) const {
      return tbb::concurrent_vector<T>::cbegin( );
   }

   const_iterator cend( ) const {
      return tbb::concurrent_vector<T>::cbegin( ) + size( );
   }
};

template<typename T, typename II>
void append(std::vector<T>& v, II ini, II fin) {
   v.insert(v.end( ), ini, fin);
}

template<typename T, typename II>
void append(concurrent_vector<T>& v, II ini, II fin) {
   v.append(ini, fin);
}

#endif
