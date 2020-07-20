#include "vector.h"
#include <iostream>

int main( ) {
   concurrent_vector<int> v;
   v.push_back(7);
   std::cout << v.size( );
}
