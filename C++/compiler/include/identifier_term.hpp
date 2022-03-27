#ifndef __IDENTIFIER_TERM_H__
#define __IDENTIFIER_TERM_H__

#include <string>
#include "term.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {

    /* represents an identifer term */
    class IdentifierTerm : public Term {
        public:
            IdentifierTerm(Tokenizer& tokenizer);

            std::string to_xml(size_t level = 0) const override;

            Type get_type() const override;

        private:
            Token token_;
    };
}

#endif
