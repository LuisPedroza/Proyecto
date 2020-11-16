#include "runtime.h"

runtime::number max(runtime::number a,runtime::number b){
if((a>=b)){
return a;
}
else{
return b;
}

}

runtime::number mochila(runtime::number n,runtime::number w,runtime::array peso,runtime::array valor){
if(((n==-1)||(w==0))){
return 0;
}
else{
if((peso(n)>w)){
return mochila((n-1),w,peso,valor);
}
else{
runtime::number aux1 = mochila((n-1),w,peso,valor);
runtime::number aux2 = (valor(n)+mochila((n-1),(w-peso(n)),peso,valor));
return max(aux1,aux2);
}
}

}

int main(int argc, char *argv[]){
runtime::array peso = runtime::read_array(argv[1]);
runtime::array valor = runtime::read_array(argv[2]);
runtime::number w = runtime::read_number(argv[3]);
runtime::print(mochila((peso.size()-1),w,peso,valor));
return 0;

}

