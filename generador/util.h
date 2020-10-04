#ifndef UTIL_H
#define UTIL_H

#include <random>
#include <string>

#include "fmt/include/fmt/format.h"

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
    generador g;
    int if_dentro;
    std::vector<std::string> v_num;
    std::vector<std::string> v_arr;

    std::string obten_id_num(std::string s) {
        int tam = v_num.size();
        std::string id;
        while (true) {
            id = v_num[g.rand() % tam];
            if (id != s) {
                break;
            }
        }
        return id;
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

    std::string genera_id_func() {
        std::string s1 = "", s2 = "", s3 = "";
        for (int i = 0; i < 3; ++i) {
            s1 += genera_car(0);
            s2 += genera_car(1);
            s3 += genera_car(2);
        }
        return fmt::format("{}_{}_{}", s2, s1, s3);
    }

    std::string genera_tipo() {
        return (g.rand() % 2 == 0 ? "number" : "array");
    }

    int genera_num_int() {
        return g.rand();
    }

    std::string genera_arreglo() {
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

    std::string genera_parametros(int& lim_a) {
        std::string s = "";
        int t = (g.rand() % 10) + 5;
        for (int i = 0; i < t; ++i) {
            auto aux = genera_tipo();
            if (aux[0] == 'n') {
                s += (aux + " " + genera_id_var(0));
            } else {
                s += (aux + " " + genera_id_var(1));
                lim_a += 1;
            }
            if (i == t - 1) {
                continue;
            } else {
                s += ",";
            }
        }
        return fmt::format("({})", s);
    }

    std::string genera_condicion(int lim_a) {
        std::vector<std::string> v;
        v.push_back(std::to_string(g.rand()));
        v.push_back(obten_id_num(""));
        if (lim_a != 0) {
            v.push_back(v_arr[g.rand() % (lim_a)] + "(" + std::to_string(((g.rand() % 5) + 1)) + ")");
            v.push_back("#" + v_arr[g.rand() % lim_a]);
        }
        int tam = v.size();
        return fmt::format("{} {} {}", v[g.rand() % tam], genera_op_cond(), v[g.rand() % tam]);
    }

    std::string genera_slice() {
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