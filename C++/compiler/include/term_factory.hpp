#ifndef __TERM_FACTORY_H__
#define __TERM_FACTORY_H__

#include <memory>
#include "term.hpp"
#include "tokenizer.hpp"

namespace ntt {

    /* a factory method to generate jack terms */
    class TermFactory {
        public:
            static std::unique_ptr<Term> parse(Tokenizer&);
    };
}

#endif
