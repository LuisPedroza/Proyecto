#ifndef LIB_DEBUG_H
#define LIB_DEBUG_H

#include"parser.h"
#include <ostream>
#include <string_view>

namespace lib{

    std::ostream& operator<<(std::ostream& os, std::string_view s){
        return os.write(s.begin(), s.size());
    }

    std::ostream& operator<<(std::ostream& os, const token_anotada& t){
        return os << std::string_view(t);
    }

    std::ostream& operator<<(std::ostream& os, const expresion& e);
    std::ostream& operator<<(std::ostream& os, const expresion_terminal& e){
        return os << *e.t;
    }

    std::ostream& operator<<(std::ostream& os, const expresion_op_prefijo& e){
        return os << *e.operador << *e.sobre;
    }

    std::ostream& operator<<(std::ostream& os, const expresion_op_binario& e){
        return os << *e.izq << *e.operador << *e.der;
    }

    std::ostream& operator<<(std::ostream& os, const expresion_parentesis_posfijo& e){
        os << *e.func << '(';
        for(auto const& i : e.parametros){
            os << *i << ',';
        }
        return os << ')';
    }

    std::ostream& operator<<(std::ostream& os, const expresion_corchetes_posfijo& e){
        return os << *e.ex << '[' << *e.izq << ".." << *e.der << ']';
    }

    std::ostream& operator<<(std::ostream& os, const expresion_arreglo& e){
        os << '[';
        for(auto const& i : e.elementos){
            os << *i << ',';
        }
        return os << ']';
    }

    std::ostream& operator<<(std::ostream& os, const expresion& e){
        if(typeid(e) == typeid(expresion_terminal)){
            return os << dynamic_cast<const expresion_terminal&>(e);
        }else if(typeid(e) == typeid(expresion_op_prefijo)){
            return os << dynamic_cast<const expresion_op_prefijo&>(e);
        }else if(typeid(e) == typeid(expresion_op_binario)){
            return os << dynamic_cast<const expresion_op_binario&>(e);
        }else if(typeid(e) == typeid(expresion_parentesis_posfijo)){
            return os << dynamic_cast<const expresion_parentesis_posfijo&>(e);
        }else if(typeid(e) == typeid(expresion_corchetes_posfijo)){
            return os << dynamic_cast<const expresion_corchetes_posfijo&>(e);
        }else if(typeid(e) == typeid(expresion_arreglo)){
            return os << dynamic_cast<const expresion_arreglo&>(e);
        }
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const sentencia& s);
    std::ostream& operator<<(std::ostream& os, const sentencia_declaracion& s){
        return os << *s.tipo << ' ' << *s.nombre << '=' << *s.inicializador << '\n';
    }

    std::ostream& operator<<(std::ostream& os, const sentencia_if& s){
        os << "if " << *s.condicion << "\n";
        for(auto const& i : s.parte_si){
            os << *i;
        }
        os << "else" << "\n";
        for(auto const& i : s.parte_no){
            os << *i;
        }
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const sentencia_return& s){
        return os << "return " << *s.ex << '\n';
    }

    std::ostream& operator<<(std::ostream& os, const sentencia& s){
        if(typeid(s) == typeid(sentencia_declaracion)){
            return os << dynamic_cast<const sentencia_declaracion&>(s);
        }else if(typeid(s) == typeid(sentencia_if)){
            return os << dynamic_cast<const sentencia_if&>(s);
        }else if(typeid(s) == typeid(sentencia_return)){
            return os << dynamic_cast<const sentencia_return&>(s);
        }
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const declaracion_funcion& d){
        if (d.nombre != nullptr) {
            os << "function " << *d.nombre << '(';
            for(auto const& i : d.parametros){
                os << *i.tipo << ' ' << *i.nombre << ',';
            }
            os << ") : " << *d.retorno << '\n';
            for(auto const& i : d.sentencias){
                os << *i;
            }
            os << '\n';
        }
        return os;
    }

}


#endif
