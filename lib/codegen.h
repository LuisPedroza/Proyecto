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
	struct generador_codigo {
		std::ostringstream& flujo;
		std::size_t id_temporal = 0;
	};

	void escribe_tipo(generador_codigo& gen, const token& tipo){
		gen.flujo << (tipo == NUMERO ? "double" : "std::vector<double>") << ' ';
	}

	std::string genera_temporal(std::size_t& id){		
		return "_t" + std::to_string(int(id++));
	}
 
	datos_expresion escribe_expresion(const expresion& nodo, funciones& f, ambitos& a, generador_codigo& gen);
	datos_expresion escribe_expresion_terminal(const expresion_terminal& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		auto temp = genera_temporal(gen.id_temporal);
		if(nodo.t->tipo == LITERAL_NUMERICA){
			escribe_tipo(gen, NUMERO);
			gen.flujo << temp << " = " << std::string_view(*nodo.t) << ";\n";
			return datos_expresion{NUMERO, temp};
		}else{
			if(f.count(*nodo.t) != 0){
				return datos_expresion{FUNCION, *nodo.t};
			}else{
				token tipo = a.back()[*nodo.t];
				escribe_tipo(gen, tipo);
				gen.flujo << temp << " = " << std::string_view(*nodo.t) << ";\n";
				return datos_expresion{tipo, temp};
			}
		}
	}

	datos_expresion escribe_expresion_op_prefijo(const expresion_op_prefijo& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		token tipo = nodo.operador->tipo;
		auto temp = genera_temporal(gen.id_temporal);
		datos_expresion id = escribe_expresion(*nodo.sobre, f, a, gen);
		if(tipo == TAMANYO_ARREGLO){
			escribe_tipo(gen, NUMERO);
			gen.flujo << temp << " = " << "vector_size(" << id.nombre << ");\n";
			return datos_expresion{NUMERO, temp};
		}else if(tipo == RESTA){
			escribe_tipo(gen, NUMERO);
			gen.flujo << temp << " = " << "-" << id.nombre << ";\n";
			return datos_expresion{NUMERO, temp};
		}else{
			escribe_tipo(gen, NUMERO);
			gen.flujo << temp << " = " << "not_operator(" << id.nombre << ");\n";
			return datos_expresion{NUMERO, temp};
		}
	}

	datos_expresion escribe_expresion_op_binario(const expresion_op_binario& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		datos_expresion izq = escribe_expresion(*nodo.izq, f, a, gen);
		datos_expresion der = escribe_expresion(*nodo.der, f, a, gen);		
		if (izq.tipo == ARREGLO && nodo.operador->tipo == MULTIPLICACION) {
            escribe_tipo(gen, ARREGLO);
			auto temp = genera_temporal(gen.id_temporal);
			gen.flujo << temp << " = " << "concatenate(" << izq.nombre << "," << der.nombre << ");\n";
			return datos_expresion{ARREGLO, temp};
        }else{
			escribe_tipo(gen, NUMERO);
			auto temp = genera_temporal(gen.id_temporal);
            gen.flujo << temp << " = " << izq.nombre << " " << std::string_view(*nodo.operador) << " " << der.nombre << ";\n";
            return datos_expresion{NUMERO, temp};
        }
	}

	datos_expresion escribe_expresion_parentesis_posfijo(const expresion_parentesis_posfijo& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		datos_expresion id = escribe_expresion(*nodo.func, f, a, gen);				
		if(id.tipo == FUNCION){
			std::string s = "";
			int tam = nodo.parametros.size();
			for(int i = 0 ; i < tam ; ++i){
				datos_expresion d = escribe_expresion(*nodo.parametros[i], f, a, gen);
				s += std::string(d.nombre) + ((i == tam - 1 ? ");\n" : ","));
			}
			token tipo = f[*nodo.func->get_token()].retorno;
			escribe_tipo(gen, tipo);
			auto temp = genera_temporal(gen.id_temporal);
			gen.flujo << temp << " = " << id.nombre << "(" << s;
			return datos_expresion{tipo, temp};
		}else{
			datos_expresion indice = escribe_expresion(**nodo.parametros.begin(), f, a, gen);
			escribe_tipo(gen, NUMERO);
			auto temp = genera_temporal(gen.id_temporal);
			gen.flujo << temp << " = " << std::string_view(*nodo.func->get_token()) << "[" << indice.nombre << "];\n";
			return datos_expresion{NUMERO, temp};
		}
	}

	datos_expresion escribe_expresion_corchetes_posfijo(const expresion_corchetes_posfijo& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		datos_expresion izq = escribe_expresion(*nodo.izq, f, a, gen);
		auto temp = genera_temporal(gen.id_temporal);
		if(nodo.der != nullptr){
			datos_expresion der = escribe_expresion(*nodo.der, f, a, gen);
			escribe_tipo(gen, ARREGLO);
			gen.flujo << temp << " = " << "slice(" << std::string_view(*nodo.ex->get_token()) << "," << izq.nombre << "," << der.nombre << ");\n";
		}else{
			escribe_tipo(gen, ARREGLO);
			gen.flujo << temp << " = " << "slice(" << std::string_view(*nodo.ex->get_token()) << "," << izq.nombre << "," << izq.nombre << ");\n";
		}
		return datos_expresion{ARREGLO, temp};
	}

	datos_expresion escribe_expresion_arreglo(const expresion_arreglo& nodo, funciones& f, ambitos& a, generador_codigo& gen) {				
		std::string s = "";
		int tam = nodo.elementos.size();
		for(int i = 0 ; i < tam ; ++i){
			datos_expresion d = escribe_expresion(*nodo.elementos[i], f, a, gen);
			 s += std::string(d.nombre) + ((i == tam - 1 ? "};\n" : ","));			
		}				
		escribe_tipo(gen, ARREGLO);
		auto temp = datos_expresion{ARREGLO, genera_temporal(gen.id_temporal)};
		gen.flujo << temp.nombre << " = {" << s;
        return temp;
	}

	datos_expresion escribe_expresion(const expresion& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
        if (auto p = dynamic_cast<const expresion_terminal*>(&nodo); p != nullptr) {
            return escribe_expresion_terminal(*p, f, a, gen);
        } else if (auto p = dynamic_cast<const expresion_op_prefijo*>(&nodo); p != nullptr) {
            return escribe_expresion_op_prefijo(*p, f, a, gen);
        } else if (auto p = dynamic_cast<const expresion_op_binario*>(&nodo); p != nullptr) {
            return escribe_expresion_op_binario(*p, f, a, gen);
        } else if (auto p = dynamic_cast<const expresion_parentesis_posfijo*>(&nodo); p != nullptr) {
            return escribe_expresion_parentesis_posfijo(*p, f, a, gen);
        } else if (auto p = dynamic_cast<const expresion_corchetes_posfijo*>(&nodo); p != nullptr) {
            return escribe_expresion_corchetes_posfijo(*p, f, a, gen);
        } else if (auto p = dynamic_cast<const expresion_arreglo*>(&nodo); p != nullptr) {
            return escribe_expresion_arreglo(*p, f, a, gen);
        }
    }

	void escribe_sentencia(const sentencia& nodo, funciones& f, ambitos& a, generador_codigo& gen);
	void escribe_declaracion(const sentencia_declaracion& nodo, funciones& f, ambitos& a, generador_codigo& gen) {		
		datos_expresion d = escribe_expresion(*nodo.inicializador, f, a, gen);		
		escribe_tipo(gen, nodo.tipo->tipo);
		gen.flujo << std::string_view(*nodo.nombre) << " = " << d.nombre << ";\n";
		declara(a.back(), *nodo.nombre, nodo.tipo->tipo);
	}

	void escribe_if(const sentencia_if& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		//...
	}

	void escribe_return(const sentencia_return& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		//...
	}

	void escribe_sentencia(const sentencia& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		if (auto p = dynamic_cast<const sentencia_declaracion*>(&nodo); p != nullptr) {
            return escribe_declaracion(dynamic_cast<const sentencia_declaracion&>(nodo), f, a, gen);
        } else if (auto p = dynamic_cast<const sentencia_if*>(&nodo); p != nullptr) {
            return escribe_if(dynamic_cast<const sentencia_if&>(nodo), f, a, gen);
        } else if (auto p = dynamic_cast<const sentencia_return*>(&nodo); p != nullptr) {
            return escribe_return(dynamic_cast<const sentencia_return&>(nodo), f, a, gen);
        }
	}	

	template <typename FI>
	void escribe_funcion(FI iter, funciones& f, std::ostringstream& os) {
		generador_codigo gen = {os};
		gen.flujo << "#include \"runtime.h\"\n\n";
		while (iter->nombre != nullptr) {
			gen.id_temporal = 0;
			escribe_tipo(gen, iter->retorno->tipo);
			gen.flujo << *iter->nombre << '(';
			int tam = iter->parametros.size();
			ambito ap;
			for(int i = 0 ; i < tam ; ++i){
				token tipo = iter->parametros[i].tipo->tipo;
				std::string_view nombre = *iter->parametros[i].nombre;
				ap.emplace(nombre, tipo);
				escribe_tipo(gen, tipo);
				gen.flujo << nombre << (i == tam - 1 ? ')' : ',');
			}
			if(tam == 0){
				gen.flujo << ")";
			}
			gen.flujo << '{' << '\n';
			ambitos a;
			a.push_back(ap);
			for(const auto& s : iter-> sentencias){
				escribe_sentencia(*s, f, a, gen);
			}
			gen.flujo << "\n"  << '}' << "\n\n";
			iter++;
		}
	}

} 

#endif
