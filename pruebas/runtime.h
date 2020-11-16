#ifndef LIB_RUNTIME_H
#define LIB_RUNTIME_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <string>

namespace runtime {
   using number = long double;

   struct array {
      std::vector<number> mem;

      array( ) = default;

      array(std::initializer_list<number> li)
      : mem(li) {
      }

      array(std::vector<number>&& v)
      : mem(std::move(v)) {
      }

      number operator()(number i) const {
         return mem[i];
      }

      array operator[](number i) const {
         return slice(i, i + 1);
      }

      array operator*(const array& a) const {
         std::vector<number> aux = mem;
         aux.insert(aux.end( ), a.mem.begin( ), a.mem.end( ));
         return std::move(aux);
      }

      number size( ) const {
         return mem.size( );
      }

      array slice(number ini, number fin) const {
         return std::vector<number>(mem.begin( ) + ini, mem.begin( ) + fin);
      }
   };

   number read_number(const char* arg) {
      return std::strtold(arg, nullptr);
   }

   void print(number n) {
      std::cout << n << "\n";
   }

   array read_array(const char* arg) {
      array a;
      std::string s = arg;
      if(s != "[]"){
         std::stringstream ss(s.substr(1, s.size( ) - 2));
         char coma;
         while(ss.good( )){
            number temp;
            ss >> temp;
            a.mem.push_back(temp);
            ss >> coma;
         }
      }
      return a;
   }

   void print(const array& a) {
      std::cout << "[ ";
      for(int i = 0 ; i < a.size( ); ++i){
         std::cout << a(i) << (i == a.size( ) - 1 ? "" : ", ");
      }
      std::cout << "]\n";
   }
   
}

#endif
