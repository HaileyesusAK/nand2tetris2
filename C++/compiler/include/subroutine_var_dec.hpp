#ifndef __SUBROUTINE_VAR_DEC_H__
#define __SUBROUTINE_VAR_DEC_H__

#include <string>
#include <vector>
#include "jack_fragment.hpp"
#include "token.hpp"
#include "tokenizer.hpp"
#include "var_dec.hpp"

namespace ntt {

    /* represents subroutine-level variable declaration */
    class SubroutineVarDec : public JackFragment {
        public:
            SubroutineVarDec(Tokenizer&);

            std::string to_xml(size_t level = 0) const override;

            const Token& type() const;

            const std::vector<Token>& names() const;

        private:
            Token var_;
            VarDec var_dec_;
    };
}

#endif
