#ifndef __SUBROUTINE_VAR_DEC_H__
#define __SUBROUTINE_VAR_DEC_H__

#include <string>
#include "jack_fragment.hpp"
#include "token.hpp"
#include "tokenizer.hpp"
#include "var_dec.hpp"

namespace ntt {

    /* represents subroutine-level variable declaration */
    class SubroutineVarDec : JackFragment {
        public:
            SubroutineVarDec(Tokenizer&);

            std::string to_xml(size_t level = 0) const override;

        private:
            Token var_;
            VarDec var_dec_;
    };
}

#endif
