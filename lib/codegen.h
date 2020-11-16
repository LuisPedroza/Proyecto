#ifndef LIB_CODEGEN_H
#define LIB_CODEGEN_H

#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
#include <sstream>

#include "lexer.h"
#include "parser.h"
#include "semantico.h"

namespace lib {
	void escribe_expresion(const expresion& nodo,std::ostream& os);
	void escribe_expresion_terminal(const expresion_terminal& nodo,std::ostream& os) {
		os << std::string_view(*nodo.t);
	}

	void escribe_expresion_op_prefijo(const expresion_op_prefijo& nodo,std::ostream& os) {		
		token tipo = nodo.operador->tipo;
		if(tipo == TAMANYO_ARREGLO){
			escribe_expresion(*nodo.sobre, os);
			os << ".size()";
		}else if(tipo == RESTA){
			os << "-";
			escribe_expresion(*nodo.sobre, os);
		}else{
			os << "!";
			escribe_expresion(*nodo.sobre, os);
		}
	}

	void escribe_expresion_op_binario(const expresion_op_binario& nodo,std::ostream& os) {		
		token operador = nodo.operador->tipo;
		if(operador == RESIDUO){
			os << "std::fmod(";
			escribe_expresion(*nodo.izq, os);
			os << ",";
			escribe_expresion(*nodo.der, os);
			os << ")";
		}else if(operador == PISO){	
			os << "std::floor(";
			escribe_expresion(*nodo.izq, os);
			os << "/";
			escribe_expresion(*nodo.der, os);
			os << ")";
		}else if(operador == POTENCIA){
			os << "std::pow(";
			escribe_expresion(*nodo.izq, os);
			os << ",";
			escribe_expresion(*nodo.der, os);
			os << ")";
		}else{
			os << "(";
			escribe_expresion(*nodo.izq, os);
			os << (operador == AND ? "&&" : operador == OR ? "||" :  operador == IGUAL ? "==" : std::string(*nodo.operador));
			escribe_expresion(*nodo.der, os);
			os << ")";
		}  
	}

	void escribe_expresion_parentesis_posfijo(const expresion_parentesis_posfijo& nodo,std::ostream& os) {
		escribe_expresion(*nodo.func, os);
		os << "(";
		int tam = nodo.parametros.size();
		for(int i = 0 ; i < tam ; ++i){
			escribe_expresion(*nodo.parametros[i], os);
			os << (i == tam - 1 ? "" : ",");
		}
		os << ")";
	}

	void escribe_expresion_corchetes_posfijo(const expresion_corchetes_posfijo& nodo,std::ostream& os) {
		escribe_expresion(*nodo.ex, os);
		if(nodo.der != nullptr){
			os << ".slice(";
			escribe_expresion(*nodo.izq, os);
			os << ", ";
			escribe_expresion(*nodo.der, os);
			os << ")";
		}else{
			os << "[";
			escribe_expresion(*nodo.der, os);
			os << "]";
		}
	}

	void escribe_expresion_arreglo(const expresion_arreglo& nodo,std::ostream& os) {
		os << "runtime::array({";
		int tam = nodo.elementos.size();
		for(int i = 0 ; i < tam ; ++i){
			escribe_expresion(*nodo.elementos[i], os);
			os << (i == tam - 1 ? "" : ",");
		}
		os << "})";
	}

	void escribe_expresion(const expresion& nodo,std::ostream& os) {
        if (auto p = dynamic_cast<const expresion_terminal*>(&nodo); p != nullptr) {
            return escribe_expresion_terminal(*p, os);
        } else if (auto p = dynamic_cast<const expresion_op_prefijo*>(&nodo); p != nullptr) {
            return escribe_expresion_op_prefijo(*p, os);
        } else if (auto p = dynamic_cast<const expresion_op_binario*>(&nodo); p != nullptr) {
            return escribe_expresion_op_binario(*p, os);
        } else if (auto p = dynamic_cast<const expresion_parentesis_posfijo*>(&nodo); p != nullptr) {
            return escribe_expresion_parentesis_posfijo(*p, os);
        } else if (auto p = dynamic_cast<const expresion_corchetes_posfijo*>(&nodo); p != nullptr) {
            return escribe_expresion_corchetes_posfijo(*p, os);
        } else if (auto p = dynamic_cast<const expresion_arreglo*>(&nodo); p != nullptr) {
            return escribe_expresion_arreglo(*p, os);
        }
    }

