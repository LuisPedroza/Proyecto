#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "util.h"

using map_f = std::unordered_map<std::string, std::vector<int>>;

std::string arregla_tab(std::string s) {
    s.pop_back();
    return s;
}

std::string genera(generador& g, bool dentro, datos d, std::string tab, int if_dentro) {
    std::string var1, var2, der;
    if (dentro) {
        int opcion = g.rand() % 6;
        if (opcion == 0) {
            var1 = d.genera_id_var(g, 0);
            return fmt::format("{}number {} = {};\n{}", tab, var1, g.rand(), genera(g, true, d, tab, if_dentro));
        } else if (opcion == 1) {
            var1 = d.genera_id_var(g, 0);
            if (d.v_num.size() < 4) {
                return fmt::format("{}number {} = {};\n{}", tab, var1, g.rand(), genera(g, true, d, tab, if_dentro));
            } else {
                return fmt::format("{}number {} = {} {} {};\n{}", tab, var1, d.genera_var_num(g, var1), genera_op_num(), d.genera_var_num(g, var1), genera(g, true, d, tab, if_dentro));
            }
        } else if (opcion == 2) {
            var1 = d.genera_id_var(g, 1);
            return fmt::format("{}array {} = {};\n{}", tab, var1, d.genera_arreglo(g), genera(g, true, d, tab, if_dentro));
        } else if (opcion == 3) {
            var1 = d.genera_id_var(g, 1);
            if (d.v_arr.size() < 4) {
                return fmt::format("{}array {} = {};\n{}", tab, var1, d.genera_arreglo(g), genera(g, true, d, tab, if_dentro));
            } else {
                int op_arr = g.rand() % 4;
                if (op_arr == 0) {
                    der = fmt::format("{}{}", d.obten_id(g, var1, 1), d.genera_slice(g));
                } else if (op_arr == 1) {
                    der = fmt::format("{} * {}", d.genera_arreglo(g), d.genera_arreglo(g));
                } else if (op_arr == 2) {
                    der = fmt::format("{} * {}", d.obten_id(g, var1, 1), d.obten_id(g, var1, 1));
                } else {
                    der = fmt::format("{}{} * {}{}", d.obten_id(g, var1, 1), d.genera_slice(g), d.obten_id(g, var1, 1), d.genera_slice(g));
                }
                return fmt::format("{}array {} = {};\n{}", tab, var1, der, genera(g, true, d, tab, if_dentro));
            }
        } else if (opcion == 4 && if_dentro < 2) {
            tab += "\t";
            return fmt::format("{}if {} {{\n{}\n{}}}\n{}", arregla_tab(tab), d.genera_condicion(g), genera(g, true, d, tab, if_dentro + 1), arregla_tab(tab), genera(g, true, d, arregla_tab(tab), if_dentro));
        } else {
            return fmt::format("{}return {};", tab, d.genera_id_var(g, 0));
        }
    }
    // std::string tipo = d.genera_tipo(g);
    // std::string nombre;
    // std::vector<int> v;
    // var1 = d.genera_parametros(g, v);
    // if(tipo[0] == 'n'){
    //     nombre = d.genera_id_func(g, f_n);
    //     f_n[nombre] = v;
    // }else{
    //     nombre = d.genera_id_func(g, f_a);
    //     f_a[nombre] = v;
    // }
    //return fmt::format("function {}{}: {}{{\n{}\n}}\n", nombre, var1, tipo, genera(g,true, d, tab += "\t", if_dentro));
    var1 = d.genera_parametros(g);
    return fmt::format("function {}{}: {}{{\n{}\n}}\n", d.genera_id_func(g), var1, d.genera_tipo(g), genera(g, true, d, tab += "\t", if_dentro));
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    srand(time(0));

    datos d;
    generador g;
    map_f f_n, f_a;

    int n;
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        d.v_num.clear();
        d.v_arr.clear();
        std::string s = genera(g, false, d, "", 0);
        std::cout << s << '\n';
    }
}