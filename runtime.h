#ifndef LIB_RUNTIME_H
#define LIB_RUNTIME_H

#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <string>

double vector_size(const std::vector<double>& v){
    return static_cast<double>(v.size());
}

std::vector<double> slice(const std::vector<double>& v, const double& ini, const double& fin){
    return std::vector<double>(v.begin() + ini, v.begin() + fin);
}

std::vector<double> concatenate(std::vector<double>& v1, std::vector<double>& v2){
    std::vector<double> aux = v1;
    aux.insert(aux.end(), v2.begin(), v2.end());
    return aux;
}

double obten_numero(const char* arg) {
    return std::stod(std::string(arg));
}

std::vector<double> obten_arreglo(const char* arg) {
    std::string s(arg);       
    std::vector<double> v;
    if(s == "[]"){
        return v;
    }
    s = s.substr(1, s.size() - 2);
    std::stringstream ss(s);    
    while(ss.good()){
        std::string num;
        std::getline(ss, num, ',');        
        v.push_back(std::stod(num));
    }
    return v;
}

void imprime_arreglo(const std::vector<double>& v){
    int tam = v.size();
    std::cout << "[";
    for(int i = 0 ; i < tam ; ++i){
        std::cout << v[i] << (i == tam - 1 ? "]\n" : ",");
    }
}

#endif

