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
		std::string_view nombre = *nodo.t;
		if(nodo.t->tipo == LITERAL_NUMERICA){
			escribe_tipo(gen, NUMERO);
			gen.flujo << temp << " = " << std::string_view(*nodo.t) << ";\n";
			return datos_expresion{NUMERO, temp};
		}else{
			if(f.count(*nodo.t) != 0){
				return datos_expresion{FUNCION, {nombre.begin(), nombre.end()}};
			}else{				
				std::map<std::string_view, token>::iterator it;
				for (auto& m : a) {
					it = m.find(*nodo.t);
					if (it != m.end()) {
						break;
					}
            	}				
				//escribe_tipo(gen, tipo);
				//gen.flujo << temp << " = " << nombre << ";\n";
				return datos_expresion{it->second, {nombre.begin(), nombre.end()}};
			}
		}
	}

	datos_expresion escribe_expresion_op_prefijo(const expresion_op_prefijo& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		token tipo = nodo.operador->tipo;
		auto temp = genera_temporal(gen.id_temporal);
		datos_expresion id = escribe_expresion(*nodo.sobre, f, a, gen);
		escribe_tipo(gen, NUMERO);
		if(tipo == TAMANYO_ARREGLO){			
			gen.flujo << temp << " = " << "vector_size(" << id.nombre << ");\n";			
		}else if(tipo == RESTA){			
			gen.flujo << temp << " = " << "-" << id.nombre << ";\n";			
		}else{			
			gen.flujo << temp << " = " << "!" << id.nombre << ");\n";			
		}
		return datos_expresion{NUMERO, temp};
	}

	datos_expresion escribe_expresion_op_binario(const expresion_op_binario& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		datos_expresion izq = escribe_expresion(*nodo.izq, f, a, gen);
		datos_expresion der = escribe_expresion(*nodo.der, f, a, gen);		
		auto temp = genera_temporal(gen.id_temporal);
		if (izq.tipo == ARREGLO && nodo.operador->tipo == MULTIPLICACION) {
            escribe_tipo(gen, ARREGLO);			
			gen.flujo << temp << " = " << "concatenate(" << izq.nombre << "," << der.nombre << ");\n";
			return datos_expresion{ARREGLO, temp};
        }else{
			escribe_tipo(gen, NUMERO);
			token operador = nodo.operador->tipo;
			if(operador == RESIDUO){
				gen.flujo << temp << " = " << "std::fmod(" << izq.nombre << "," << der.nombre << ");\n";
			}else if(operador == PISO){
				gen.flujo << temp << " = " << "std::floor(" << izq.nombre << "/" << der.nombre << ");\n";
			}else if(operador == POTENCIA){
				gen.flujo << temp << " = " << "std::pow(" << izq.nombre << "," << der.nombre << ");\n";
			}else{
				std::string op = (operador == AND ? "&&" : operador == OR ? "||" :  operador == IGUAL ? "==" : std::string(*nodo.operador));
				gen.flujo << temp << " = " << izq.nombre << " " << op << " " << der.nombre << ";\n";	
			}            
            return datos_expresion{NUMERO, temp};
        }
	}

	datos_expresion escribe_expresion_parentesis_posfijo(const expresion_parentesis_posfijo& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		datos_expresion id = escribe_expresion(*nodo.func, f, a, gen);
		auto temp = genera_temporal(gen.id_temporal);
		if(id.tipo == FUNCION){
			std::string s = "";
			int tam = nodo.parametros.size();
			for(int i = 0 ; i < tam ; ++i){
				datos_expresion d = escribe_expresion(*nodo.parametros[i], f, a, gen);
				s += d.nombre + ((i == tam - 1 ? ");\n" : ","));
			}
			token tipo = f[*nodo.func->get_token()].retorno;
			escribe_tipo(gen, tipo);			
			gen.flujo << temp << " = " << id.nombre << "(" << s;
			return datos_expresion{tipo, temp};
		}else{
			datos_expresion indice = escribe_expresion(**nodo.parametros.begin(), f, a, gen);
			datos_expresion arreglo = escribe_expresion(*nodo.func, f, a, gen);
			escribe_tipo(gen, NUMERO);			
			gen.flujo << temp << " = " << arreglo.nombre << "[" << indice.nombre << "];\n";
			return datos_expresion{NUMERO, temp};
		}
	}

	datos_expresion escribe_expresion_corchetes_posfijo(const expresion_corchetes_posfijo& nodo, funciones& f, ambitos& a, generador_codigo& gen) {
		datos_expresion izq = escribe_expresion(*nodo.izq, f, a, gen);
		datos_expresion ex = escribe_expresion(*nodo.ex, f, a, gen);
		auto temp = genera_temporal(gen.id_temporal);
		if(nodo.der != nullptr){
			datos_expresion der = escribe_expresion(*nodo.der, f, a, gen);
			escribe_tipo(gen, ARREGLO);
			gen.flujo << temp << " = " << "slice(" << ex.nombre << "," << izq.nombre << "," << der.nombre << ");\n";
		}else{
			escribe_tipo(gen, ARREGLO);
			gen.flujo << temp << " = " << "slice(" << ex.nombre << "," << izq.nombre << "," << izq.nombre << ");\n";
		}
		return datos_expresion{ARREGLO, temp};
	}

	datos_expresion escribe_expresion_arreglo(const expresion_arreglo& nodo, funciones& f, ambitos& a, generador_codigo& gen) {				
		std::string s = "";
		auto temp = genera_temporal(gen.id_temporal);
		int tam = nodo.elementos.size();
		for(int i = 0 ; i < tam ; ++i){
			datos_expresion d = escribe_expresion(*nodo.elementos[i], f, a, gen);
			 s += d.nombre + ((i == tam - 1 ? "};\n" : ","));
		}				
		escribe_tipo(gen, ARREGLO);		
		gen.flujo << temp << " = {" << s;
        return datos_expresion{ARREGLO, temp};
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

	void escribe_sentencia(const sentencia& nodo, funciones& f, ambitos& a, generador_codigo& gen, bool es_main);
	void escribe_declaracion(const sentencia_declaracion& nodo, funciones& f, ambitos& a, generador_codigo& gen, bool es_main) {		
		datos_expresion d = escribe_expresion(*nodo.inicializador, f, a, gen);		
		escribe_tipo(gen, nodo.tipo->tipo);
		gen.flujo << std::string_view(*nodo.nombre) << " = " << d.nombre << ";\n";		
		declara(a.back(), *nodo.nombre, nodo.tipo->tipo);
	}

	void escribe_if(const sentencia_if& nodo, funciones& f, ambitos& a, generador_codigo& gen, bool es_main) {
		datos_expresion d = escribe_expresion(*nodo.condicion, f, a, gen);
		gen.flujo << "if(" << d.nombre << "){\n";
		ambito m_si;
        a.push_back(m_si);
		for(auto& s : nodo.parte_si){
            escribe_sentencia(*s, f, a, gen, es_main);
        }
		gen.flujo << "}\n";
		a.pop_back();		
        if(!nodo.parte_no.empty()){
            ambito m_no;
			gen.flujo << "else{\n";
            a.push_back(m_no);
            for(auto& s : nodo.parte_no){
                escribe_sentencia(*s, f, a, gen, es_main);
            }
            a.pop_back();
			gen.flujo << "}\n";
        }
	}

	void escribe_return(const sentencia_return& nodo, funciones& f, ambitos& a, generador_codigo& gen, bool es_main) {
		datos_expresion d = escribe_expresion(*nodo.ex, f, a, gen);
		if(es_main){
			if(d.tipo == NUMERO){
				gen.flujo << "std::cout << " << d.nombre << " << \'\\n\';\n";				
			}else{
				gen.flujo << "imprime_arreglo(" << d.nombre << ");\n";
			}
			gen.flujo << "return 0;\n";
		}
		gen.flujo << "return " << d.nombre << ";\n";
	}

	void escribe_sentencia(const sentencia& nodo, funciones& f, ambitos& a, generador_codigo& gen, bool es_main) {
		if (auto p = dynamic_cast<const sentencia_declaracion*>(&nodo); p != nullptr) {
            return escribe_declaracion(dynamic_cast<const sentencia_declaracion&>(nodo), f, a, gen, es_main);
        } else if (auto p = dynamic_cast<const sentencia_if*>(&nodo); p != nullptr) {
            return escribe_if(dynamic_cast<const sentencia_if&>(nodo), f, a, gen, es_main);
        } else if (auto p = dynamic_cast<const sentencia_return*>(&nodo); p != nullptr) {
            return escribe_return(dynamic_cast<const sentencia_return&>(nodo), f, a, gen, es_main);
        }
	}	

	template <typename FI>
	void escribe_funcion(FI iter, funciones& f, std::ostringstream& os) {
		generador_codigo gen = {os};
		gen.flujo << "#include \"runtime.h\"\n\n";
		ambito ap;
		bool es_main = false;
		while (iter->nombre != nullptr) {
			std::string_view aux = *iter->nombre;
			std::string nombre = {aux.begin(), aux.end()};
			if(nombre == "main"){
				es_main = true;
				gen.flujo << "int " << nombre << "(int argc, char *argv[]){\n";
				int tam = iter->parametros.size();				
				for(int i = 0 ; i < tam ; ++i){					
					token tipo = iter->parametros[i].tipo->tipo;
					std::string_view nombre = *iter->parametros[i].nombre;
					ap.emplace(nombre, tipo);
					std::string arg = "argv[" + std::to_string(i + 1) + "]";
					if(tipo == NUMERO){
						escribe_tipo(gen, NUMERO);
						gen.flujo << nombre << " = obten_numero(" << arg << ");\n";
					}else{
						escribe_tipo(gen, ARREGLO);
						gen.flujo << nombre << " = obten_arreglo(" << arg << ");\n";
					}
				}
			}else{
				gen.id_temporal = 0;
				escribe_tipo(gen, iter->retorno->tipo);
				gen.flujo << *iter->nombre << '(';
				int tam = iter->parametros.size();
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
			}
			ambitos a;
			a.push_back(ap);
			for(const auto& s : iter-> sentencias){
				escribe_sentencia(*s, f, a, gen, es_main);
			}
			gen.flujo << "\n"  << '}' << "\n\n";
			iter++;
		}
	}

} 

#endif
