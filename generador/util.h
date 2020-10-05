#ifndef UTIL_H
#define UTIL_H

#include <random>
#include <string>
#include <unordered_map>
#include <iterator>

#include "fmt/include/fmt/format.h"

using map_f = std::unordered_map<std::string, std::vector<int>>;

struct funciones{
    map_f num;
    map_f arr;
};

class generador {
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> num;

   public:
    int rand() {
        return num(gen);
    }

    generador() {
        gen = std::mt19937(rd());
        num = std::uniform_int_distribution<>(0, 1000000);
    }
};

struct datos {        
    std::vector<std::string> v_num;
    std::vector<std::string> v_arr;

    std::string obten_id(generador& g, std::string s, int t) {
        int tam_n = v_num.size();
        int tam_a = v_arr.size();
        std::string id;
        while (true) {
            id = (t == 0 ? v_num[g.rand() % tam_n] : v_arr[g.rand() % tam_a]);
            if (id != s) {
                break;
            }
        }
        return id;
    }

    std::string genera_func(generador& g, map_f& m, std::string s){
        int n = g.rand() % m.size();
        auto ini = m.begin();
        std::advance(ini, n);
        std::string params = ini->first + "(";
        int tam = ini->second.size();        
        int cont = 0;
        for(int i : ini->second){
            params += obten_id(g, s, i);
            if(cont++ != tam - 1){
                params += ",";
            }
        }
        params += ")";
        return params;
    }


    char genera_car(generador& g, int op) {
        if (op == 0) {
            return char(65 + (g.rand() % 25));
        } else if (op == 1) {
            return char(97 + (g.rand() % 25));
        } else if (op == 2) {
            return char(48 + g.rand() % 10);
        }
        return '.';
    }

    std::string genera_id_var(generador& g, int t) {
        std::string aux = fmt::format("{}_{}{}", genera_car(g, 0), genera_car(g, 1), genera_car(g, 2));
        t == 0 ? v_num.push_back(aux) : v_arr.push_back(aux);
        return aux;
    }
    
    std::string genera_id_func(generador& g, funciones& f) {
        std::string s1 = "", s2 = "", s3 = "", id;
        while(true){
            for (int i = 0; i < 3; ++i) {
                s1 += genera_car(g,0);
                s2 += genera_car(g,1);
                s3 += genera_car(g,2);
            }
            id = fmt::format("{}_{}_{}", s2, s1, s3);
            if(f.arr.count(id) == 0 && f.num.count(id) == 0){
                break;
            }
        }                    
        return id;
    }

    std::string genera_var_num(generador& g, std::string s){
        return (g.rand() % 2 == 0 ? obten_id(g, s, 0) : std::to_string(g.rand()));
    }

    std::string genera_tipo(generador& g) {
        return (g.rand() % 2 == 0 ? "number" : "array");
    }    

    std::string genera_arreglo(generador& g) {
        std::string s = "";
        int t = (g.rand() % 10) + 5;
        for (int i = 0; i < t; ++i) {
            s += std::to_string(g.rand());
            if (i == t - 1) {
                continue;
            } else {
                s += ",";
            }
        }
        return fmt::format("[{}]", s);
    }

    std::string genera_parametros(generador& g, std::vector<int>& v) {
        std::string s = "";        
        int t = (g.rand() % 5) + 1;
        for (int i = 0; i < t; ++i) {
            auto aux = genera_tipo(g);
            if (aux[0] == 'n') {
                s += (aux + " " + genera_id_var(g, 0));
                v.push_back(0);
            } else {
                s += (aux + " " + genera_id_var(g, 1));                
                v.push_back(1);
            }
            if (i == t - 1) {
                continue;
            } else {
                s += ",";
            }
        }
        return fmt::format("({})", s);
    }

    std::string genera_rand_var(generador& g){
        std::vector<std::string> v;
        v.push_back(std::to_string(g.rand()));
        if(!v_num.empty()){
            v.push_back(obten_id(g, "", 0));
        }
        if (!v_arr.empty()) {
            v.push_back(obten_id(g, "", 1) + "(" + std::to_string((g.rand() % 5) + 1) + ")");
            v.push_back("#" + obten_id(g, "", 1));
        }
        int tam = v.size();
        return v[g.rand() % tam];
    }

    std::string genera_condicion(generador& g) {      
        int n = (g.rand() % 3) + 1;
        std::string cond = "";
        for(int i = 0 ; i < n ; ++i){
            cond += fmt::format("{} {} {}", genera_rand_var(g), genera_op_cond(), genera_rand_var(g));
            if(i != (n - 1)){
                cond += (g.rand() % 2 == 0 ? " & " : " | ");
            }
        }
        return cond;
    }

    std::string genera_slice(generador& g) {
        return fmt::format("[0..{}]", g.rand() % 5 + 1);
    }

   private:
    std::string genera_op_cond() {
        switch (rand() % 6) {
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
            default:
                return "";
        }
    }
};

std::string genera_op_num() {
    switch (rand() % 7) {
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
        default:
            return "";
    }
}

#endif