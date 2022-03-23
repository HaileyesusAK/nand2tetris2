#ifndef __CLASS_H__
#define __CLASS_H__

#include <string>
#include <vector>
#include "class_var_dec.hpp"
#include "jack_fragment.hpp"
#include "subroutine_dec.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {
    
    class Class : public JackFragment {
        public:
            Class(Tokenizer&);

            std::string to_xml(size_t level = 0) const override;

        private:
            Token class_;

            Token name_;

            Token left_brace_;

            std::vector<ClassVarDec> class_var_dec_;

            std::vector<SubroutineDec> subroutine_dec_;

            Token right_brace_;

            static std::vector<ClassVarDec> parse_class_var_dec_(Tokenizer&);

            static std::vector<SubroutineDec> parse_subroutine_dec_(Tokenizer&);
    };
}

#endif
