#ifndef __CODE_GENERATOR_H__
#define __CODE_GENERATOR_H__

#include <fstream>
#include <string>
#include <vector>
#include "symbol_table.hpp"
#include "syntax_tree.hpp"

namespace ntt {

    class CodeGenerator {

        public:
            void compile_term(const SyntaxTree& tree);

            void compile_expression(const SyntaxTree& tree);

            void compile_class_var_dec(const SyntaxTree& tree);

            void compile_parameter_list(const SyntaxTree& tree);

            void compile_var_dec(const SyntaxTree& tree);

            void write(std::ofstream& ofs);

        private:
            std::vector<std::string> vm_commands_;

            SymbolTable symbol_table_;
    };
}

#endif