	void escribe_sentencia(const sentencia& nodo,std::ostream& os, bool es_main);
	void escribe_declaracion(const sentencia_declaracion& nodo,std::ostream& os, bool es_main) {		
		os << "runtime::" << std::string_view(*nodo.tipo) << " " << std::string_view(*nodo.nombre) << " = ";
		escribe_expresion(*nodo.inicializador, os);
		os << ";\n";
	}

	void escribe_if(const sentencia_if& nodo,std::ostream& os, bool es_main) {
		os << "if(";
		escribe_expresion(*nodo.condicion, os);
		os << "){\n";
		for(auto& s : nodo.parte_si){
            escribe_sentencia(*s, os, es_main);
        }
		os << "}\n";
		if(!nodo.parte_no.empty()){
			os << "else{\n";
			for(auto& s : nodo.parte_no){
				escribe_sentencia(*s, os, es_main);
			}
			os << "}\n";
		}
	}

	void escribe_return(const sentencia_return& nodo,std::ostream& os, bool es_main) {
		if(es_main){
			os << "runtime::print(";
			escribe_expresion(*nodo.ex, os);
			os << ");\n";			
			os << "return 0;\n";
		}else{
			os << "return ";
			escribe_expresion(*nodo.ex, os);
			os << ";\n";
		}

	}

	void escribe_sentencia(const sentencia& nodo,std::ostream& os, bool es_main) {
		if (auto p = dynamic_cast<const sentencia_declaracion*>(&nodo); p != nullptr) {
            return escribe_declaracion(dynamic_cast<const sentencia_declaracion&>(nodo), os, es_main);
        } else if (auto p = dynamic_cast<const sentencia_if*>(&nodo); p != nullptr) {
            return escribe_if(dynamic_cast<const sentencia_if&>(nodo), os, es_main);
        } else if (auto p = dynamic_cast<const sentencia_return*>(&nodo); p != nullptr) {
            return escribe_return(dynamic_cast<const sentencia_return&>(nodo), os, es_main);
        }
	}	

	template <typename FI>
	void escribe_funcion(FI iter, std::ostream& os) {		
		os << "#include \"runtime.h\"\n\n";		
		bool es_main = false;
		while (iter->nombre != nullptr) {
			std::string_view aux = *iter->nombre;
			std::string nombre = {aux.begin(), aux.end()};
			if(nombre == "main"){
				es_main = true;
				os << "int " << nombre << "(int argc, char *argv[]){\n";
				int tam = iter->parametros.size();
				for(int i = 0 ; i < tam ; ++i){
					auto tipo = *iter->parametros[i].tipo;
					auto id = *iter->parametros[i].nombre;
					std::string arg = "argv[" + std::to_string(i + 1) + "]";
					os << "runtime::" << std::string_view(tipo) << " " << std::string_view(id);
					os << (tipo.tipo == NUMERO ?  " = runtime::read_number(" : " = runtime::read_array(") << arg << ");\n";
				}
			}else{				
				os << "runtime::" << *iter->retorno << " " << *iter->nombre << "(";
				int tam = iter->parametros.size();
				for(int i = 0 ; i < tam ; ++i){
					auto tipo = *iter->parametros[i].tipo;
					auto id = *iter->parametros[i].nombre;
					os << "const runtime::" << std::string_view(tipo) << "& " << std::string_view(id) << (i == tam - 1 ? "" : ",");
				}
				os << "){" << '\n';
			}
			for(const auto& s : iter-> sentencias){
				escribe_sentencia(*s, os, es_main);
			}
			os << "\n"  << '}' << "\n\n";
			iter++;
		}
	}

} 

#endif
