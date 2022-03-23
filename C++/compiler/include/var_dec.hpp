#ifndef __VAR_DEC_H__
#define __VAR_DEC_H__

#include <string>
#include <utility>
#include <vector>
#include "jack_fragment.hpp"
#include "tokenizer.hpp"
#include "token.hpp"

namespace ntt {

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

            Token type_;

            Token first_var_name_;

            using TrailingVariables = std::vector<TrailingVariable>;

            TrailingVariables trailing_var_names_;

            Token semicolon_;

            static TrailingVariables parse_trailing_variables_(Tokenizer&);
    };
}

#endif
