#ifndef __PARAMETER_LIST_H__
#define __PARAMETER_LIST_H__

#include <optional>
#include <string>
#include <tuple>
#include <vector>
#include "jack_fragment.hpp"
#include "tokenizer.hpp"
#include "token.hpp"

namespace ntt {

    /* represents subroutine parameter list */
    class ParameterList : public JackFragment {
        public:
            ParameterList(Tokenizer&);

            std::string to_xml(size_t level = 0) const override;

            struct Parameter {
                Token type;
                Token name;

                Parameter(Tokenizer&);
            };
            
            const std::vector<Parameter>& parameters() const;

        private:
            std::vector<Parameter> parameter_list_;
            std::vector<Token> commas_;
    };
}

#endif
