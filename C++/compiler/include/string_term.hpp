#ifndef __STRING_TERM_H__
#define __STRING_TERM_H__

#include <string>
#include "term.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {

    class StringTerm : public Term {
        public:
            StringTerm(Tokenizer& tokenizer);

            std::string to_xml() const override;

        private:
            Token token_;
    };
}

#endif
