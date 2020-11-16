#include "runtime.h"

runtime::array merge(runtime::array a,runtime::array b){
if((a.size()==0)){
return b;
}
else{
if((b.size()==0)){
return a;
}
else{
if((a(0)<b(0))){
return (runtime::array({a(0)})*merge(a.slice(1, a.size()),b));
}
else{
return (runtime::array({b(0)})*merge(a,b.slice(1, b.size())));
}
}
}

}

runtime::array mergesort(runtime::array a){
if((a.size()<=1)){
return a;
}
else{
runtime::number m = std::floor(a.size()/2);
return merge(mergesort(a.slice(0, m)),mergesort(a.slice(m, a.size())));
}

}

int main(int argc, char *argv[]){
runtime::array a = runtime::array({2,5,1,3,7,5,6,9,7,5});
runtime::print(mergesort(a));
return 0;

}

