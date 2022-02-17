#ifndef __pARSER_H__
#define __pARSER_H__

#include <fstream>
#include <memory>
#include "term.hpp"
#include "tokenizer.hpp"

namespace ntt {

    class parser {

        public:
            parser(std::ifstream& ifs) : tokenizer_(ifs) {}

            std::unique_ptr<Term> parse_term();

        private:

            Tokenizer tokenizer_;

    };
}

#endif
