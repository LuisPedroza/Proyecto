#include <fstream>
int main( ) {
   std::ofstream ofs("temp.txt");
   for (int i = 0; i < 25 * 1024 * 1024; ++i) {
      ofs << "@";
   }
}