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

std::string genera(bool dentro, datos& d, int lim_a, std::string tab, int if_dentro) {
    std::string var1, var2, var3, der;
    if (dentro) {
        int opcion = rand() % 6;
        if (opcion == 0) {
            var1 = d.genera_id_var(0);
            return fmt::format("{}number {} = {};\n{}", tab, var1, d.genera_num_int(), genera(true, d, lim_a, tab, if_dentro));
        } else if (opcion == 1) {
            var1 = d.genera_id_var(0);
            if (d.v_num.size() < 2) {
                return fmt::format("{}number {} = {};\n{}", tab, var1, d.genera_num_int(), genera(true, d, lim_a, tab, if_dentro));
            } else {
                var2 = (rand() % 2 == 0 ? d.obten_id_num(var1) : std::to_string(rand() % 1000));
                var3 = (rand() % 2 == 0 ? d.obten_id_num(var1) : std::to_string(rand() % 1000));
                return fmt::format("{}number {} = {} {} {};\n{}", tab, var1, var2, genera_op_num(), var3, genera(true, d, lim_a, tab, if_dentro));
            }
        } else if (opcion == 2) {
            var1 = d.genera_id_var(1);
            return fmt::format("{}array {} = {};\n{}", tab, var1, d.genera_arreglo(), genera(true, d, lim_a + 1, tab, if_dentro));
        } else if (opcion == 3) {
            var1 = d.genera_id_var(1);
            if (d.v_arr.size() < 2) {
                return fmt::format("{}array {} = {};\n{}", tab, var1, d.genera_arreglo(), genera(true, d, lim_a + 1, tab, if_dentro));
            } else {
                var2 = d.v_arr[rand() % lim_a];
                int op_arr = rand() % 4;
                if (op_arr == 0) {
                    der = fmt::format("{}{}", var2, d.genera_slice());
                } else if (op_arr == 1) {
                    der = fmt::format("{} * {}", d.genera_arreglo(), d.genera_arreglo());
                } else if (op_arr == 2) {
                    der = fmt::format("{} * {}", d.v_arr[rand() % lim_a], d.v_arr[rand() % lim_a]);
                } else {
                    der = fmt::format("{}{} * {}{}", d.v_arr[rand() % lim_a], d.genera_slice(), d.v_arr[rand() % lim_a], d.genera_slice());
                }
                return fmt::format("{}array {} = {};\n{}", tab, var1, der, genera(true, d, lim_a + 1, tab, if_dentro));
            }

        } else if (opcion == 4 && if_dentro < 2) {
            tab += "\t";
            return fmt::format("{}if {} {{\n{}\n{}}}", arregla_tab(tab), d.genera_condicion(lim_a), genera(true, d, lim_a, tab, if_dentro + 1), arregla_tab(tab));
        } else {
            return fmt::format("{}return {};", tab, d.genera_id_var(0));
        }
    }
    var1 = d.genera_parametros(lim_a);
    return fmt::format("function {}{}: {}{{\n{}\n}}\n", d.genera_id_func(), var1, d.genera_tipo(), genera(true, d, lim_a, tab += "\t", if_dentro));
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    srand(time(0));

    datos d;

    int n = 15000;

    for (int i = 0; i < n; ++i) {
        d.v_num.clear();
        d.v_arr.clear();
        d.if_dentro = 0;
        std::string s = genera(false, d, 0, "", 0);
        std::cout << s << '\n';
    }
}