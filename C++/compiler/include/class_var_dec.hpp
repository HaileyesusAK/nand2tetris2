#ifndef __CLASS_VAR_DEC_H__
#define __CLASS_VAR_DEC_H__

#include <string>
#include "jack_fragment.hpp"
#include "token.hpp"
#include "tokenizer.hpp"
#include "var_dec.hpp"

namespace ntt {

    class ClassVarDec : JackFragment {

        public:
            ClassVarDec(Tokenizer&); 

            std::string to_xml(size_t level = 0) const override; 

        private:
            Token storage_class_;

            VarDec var_dec_;
    };
}

#endif
