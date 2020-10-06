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

std::string genera1(int dentro, datos d, std::string tab, int if_dentro, funciones& f, std::string nombre) {
    std::string aux1, aux2;
    if (dentro != 0) {
        int opcion = d.g.rand() % 4;
        if (opcion == 0) {
            std::string var_n = d.genera_id_var(0);
            if (d.v_num.size() < 4) {
                return fmt::format("{}number {} = {};\n{}", tab, var_n, d.genera_var_num(f.num, nombre), genera1(dentro, d, tab, if_dentro, f, nombre));
            } else {
                return fmt::format("{}number {} = {} {} {};\n{}", tab, var_n, d.genera_var(var_n, f.num, nombre, 0), d.genera_op_num(), d.genera_var(var_n, f.num, nombre, 0), genera1(dentro, d, tab, if_dentro, f, nombre));
            }
        } else if (opcion == 1) {
            std::string var_a = d.genera_id_var(1), der;
            if (d.v_arr.size() < 4) {
                return fmt::format("{}array {} = {};\n{}", tab, var_a, d.genera_var_arr(f.arr, nombre), genera1(dentro, d, tab, if_dentro, f, nombre));
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
                return fmt::format("{}array {} = {};\n{}", tab, var_a, der, genera1(dentro, d, tab, if_dentro, f, nombre));
            }
        } else if (opcion == 2 && if_dentro < 1) {
            tab += "\t";
            int op_if = d.g.rand() % 3;
            if (op_if == 0) {
                return fmt::format("{}if {} {{\n{}\n{}}}\n{}", arregla_tab(tab), d.genera_condicion(), genera1(dentro, d, tab, if_dentro + 1, f, nombre), arregla_tab(tab), genera1(dentro, d, arregla_tab(tab), if_dentro, f, nombre));
            } else if (op_if == 1) {
                return fmt::format("{}if {} {{\n{}\n{}}} else {{\n{}\n{}}}\n{}", arregla_tab(tab), d.genera_condicion(), genera1(dentro, d, tab, if_dentro + 1, f, nombre), arregla_tab(tab), genera1(dentro, d, tab, if_dentro + 1, f, nombre), arregla_tab(tab), genera1(dentro, d, arregla_tab(tab), if_dentro, f, nombre));
            } else {
                return fmt::format("{}if {} {{\n{}\n{}}} else if {} {{\n{}\n{}}} else {{\n{}\n{}}}\n{}", arregla_tab(tab), d.genera_condicion(), genera1(dentro, d, tab, if_dentro + 1, f, nombre), arregla_tab(tab), d.genera_condicion(), genera1(dentro, d, tab, if_dentro + 1, f, nombre), arregla_tab(tab), genera1(dentro, d, tab, if_dentro + 1, f, nombre), arregla_tab(tab), genera1(dentro, d, arregla_tab(tab), if_dentro, f, nombre));
            }
        } else {
            int op = d.g.rand() % 2, t = dentro - 1;
            return fmt::format("{}return {};", tab, d.obten_id("", t));
        }
    }
    nombre = d.genera_id_func(f);
    std::string tipo = d.genera_tipo();
    std::vector<int> v;
    std::string param = d.genera_parametros(v);
    if (tipo[0] == 'n') {
        f.num[nombre] = v;
    } else {
        f.arr[nombre] = v;
    }
    return fmt::format("function {}{}: {}{{\n{}\n}}\n", nombre, param, tipo, genera1((tipo[0] == 'n' ? 1 : 2), d, tab += "\t", if_dentro, f, nombre));
}

