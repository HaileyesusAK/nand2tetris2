#include <sstream>
#include "parameter_list.hpp"

namespace ntt {

    ParameterList::Parameter::Parameter(Tokenizer& tokenizer)
        : type(tokenizer.consume_type()),
          var_name(tokenizer.consume_identifier())
    {}

    std::string ParameterList::Parameter::to_xml(size_t level) const {
        std::ostringstream oss;
        oss << type.to_xml(level, JackFragment::TAB_WIDTH_) << std::endl;
        oss << var_name.to_xml(level, JackFragment::TAB_WIDTH_) << std::endl;
        return oss.str();
    }

    ParameterList::TrailingParameter::TrailingParameter(Tokenizer& tokenizer)
        : comma(tokenizer.consume_symbol(",")),
          parameter(Parameter(tokenizer))
    {}

    std::string ParameterList::TrailingParameter::to_xml(size_t level) const {
        std::ostringstream oss;
        oss << comma.to_xml(level, JackFragment::TAB_WIDTH_) << std::endl;
        oss << parameter.to_xml(level);
        return oss.str();
    }

    ParameterList::ParameterList(Tokenizer& tokenizer) {
        if(!tokenizer.has_token())
            throw NoTokenErr();

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

}
