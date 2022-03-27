#ifndef __STRING_TERM_H__
#define __STRING_TERM_H__

#include <string>
#include "term.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {

    /* represents a string constant term */
    class StringTerm : public Term {
        public:
            StringTerm(Tokenizer& tokenizer);

            std::string to_xml(size_t level = 0) const override;

            Type get_type() const override;

        private:
            Token token_;
    };
}

#endif
