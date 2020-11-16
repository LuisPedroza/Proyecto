#include "runtime.h"

runtime::number revisa_divisores(runtime::number n,runtime::number d){
if((d==1)){
return 1;
}
if((std::fmod(n,d)==0)){
return 0;
}
return revisa_divisores(n,(d-1));

}

runtime::number es_primo(runtime::number n){
return revisa_divisores(n,(n-1));

}

int main(int argc, char *argv[]){
runtime::number n = runtime::read_number(argv[1]);
runtime::print(es_primo(n));
return 0;

}

