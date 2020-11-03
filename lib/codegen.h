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

#include "lexer.h"
#include "parser.h"
#include "semantico.h"

namespace lib {
	struct generador_codigo {
		std::ostream& flujo;
		std::size_t id_temporal = 0;
	};

	datos_expresion escribe_expresion(const expresion& nodo, funciones& f, ambitos& a, generador_codigo& gen);
	datos_expresion escribe_expresion_terminal(const expresion_terminal& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		//...
	}

	datos_expresion escribe_expresion_op_prefijo(const expresion_op_prefijo& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		//...
	}

	datos_expresion escribe_expresion_op_binario(const expresion_op_binario& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		//...
	}

	datos_expresion escribe_expresion_parentesis_posfijo(const expresion_parentesis_posfijo& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		//...
	}

	datos_expresion escribe_expresion_corchetes_posfijo(const expresion_corchetes_posfijo& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		//...
	}

	datos_expresion escribe_expresion_arreglo(const expresion_arreglo& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		//...
	}

	datos_expresion escribe_expresion(const expresion& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		//...
	}

	void escribe_sentencia(const sentencia& nodo, funciones& f, ambitos& a, generador_codigo& gen);
	void escribe_declaracion(const sentencia_declaracion& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		//...
	}

	void escribe_if(const sentencia_if& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		//...
	}

	void escribe_return(const sentencia_return& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		//...
	}

	void escribe_sentencia(const sentencia& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		//...
	}

	template <typename FI>
	void escribe_funcion(FI iter, funciones& f, std::ostream& os) {
		generador_codigo gen = {os};
		while (iter->nombre != nullptr) {
			//...
		}
	}

} 

#endif
