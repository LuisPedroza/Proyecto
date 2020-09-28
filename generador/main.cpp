#include<iostream>
#include<vector>
#include<algorithm>
#include<utility>
#include<string>
#include<cstdlib>
#include<ctime>

#include"util.h"

std::string arregla_tab(std::string s){
    s.pop_back();    
    return s;
}

std::string genera(bool dentro, datos& d, int lim_n, int lim_a, std::string tab, int if_dentro){
    std::string var1, var2, der;
    if(dentro){
        int opcion = rand() % 7;
        if(opcion == 0){
            var1 = d.gen.genera_id_var(d.num);
            return fmt::format("{}number {} = {};\n{}", tab, var1, d.gen.genera_num_int(), genera(true,d,lim_n + 1,lim_a,tab,if_dentro));
        }else if(opcion == 1){
            var1 = d.gen.genera_id_var(d.num);
            if(lim_n < 2){                    
                return fmt::format("{}number {} = {};\n{}", tab, var1, d.gen.genera_num_int(), genera(true,d,lim_n + 1,lim_a,tab,if_dentro));
            }            
            var2 = (rand() % 2 == 0 ? d.num[rand() % lim_n] : std::to_string(rand() % 1000));                 
            return fmt::format("{}number {} = {} {} {};\n{}", tab, var1, d.num[rand() % lim_n], genera_op_num(), var2, genera(true,d,lim_n + 1,lim_a,tab,if_dentro));
        }else if(opcion == 2){
            var1 = d.gen.genera_id_var(d.arr);
            return fmt::format("{}array {} = {};\n{}", tab, var1, d.gen.genera_arreglo(),genera(true,d,lim_n,lim_a + 1,tab,if_dentro));
        }else if(opcion == 3){
            var1 = d.gen.genera_id_var(d.arr);
            if(d.arr.size() < 2){
                return fmt::format("{}array {} = {};\n{}", tab, var1, d.gen.genera_arreglo(),genera(true,d,lim_n,lim_a + 1,tab,if_dentro));
            }
            var2 = d.arr[rand() % lim_a];
            int op_arr = rand() % 4;
            if(op_arr == 0){
                der = fmt::format("{}{}",var2, d.gen.genera_slice());
            }else if(op_arr == 1){
                der = fmt::format("{} * {}",d.gen.genera_arreglo(),d.gen.genera_arreglo());
            }else if(op_arr == 2){
                der = fmt::format("{} * {}",d.arr[rand() % lim_a],d.arr[rand() % lim_a]);
            }else{
                der = fmt::format("{}{} * {}{}",d.arr[rand() % lim_a],d.gen.genera_slice(),d.arr[rand() % lim_a],d.gen.genera_slice());
            }
            return fmt::format("{}array {} = {};\n{}", tab, var1, der, genera(true,d,lim_n,lim_a+1,tab,if_dentro));
        }else if(opcion == 4 && if_dentro < 2){            
            tab += "\t";            
            return fmt::format("{}if {} {{\n{}\n{}}}", arregla_tab(tab), d.gen.genera_condicion(d.num, d.arr, lim_n, lim_a), genera(true,d,lim_n,lim_a,tab,if_dentro + 1), arregla_tab(tab));                        
        }else if(opcion == 5){
            //tab += "\t";            
            //return fmt::format("{}if {} {{\n{}\n{}}} else {{\n{}\n{}}}", arregla_tab(tab), d.gen.genera_condicion(d.num, d.arr, lim_n, lim_a), genera(true,d,lim_n,lim_a,tab,if_dentro + 1), arregla_tab(tab), genera(true,d,lim_n,lim_a,tab,if_dentro + 1), arregla_tab(tab));
            return fmt::format("{}number {} = {};\n{}", tab, var1, d.gen.genera_num_int(), genera(true,d,lim_n + 1,lim_a,tab,if_dentro));
        }else{
            return fmt::format("{}return {};", tab,d.gen.genera_id_var(d.num));
        }
    }
    var1 = d.gen.genera_parametros(d.num, d.arr, lim_n, lim_a);
    return fmt::format("function {}{}: {}{{\n{}\n}}\n",d.gen.genera_id_func(), var1, d.gen.genera_tipo(), genera(true,d,lim_n,lim_a,tab  += "\t",if_dentro));
}

int main(){

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr); std::cout.tie(nullptr);

    srand(time(0));

    datos d;    

    int n = 15000;         

    for(int i = 0 ; i < n ; ++i){
        d.num.clear();
        d.arr.clear();
        d.if_dentro = 0;
        std::string s = genera(false,d, 0, 0, "", 0);        
        std::cout << s << '\n';

    }

}