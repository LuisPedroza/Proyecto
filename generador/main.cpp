#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "util.h"

std::string arregla_tab(std::string s) {
    s.pop_back();
    return s;
}

std::string genera(bool dentro, datos d, std::string tab, int if_dentro) {
    std::string var1, var2, var3, der;
    if (dentro) {
        int opcion = d.g.rand() % 6;
        if (opcion == 0) {
            var1 = d.genera_id_var(0);
            return fmt::format("{}number {} = {};\n{}", tab, var1, d.genera_int(), genera(true, d, tab, if_dentro));
        } else if (opcion == 1) {
            var1 = d.genera_id_var(0);
            if (d.v_num.size() < 2) {
                return fmt::format("{}number {} = {};\n{}", tab, var1, d.genera_int(), genera(true, d, tab, if_dentro));
            } else {                
                return fmt::format("{}number {} = {} {} {};\n{}", tab, var1, d.genera_var_num(var1), genera_op_num(), d.genera_var_num(var1), genera(true, d, tab, if_dentro));
            }
        } else if (opcion == 2) {
            var1 = d.genera_id_var(1);
            return fmt::format("{}array {} = {};\n{}", tab, var1, d.genera_arreglo(), genera(true, d, tab, if_dentro));
        } else if (opcion == 3) {
            var1 = d.genera_id_var(1);
            if (d.v_arr.size() < 2) {
                return fmt::format("{}array {} = {};\n{}", tab, var1, d.genera_arreglo(), genera(true, d, tab, if_dentro));
            } else {                
                int op_arr = d.g.rand() % 4;
                if (op_arr == 0) {
                    der = fmt::format("{}{}", d.obten_id(var1, 1), d.genera_slice());
                } else if (op_arr == 1) {
                    der = fmt::format("{} * {}", d.genera_arreglo(), d.genera_arreglo());
                } else if (op_arr == 2) {
                    der = fmt::format("{} * {}", d.obten_id(var1, 1), d.obten_id(var1, 1));
                } else {
                    der = fmt::format("{}{} * {}{}", d.obten_id(var1, 1), d.genera_slice(), d.obten_id(var1, 1), d.genera_slice());
                }
                return fmt::format("{}array {} = {};\n{}", tab, var1, der, genera(true, d, tab, if_dentro));
            }

        } else if (opcion == 4 && if_dentro < 3) {
            tab += "\t";
            return fmt::format("{}if {} {{\n{}\n{}}}", arregla_tab(tab), d.genera_condicion(), genera(true, d, tab, if_dentro + 1), arregla_tab(tab));
        } else {
            return fmt::format("{}return {};", tab, d.genera_id_var(0));
        }
    }
    var1 = d.genera_parametros();
    return fmt::format("function {}{}: {}{{\n{}\n}}\n", d.genera_id_func(), var1, d.genera_tipo(), genera(true, d, tab += "\t", if_dentro));
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    srand(time(0));

    datos d;

    int n;
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        d.v_num.clear();
        d.v_arr.clear();
        d.if_dentro = 0;
        std::string s = genera(false, d, "", 0);
        std::cout << s << '\n';
    }
}