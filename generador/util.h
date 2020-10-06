#ifndef UTIL_H
#define UTIL_H

#include <iterator>
#include <random>
#include <string>
#include <unordered_map>
#include <ctime>

#include <fmt/format.h>  

using map_f = std::unordered_map<std::string, std::vector<int>>;

struct funciones {
    map_f num;
    map_f arr;
};

class generador {    
    std::mt19937 gen;
    std::uniform_int_distribution<> num;

   public:
    int rand() {
        return num(gen);
    }

    generador() {
        gen = std::mt19937(std::time(nullptr));
        num = std::uniform_int_distribution<>(0, 1000000);
    }
};

struct datos {
    generador& g;
    std::vector<std::string> v_num;
    std::vector<std::string> v_arr;
    
    std::string obten_id(const std::string& s, int t) {
        int tam_n = v_num.size();
        int tam_a = v_arr.size();
        std::string id;
        while (true) {
            if (t == 0) {
                id = (tam_n != 0 ? v_num[g.rand() % tam_n] : std::to_string(g.rand()));
            } else {
                id = (tam_a != 0 ? v_arr[g.rand() % tam_a] : genera_arreglo());
            }
            if (id != s) {
                break;
            }
        }
        return id;
    }

    std::string llama_func(const map_f& m, const std::string& s, const std::string& nombre) {
        map_f::const_iterator ini;
        while (true) {
            int n = g.rand() % m.size();
            ini = m.begin();
            std::advance(ini, n);
            if (ini->first != nombre) {
                break;
            }
        }
        std::string params = ini->first + "(";
        int tam = ini->second.size();
        int cont = 0;
        for (int i : ini->second) {
            params += obten_id(s, i);
            if (cont++ != tam - 1) {
                params += ",";
            }
        }
        params += ")";
        return params;
    }

    char genera_car(int op) {
        if (op == 0) {
            return char(65 + (g.rand() % 25));
        } else if (op == 1) {
            return char(97 + (g.rand() % 25));
        } else if (op == 2) {
            return char(48 + g.rand() % 10);
        }
        return '.';
    }

    std::string genera_id_var(int t) {
        std::string aux = fmt::format("{}_{}{}", genera_car(0), genera_car(1), genera_car(2));
        t == 0 ? v_num.push_back(aux) : v_arr.push_back(aux);
        return aux;
    }

    std::string genera_id_func(funciones& f) {
        std::string s1 = "", s2 = "", s3 = "", id;
        while (true) {
            for (int i = 0; i < 3; ++i) {
                s1 += genera_car(0);
                s2 += genera_car(1);
                s3 += genera_car(2);
            }
            id = fmt::format("{}_{}_{}", s2, s1, s3);
            if (f.arr.count(id) == 0 && f.num.count(id) == 0) {
                break;
            }
        }
        return id;
    }

    std::string genera_var_num(const map_f& f, const std::string& nombre) {
        return (f.size() < 3 ? std::to_string(g.rand()) : (g.rand() % 2 == 0 ? llama_func(f, "", nombre) : std::to_string(g.rand())));
    }    

    std::string genera_var_arr(const map_f& f, const std::string& nombre) {
        return (f.size() < 3 ? genera_arreglo() : (g.rand() % 2 == 0 ? llama_func(f, "", nombre) : genera_arreglo()));
    }

    std::string genera_var(const std::string& v, const map_f& f, const std::string& nombre, int t) {
        if(f.size() < 3){
            return obten_id(v, t);
        }else{
            int op = g.rand() %3;
            if(op == 0){
                return llama_func(f, v, nombre);
            }else if(op == 1){
                return obten_id(v, t);
            }else{
                return t == 0 ? std::to_string(g.rand()) : genera_arreglo();
            }
        }                     
    }    

    std::string genera_tipo() {
        return (g.rand() % 2 == 0 ? "number" : "array");
    }

    std::string genera_arreglo() {
        std::string s = "";
        int t = (g.rand() % 5) + 5;
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

    std::string genera_parametros(std::vector<int>& v) {
        std::string s = "";
        int t = (g.rand() % 5) + 1;
        for (int i = 0; i < t; ++i) {
            auto aux = genera_tipo();
            if (aux[0] == 'n') {
                s += (aux + " " + genera_id_var(0));
                v.push_back(0);
            } else {
                s += (aux + " " + genera_id_var(1));
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

    std::string genera_rand_var() {
        std::vector<std::string> v;
        v.push_back(std::to_string(g.rand()));
        if (!v_num.empty()) {
            v.push_back(obten_id("", 0));
        }
        if (!v_arr.empty()) {
            v.push_back(obten_id("", 1) + "(" + std::to_string((g.rand() % 5) + 1) + ")");
            v.push_back("#" + obten_id("", 1));
        }
        int tam = v.size();
        return v[g.rand() % tam];
    }

    std::string genera_condicion() {
        int n = (g.rand() % 3) + 1;
        std::string cond = "";
        for (int i = 0; i < n; ++i) {
            cond += fmt::format("{} {} {}", genera_rand_var(), genera_op_cond(), genera_rand_var());
            if (i != (n - 1)) {
                cond += (g.rand() % 2 == 0 ? " & " : " | ");
            }
        }
        return cond;
    }

    std::string genera_slice() {
        return fmt::format("[0..{}]", g.rand() % 5 + 1);
    }

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

};

#endif