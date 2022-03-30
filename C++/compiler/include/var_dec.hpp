#ifndef __VAR_DEC_H__
#define __VAR_DEC_H__

#include <string>
#include <utility>
#include <vector>
#include "jack_fragment.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {

    /* represent a generic variable declartion */
    class VarDec : public JackFragment {
        public:
            VarDec(Tokenizer&);

            std::string to_xml(size_t level = 0) const override;

            const Token& type() const;

            const std::vector<Token>& names() const;

        private:
            Token type_;
            std::vector<Token> names_;
            std::vector<Token> commas_;
            Token semicolon_;
    };
}

#endif
