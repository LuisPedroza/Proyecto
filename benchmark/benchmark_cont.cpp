#include <iostream>
#include <chrono>
#include <deque>
#include "lib/concurrent_vector.h"
#include "tbb/concurrent_queue.h"
#include <cstring>
#include <fstream>
#include <iterator>
#include <vector>

class Timer{
public:
    Timer(){
        ini_t = std::chrono::high_resolution_clock::now();
    }

    ~Timer(){
        stop();
    }

    void stop(){
        std::chrono::time_point<std::chrono::high_resolution_clock> fin_t;
        fin_t = std::chrono::high_resolution_clock::now();
        auto ini = std::chrono::time_point_cast<std::chrono::microseconds>(ini_t).time_since_epoch().count();
        auto fin = std::chrono::time_point_cast<std::chrono::microseconds>(fin_t).time_since_epoch().count();
        auto tiempo = fin - ini;
        double seg = tiempo * 0.000001;
        std::cout << seg << '\n';
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> ini_t;
};

int main(int argc, char *argv[]) {

   int n = 10000000;

   lib::concurrent_vector<int> v_lib;
   tbb::concurrent_vector<int> v_tbb;
   std::vector<int> v_std;
   tbb::concurrent_queue<int> q_tbb;
   std::deque<int> d;


   {
      std::cout << "lib::concurrent_vector : \t";
      Timer t;
      for(int i = 0 ; i < n ; ++i){
         v_lib.push_back(i);
      }
   }

   {
      std::cout << "tbb::concurrent_vector : \t";
      Timer t;
      for(int i = 0 ; i < n ; ++i){
         v_tbb.push_back(i);
      }
   }

   {
      std::cout << "std::vector : \t\t\t";
      Timer t;
      for(int i = 0 ; i < n ; ++i){
         v_std.push_back(i);
      }
   }

   {
      std::cout << "tbb::concurrent_queue : \t";
      Timer t;
      for(int i = 0 ; i < n ; ++i){
         q_tbb.push(i);
      }
   }

   {
      std::cout << "std::deque : \t\t\t";
      Timer t;
      for(int i = 0 ; i < n ; ++i){
         d.push_back(i);
      }
   }



}
