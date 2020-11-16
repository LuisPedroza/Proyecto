#include "runtime.h"

runtime::array siguiente_fib(runtime::number n,runtime::number cont,runtime::array anteriores){
if((n==cont)){
return runtime::array({(anteriores(0)+anteriores(1))});
}
else{
runtime::number siguiente = (anteriores(0)+anteriores(1));
runtime::array aux = runtime::array({anteriores(1),siguiente});
return (runtime::array({siguiente})*siguiente_fib(n,(cont+1),aux));
}

}

runtime::array fibonacci(runtime::number n){
if((n==0)){
return runtime::array({0});
}
else{
if((n==1)){
return runtime::array({0,1});
}
else{
return (runtime::array({0,1})*siguiente_fib(n,2,runtime::array({0,1})));
}
}

}

int main(int argc, char *argv[]){
runtime::number n = runtime::read_number(argv[1]);
runtime::print(fibonacci(n));
return 0;

}

