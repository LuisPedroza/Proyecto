#include "runtime.h"

runtime::array xgcd(const runtime::number& a,const runtime::number& b){
if((a==0)){
return runtime::array({b,0,1});
}
runtime::array aux = xgcd(std::fmod(b,a),a);
runtime::number gcd = aux(0);
runtime::number x1 = aux(1);
runtime::number y1 = aux(2);
runtime::number x = (y1-(std::floor(b/a)*x1));
runtime::number y = x1;
return runtime::array({gcd,x,y});

}

int main(int argc, char *argv[]){
runtime::number a = runtime::read_number(argv[1]);
runtime::number b = runtime::read_number(argv[2]);
runtime::print(xgcd(a,b));
return 0;

}

