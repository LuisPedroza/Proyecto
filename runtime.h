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

double mod(const double& a, const double& b){
    return std::fmod(a,b);
}

std::vector<double> slice(const std::vector<double>& v, const double& ini, const double& fin){
    return std::vector<double>(v.begin() + ini, v.begin() + fin);
}

void concatenate(std::vector<double>& dest, std::vector<double>& src){
    dest.insert(dest.end(), std::make_move_iterator(src.begin()), std::make_move_iterator(src.end()));
}

#endif

