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

std::string genera(int dentro, datos d, std::string tab, int if_dentro, funciones& f, std::string nombre) {
    std::string aux1, aux2;
    if (dentro != 0 && dentro != 3) {
        int opcion = d.g.rand() % 4;
        if (opcion == 0) {
            std::string var_n = d.genera_id_var(0);
            if (d.v_num.size() < 4) {
                return fmt::format("{}number {} = {};\n{}", tab, var_n, d.genera_var_num(f.num, nombre), genera(dentro, d, tab, if_dentro, f, nombre));
            } else {
                return fmt::format("{}number {} = {} {} {};\n{}", tab, var_n, d.genera_var(var_n, f.num, nombre, 0), d.genera_op_num(), d.genera_var(var_n, f.num, nombre, 0), genera(dentro, d, tab, if_dentro, f, nombre));
            }
        } else if (opcion == 1) {
            std::string var_a = d.genera_id_var(1), der;
            if (d.v_arr.size() < 4) {
                return fmt::format("{}array {} = {};\n{}", tab, var_a, d.genera_var_arr(f.arr, nombre), genera(dentro, d, tab, if_dentro, f, nombre));
            } else {
                int op_arr = d.g.rand() % 4;
                if (op_arr == 0) {
                    der = fmt::format("{}{}", d.genera_var(var_a, f.arr, nombre, 1), d.genera_slice());
                } else if (op_arr == 1) {
                    der = fmt::format("{} * {}", d.genera_arreglo(), d.genera_arreglo());
                } else if (op_arr == 2) {
                    der = fmt::format("{} * {}", d.genera_var(var_a, f.arr, nombre, 1), d.genera_var(var_a, f.arr, nombre, 1));
                } else {
                    der = fmt::format("{}{} * {}{}", d.obten_id(var_a, 1), d.genera_slice(), d.obten_id(var_a, 1), d.genera_slice());
                }
                return fmt::format("{}array {} = {};\n{}", tab, var_a, der, genera(dentro, d, tab, if_dentro, f, nombre));
            }
        } else if (opcion == 2 && if_dentro < 1) {
            tab += "\t";
            int op_if = d.g.rand() % 3;
            if (op_if == 0) {
                return fmt::format("{}if {} {{\n{}\n{}}}\n{}", arregla_tab(tab), d.genera_condicion(), genera(dentro, d, tab, if_dentro + 1, f, nombre), arregla_tab(tab), genera(dentro, d, arregla_tab(tab), if_dentro, f, nombre));
            } else if (op_if == 1) {
                return fmt::format("{}if {} {{\n{}\n{}}} else {{\n{}\n{}}}\n{}", arregla_tab(tab), d.genera_condicion(), genera(dentro, d, tab, if_dentro + 1, f, nombre), arregla_tab(tab), genera(dentro, d, tab, if_dentro + 1, f, nombre), arregla_tab(tab), genera(dentro, d, arregla_tab(tab), if_dentro, f, nombre));
            } else {
                return fmt::format("{}if {} {{\n{}\n{}}} else if {} {{\n{}\n{}}} else {{\n{}\n{}}}\n{}", arregla_tab(tab), d.genera_condicion(), genera(dentro, d, tab, if_dentro + 1, f, nombre), arregla_tab(tab), d.genera_condicion(), genera(dentro, d, tab, if_dentro + 1, f, nombre), arregla_tab(tab), genera(dentro, d, tab, if_dentro + 1, f, nombre), arregla_tab(tab), genera(dentro, d, arregla_tab(tab), if_dentro, f, nombre));
            }
        } else {
            int op = d.g.rand() % 2, t = dentro - 1;
            return fmt::format("{}return {};", tab, d.obten_id("", t));
        }
    }
    nombre = (dentro == 3 ? "main" : d.genera_id_func(f));
    std::string tipo = d.genera_tipo();
    std::vector<int> v;
    std::string param = d.genera_parametros(v);
    if (tipo[0] == 'n') {
        f.num[nombre] = v;
    } else {
        f.arr[nombre] = v;
    }
    return fmt::format("function {}{}: {}{{\n{}\n}}\n", nombre, param, tipo, genera((tipo[0] == 'n' ? 1 : 2), d, tab += "\t", if_dentro, f, nombre));
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    srand(time(0));

    generador g;
    datos d = {g};
    funciones f;

    int n;
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        d.v_num.clear();
        d.v_arr.clear();
        std::cout << genera(0, d, "", 0, f, "") << '\n';        
    }

    std::cout << genera(3, d, "", 0, f, "") << '\n';


}