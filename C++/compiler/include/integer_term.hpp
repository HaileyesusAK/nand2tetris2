#ifndef __INTEGER_TERM_H__
#define __INTEGER_TERM_H__

#include <string>
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {

    class IntegerTerm {
        public:
            IntegerTerm(Tokenizer& tokenizer);

            std::string to_xml() const;

        private:
            Token token_;
    };
}

#endif
