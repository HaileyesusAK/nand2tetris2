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

        private:
            struct TrailingVariable {
                Token comma;
                Token var_name;

                TrailingVariable(Tokenizer&);
            };
            using TrailingVariables = std::vector<TrailingVariable>;

            Token type_;
            Token first_var_name_;
            TrailingVariables trailing_var_names_;
            Token semicolon_;

            static TrailingVariables parse_trailing_variables_(Tokenizer&);
    };
}

#endif