// std::string genera(int dentro, datos d, std::string tab, int if_dentro, funciones& f, std::string nombre) {
//     std::string der;
//     std::string var_n = d.genera_id_var(0);
//     std::string var_a = d.genera_id_var(1);
//     if (dentro != 0) {
//         int opcion = d.g.rand() % 10;
//         if (opcion == 0) {
//             return fmt::format("{}number {} = {};\n{}", tab, var_n, d.g.rand(), genera(dentro, d, tab, if_dentro, f, nombre));
//         } else if (opcion == 1) {
//             if (d.v_num.size() < 4) {
//                 return fmt::format("{}number {} = {};\n{}", tab, var_n, d.g.rand(), genera(dentro, d, tab, if_dentro, f, nombre));
//             } else {
//                 return fmt::format("{}number {} = {} {} {};\n{}", tab, var_n, d.genera_var_num(var_n), d.genera_op_num(), d.genera_var_num(var_n), genera(dentro, d, tab, if_dentro, f, nombre));
//             }
//         } else if (opcion == 2) {
//             return fmt::format("{}array {} = {};\n{}", tab, var_a, d.genera_arreglo(), genera(dentro, d, tab, if_dentro, f, nombre));
//         } else if (opcion == 3) {
//             if (d.v_arr.size() < 4) {
//                 return fmt::format("{}array {} = {};\n{}", tab, var_a, d.genera_arreglo(), genera(dentro, d, tab, if_dentro, f, nombre));
//             } else {
//                 int op_arr = d.g.rand() % 4;
//                 std::string id1 = d.obten_id(var_a, 1), id2 = d.obten_id(var_a, 1);
//                 if (op_arr == 0) {
//                     der = fmt::format("{}{}", id1, d.genera_slice());
//                 } else if (op_arr == 1) {
//                     der = fmt::format("{} * {}", d.genera_arreglo(), d.genera_arreglo());
//                 } else if (op_arr == 2) {
//                     der = fmt::format("{} * {}", id1, id2);
//                 } else {
//                     der = fmt::format("{}{} * {}{}", id1, d.genera_slice(), id2, d.genera_slice());
//                 }
//                 return fmt::format("{}array {} = {};\n{}", tab, var_a, der, genera(dentro, d, tab, if_dentro, f, nombre));
//             }
//         } else if (opcion == 4 && if_dentro < 1) {
//             tab += "\t";
//             return fmt::format("{}if {} {{\n{}\n{}}}\n{}", arregla_tab(tab), d.genera_condicion(), genera(dentro, d, tab, if_dentro + 1, f, nombre), arregla_tab(tab), genera(dentro, d, arregla_tab(tab), if_dentro, f, nombre));
//         } else if (opcion == 5 && if_dentro < 2) {
//             tab += "\t";
//             return fmt::format("{}if {} {{\n{}\n{}}} else {{\n{}\n{}}}\n{}", arregla_tab(tab), d.genera_condicion(), genera(dentro, d, tab, if_dentro + 1, f, nombre), arregla_tab(tab), genera(dentro, d, tab, if_dentro + 1, f, nombre), arregla_tab(tab), genera(dentro, d, arregla_tab(tab), if_dentro, f, nombre));
//         } else if (opcion == 7 && f.num.size() > 2) {
//             if (d.g.rand() == 0) {
//                 return fmt::format("{}number {} = {};\n{}", tab, var_n, d.llama_func(f.num, "", nombre), genera(dentro, d, tab, if_dentro, f, nombre));
//             } else {
//                 return fmt::format("{}number {} = {} {} {};\n{}", tab, var_n, d.llama_func(f.num, "", nombre), d.genera_op_num(), d.llama_func(f.num, "", nombre), genera(dentro, d, tab, if_dentro, f, nombre));
//             }
//         } else if (opcion == 8 && f.arr.size() > 2) {
//             int op_arr = d.g.rand() % 2;
//             if (d.v_arr.size() > 4) {
//                 der = fmt::format("{} * {}", d.obten_id(var_a, 1), d.llama_func(f.arr, "", nombre));
//             } else {
//                 if (op_arr == 0) {
//                     der = fmt::format("{}", d.llama_func(f.arr, "", nombre));
//                 } else {
//                     der = fmt::format("{} * {}", d.llama_func(f.arr, "", nombre), d.genera_arreglo());
//                 }
//             }
//             return fmt::format("{}array {} = {};\n{}", tab, var_a, der, genera(dentro, d, tab, if_dentro, f, nombre));
//         } else {
//             int op = d.g.rand() % 2, t = dentro - 1;
//             return fmt::format("{}return {};", tab, d.obten_id("", t));
//         }
//     }
//     std::string tipo = d.genera_tipo();
//     nombre = d.genera_id_func(f);
//     std::vector<int> v;
//     std::string param = d.genera_parametros(v);
//     if (tipo[0] == 'n') {
//         f.num[nombre] = v;
//     } else {
//         f.arr[nombre] = v;
//     }
//     return fmt::format("function {}{}: {}{{\n{}\n}}\n", nombre, param, tipo, genera((tipo[0] == 'n' ? 1 : 2), d, tab += "\t", if_dentro, f, nombre));
// }

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
        std::string s = genera1(0, d, "", 0, f, "");
        std::cout << s << '\n';
    }
}