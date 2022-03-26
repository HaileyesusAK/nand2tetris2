#ifndef __INTEGER_TERM_H__
#define __INTEGER_TERM_H__

#include <string>
#include "term.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {
    
    /* represents integer constant term */
    class IntegerTerm : public Term {
        public:
            IntegerTerm(Tokenizer& tokenizer);

            std::string to_xml(size_t level = 0) const override;

        private:
            Token token_;
    };
}

#endif
