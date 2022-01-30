#ifndef __PARSER_H__
#define __PARSER_H__

#include <fstream>

#include "syntax_tree.hpp"
#include "token.hpp"
#include "tokenizer.hpp"
#include <iostream>

namespace ntt {

    class Parser {

        public:
            Parser(std::ifstream& ifs) : tokenizer(ifs) {}

            Tree parse_term();

            Tree parse_exp();

            Tree parse_exp_list();

            Tree parse_let_statement();

            Tree parse_do_statement();

            Tree parse_return_statement();

            Tree parse_if_statement();

            Tree parse_while_statement();

            Tree parse_var_dec();

        private:

            Tree parse_statement();

            Tree parse_statements();

            Tokenizer tokenizer;

    };
}

#endif
