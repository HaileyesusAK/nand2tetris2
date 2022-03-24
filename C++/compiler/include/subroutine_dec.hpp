#ifndef __SUBROUTINE_DEC_H__
#define __SUBROUTINE_DEC_H__

#include <string>
#include "jack_fragment.hpp"
#include "parameter_list.hpp"
#include "subroutine_body.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {

    /* represents a subroutine declaration */
    class SubroutineDec : public JackFragment {
        public:
            SubroutineDec(Tokenizer&);

            std::string to_xml(size_t level = 0) const override;

        private:
            Token routine_type_;
            Token return_type_;
            Token name_;
            Token left_parenthesis_;
            ParameterList parameter_list_;
            Token right_parenthesis_;
            SubroutineBody body_;

            static Token get_return_type_(Tokenizer&);
    };
}

#endif
