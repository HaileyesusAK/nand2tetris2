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

        private:
            Tokenizer tokenizer;

    };
}

#endif
