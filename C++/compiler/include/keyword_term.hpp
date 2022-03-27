#ifndef __KEYWORD_TERM_H__
#define __KEYWORD_TERM_H__

#include <string>
#include "term.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {

    /* represents keyword constant term */
    class KeywordTerm : public Term {
        public:
            KeywordTerm(Tokenizer& tokenizer);

            std::string to_xml(size_t level = 0) const override;

            Type get_type() const override;

        private:
            Token token_;
    };
}

#endif
