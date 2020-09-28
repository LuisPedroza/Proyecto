#ifndef UTIL_H
#define UTIL_H

#include<random>
#include<string>
#include"fmt/include/fmt/format.h"

class generador{
    std::random_device rd;
    std::mt19937 gen;     
    std::uniform_int_distribution<> num;        

    char genera_car(int op){
        if(op == 0){
            return char(65 + (num(gen) % 25));
        }else if(op == 1){
            return char(97 + (num(gen) % 25));
        }else if(op == 2){
            return char(48 + num(gen) % 10);
        }
        return '.';
    }

public:
    std::string genera_id_var(std::vector<std::string>& n){
        std::string aux = fmt::format("{}_{}{}",genera_car(0),genera_car(1),genera_car(2));
        n.push_back(aux);
        return aux;

    }

    std::string genera_id_func(){
        std::string s1 = "", s2 = "", s3 = "";
        for(int i = 0 ; i < 3 ; ++i){
            s1 += genera_car(0);
            s2 += genera_car(1);
            s3 += genera_car(2);
        }    
        return fmt::format("{}_{}_{}",s2,s1,s3);        
    }

    std::string genera_tipo(){
        return (num(gen) % 2 == 0 ? "number" : "array");
    }

    int genera_num_int(){
        return num(gen);
    }

    std::string genera_arreglo(){
        std::string s = "";
        int t = (num(gen) % 10) + 5;
        for(int i = 0 ; i < t ; ++i){
            s += std::to_string(num(gen));
            if(i == t - 1){
                continue;
            }else{                
                s += ",";
            }
        }
        return fmt::format("[{}]",s);
    }

    std::string genera_parametros(std::vector<std::string>& n, std::vector<std::string>& a, int& lim_n, int& lim_a){
        std::string s = "";
        int t = (num(gen) % 10) + 5;
        for(int i = 0 ; i < t ; ++i){
            auto aux = genera_tipo();
            if(aux[0] == 'n'){
                s += (aux + " " + genera_id_var(n));    
                lim_n += 1;
            }else{
                s += (aux + " " + genera_id_var(a)); 
                lim_a += 1;   
            }            
            if(i == t - 1){
                continue;
            }else{                
                s += ",";
            }
        }        
        return fmt::format("({})",s);
    }

    std::string genera_condicion(std::vector<std::string>& n, std::vector<std::string>& a, int lim_n, int lim_a){
        std::vector<std::string> v;
        v.push_back(std::to_string(num(gen)));
        if(lim_n != 0){
            v.push_back(n[num(gen) % lim_n]);
        }
        if(lim_a != 0){
            v.push_back(a[num(gen) % (lim_a)] + "(" + std::to_string(((num(gen) % 5) + 1)) + ")");
            v.push_back("#" + a[num(gen) % lim_a]);
        }
        int tam = v.size();        
        return fmt::format("{} {} {}",v[num(gen) % tam], genera_op_cond(), v[num(gen) % tam]);
    }

    std::string genera_slice(){
        return fmt::format("[0..{}]",num(gen) % 5 + 1);
    }

    std::string genera_op_cond(){
        switch(rand() % 6){
            case 0:
                return "=";
            case 1:
                return "!=";
            case 2:
                return ">";
            case 3:
                return "<";
            case 4:
                return ">=";
            case 5:
                return "<=";        
        }
        return "";
    }

    

    generador(){        
        gen = std::mt19937(rd());        
        num = std::uniform_int_distribution<>(0,1000000);        
    }

};

struct datos{
    generador gen;
    int if_dentro;
    std::vector<std::string> num;
    std::vector<std::string> arr;
};

std::string genera_op_num(){
    switch(rand() % 7){
        case 0:
            return "+";
        case 1:
            return "-";
        case 2:
            return "*";
        case 3:
            return "/";
        case 4:
            return "//";
        case 5:
            return "%";
        case 6:
            return "^";
    }
    return "";
}

#endif