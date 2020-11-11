#ifndef LIB_RUNTIME_H
#define LIB_RUNTIME_H

#include <iostream>
#include <vector>
#include <cmath>

double vector_size(const std::vector<double>& v){
    return static_cast<double>(v.size());
}

double not_operator(const double& n){
    return !n;
}

std::vector<double> slice(const std::vector<double>& v, const double& ini, const double& fin){
    return std::vector<double>(v.begin() + ini, v.begin() + fin);
}

std::vector<double> concatenate(std::vector<double>& v1, std::vector<double>& v2){
    std::vector<double> aux = v1;
    aux.insert(aux.end(), v2.begin(), v2.end());
    return aux;
}

#endif

