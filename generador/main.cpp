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

std::string genera(generador& g, int dentro, datos d, std::string tab, int if_dentro, funciones& f, std::string nombre) {
    std::string var1, var2, der;
    if (dentro != 0) {
        int opcion = g.rand() % 10;
        if (opcion == 0) {
            var1 = d.genera_id_var(g, 0);
            return fmt::format("{}number {} = {};\n{}", tab, var1, g.rand(), genera(g, dentro, d, tab, if_dentro, f, nombre));
        } else if (opcion == 1) {
            var1 = d.genera_id_var(g, 0);
            if (d.v_num.size() < 4) {
                return fmt::format("{}number {} = {};\n{}", tab, var1, g.rand(), genera(g, dentro, d, tab, if_dentro, f, nombre));
            } else {
                return fmt::format("{}number {} = {} {} {};\n{}", tab, var1, d.genera_var_num(g, var1), genera_op_num(), d.genera_var_num(g, var1), genera(g, dentro, d, tab, if_dentro, f, nombre));
            }
        } else if (opcion == 2) {
            var1 = d.genera_id_var(g, 1);
            return fmt::format("{}array {} = {};\n{}", tab, var1, d.genera_arreglo(g), genera(g, dentro, d, tab, if_dentro, f, nombre));
        } else if (opcion == 3) {
            var1 = d.genera_id_var(g, 1);
            if (d.v_arr.size() < 4) {
                return fmt::format("{}array {} = {};\n{}", tab, var1, d.genera_arreglo(g), genera(g, dentro, d, tab, if_dentro, f, nombre));
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
                return fmt::format("{}array {} = {};\n{}", tab, var1, der, genera(g, dentro, d, tab, if_dentro, f, nombre));
            }
        } else if (opcion == 4 && if_dentro < 2) {
            tab += "\t";
            return fmt::format("{}if {} {{\n{}\n{}}}\n{}", arregla_tab(tab), d.genera_condicion(g), genera(g, dentro, d, tab, if_dentro + 1, f, nombre), arregla_tab(tab), genera(g, dentro, d, arregla_tab(tab), if_dentro, f, nombre));
        } else if(opcion == 5 && if_dentro < 2) {
            tab += "\t";
            return fmt::format("{}if {} {{\n{}\n{}}} else {{\n{}\n{}}}\n{}", arregla_tab(tab), d.genera_condicion(g), genera(g, dentro, d, tab, if_dentro + 1, f, nombre), arregla_tab(tab), genera(g, dentro, d, tab, if_dentro + 1, f, nombre), arregla_tab(tab), genera(g, dentro, d, arregla_tab(tab), if_dentro, f, nombre));
        }else if (opcion == 7 && f.num.size() > 2) {
            var1 = d.genera_id_var(g, 0);
            if (g.rand() == 0) {
                return fmt::format("{}number {} = {};\n{}", tab, var1, d.llama_func(g, f.num, "", nombre), genera(g, dentro, d, tab, if_dentro, f, nombre));
            } else {
                return fmt::format("{}number {} = {} {} {};\n{}", tab, var1, d.llama_func(g, f.num, "", nombre), genera_op_num(), d.llama_func(g, f.num, "", nombre), genera(g, dentro, d, tab, if_dentro, f, nombre));
            }
        } else if (opcion == 8 && f.arr.size() > 2) {
            var1 = d.genera_id_var(g, 1);
            int op_arr = g.rand() % 2;
            if (d.v_arr.size() > 4) {
                der = fmt::format("{} * {}", d.obten_id(g, var1, 1), d.llama_func(g, f.arr, "", nombre));
            } else {
                if (op_arr == 0) {
                    der = fmt::format("{}", d.llama_func(g, f.arr, "", nombre));
                } else {
                    der = fmt::format("{} * {}", d.llama_func(g, f.arr, "", nombre), d.genera_arreglo(g));
                }
            }
            return fmt::format("{}array {} = {};\n{}", tab, var1, der, genera(g, dentro, d, tab, if_dentro, f, nombre));
        } else {
            int op = g.rand() % 2, t = dentro - 1;
            return fmt::format("{}return {};", tab, d.obten_id(g, "", t));
        }
    }
    std::string tipo = d.genera_tipo(g);
    nombre = d.genera_id_func(g, f);
    std::vector<int> v;
    var1 = d.genera_parametros(g, v);
    if (tipo[0] == 'n') {
        f.num[nombre] = v;
    } else {
        f.arr[nombre] = v;
    }
    return fmt::format("function {}{}: {}{{\n{}\n}}\n", nombre, var1, tipo, genera(g, (tipo[0] == 'n' ? 1 : 2), d, tab += "\t", if_dentro, f, nombre));
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    srand(time(0));

    datos d;
    generador g;
    funciones f;

    int n;
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        d.v_num.clear();
        d.v_arr.clear();
        std::string s = genera(g, 0, d, "", 0, f, "");
        std::cout << s << '\n';
    }
}