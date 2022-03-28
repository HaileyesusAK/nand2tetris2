#include <sstream>
#include "parameter_list.hpp"

namespace ntt {

    ParameterList::Parameter::Parameter(Tokenizer& tokenizer)
        : type(tokenizer.consume_type()),
          var_name(tokenizer.consume_identifier())
    {}

    std::string ParameterList::Parameter::to_xml(size_t level) const {
        std::ostringstream oss;
        oss << JackFragment::to_xml(type, level);
        oss << JackFragment::to_xml(var_name, level);
        return oss.str();
    }

    ParameterList::TrailingParameter::TrailingParameter(Tokenizer& tokenizer)
        : comma(tokenizer.consume_symbol(",")),
          parameter(Parameter(tokenizer))
    {}

    std::string ParameterList::TrailingParameter::to_xml(size_t level) const {
        std::ostringstream oss;
        oss << JackFragment::to_xml(comma, level);
        oss << parameter.to_xml(level);
        return oss.str();
    }

    /* parameterList : ((type varName)(',' type varName)*)? */
    ParameterList::ParameterList(Tokenizer& tokenizer) {
        if(!tokenizer.has_token())
            throw NoTokenErr();

        /*
            parameter list is defined in the context of function definition;
            therefore, continue checking the token stream until ')' is encountered.
        */
        if(tokenizer.peek().value() != ")") {
            parameter_list_ = std::make_pair<Parameter, std::vector<TrailingParameter>>(Parameter(tokenizer), {});

            auto& trailing_parameters = parameter_list_.value().second;
            while(tokenizer.has_token() && tokenizer.peek().value() == ",")
                trailing_parameters.emplace_back(TrailingParameter(tokenizer));
        }
    }

    std::string ParameterList::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<parameterList>", level);
        if(parameter_list_.has_value()) {
            oss << parameter_list_.value().first.to_xml(level + 1);
            for(const auto& trailing_parameter : parameter_list_.value().second) {
                oss << trailing_parameter.to_xml(level + 1);
            }
        }
        oss << JackFragment::get_line("</parameterList>", level);

        return oss.str();
    }

    std::vector<std::pair<Token, Token>> ParameterList::parameters() const {
        std::vector<std::pair<Token, Token>> parameters;
        if(parameter_list_.has_value()) {
            const auto& first_parameter = parameter_list_.value().first;
            parameters.emplace_back(first_parameter.type, first_parameter.var_name);

            for(const auto& trailing_parameter : parameter_list_.value().second)
                parameters.emplace_back(trailing_parameter.parameter.type, trailing_parameter.parameter.var_name);
        }

        return parameters;
    }

}
