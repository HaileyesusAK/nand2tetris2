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

            std::vector<std::pair<Token, Token>> parameters() const;

        private:
            struct Parameter {
                Token type;
                Token var_name;

                Parameter(Tokenizer&);
                std::string to_xml(size_t level) const;
            };

            struct TrailingParameter {
                Token comma;
                Parameter parameter;

                TrailingParameter(Tokenizer&);
                std::string to_xml(size_t level) const;
            };

            using Parameters = std::optional<std::pair<Parameter, std::vector<TrailingParameter>>>;
            Parameters parameter_list_ = std::nullopt;
    };
}

#endif
