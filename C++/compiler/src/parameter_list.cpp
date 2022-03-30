#include <sstream>
#include "parameter_list.hpp"

namespace ntt {

    ParameterList::Parameter::Parameter(Tokenizer& tokenizer)
        : type(tokenizer.consume_type()),
          name(tokenizer.consume_identifier())
    {}

    /* parameterList : ((type varName)(',' type varName)*)? */
    ParameterList::ParameterList(Tokenizer& tokenizer) {
        if(!tokenizer.has_token())
            throw NoTokenErr();

        /*
            parameter list is defined in the context of function definition;
            therefore, continue checking the token stream until ')' is encountered.
        */
        if(tokenizer.peek().value() != ")") {
            parameter_list_.emplace_back(Parameter(tokenizer));
            while(tokenizer.has_token() && tokenizer.peek().value() == ",") {
                commas_.emplace_back(tokenizer.consume_symbol(","));
                parameter_list_.emplace_back(Parameter(tokenizer));
            }
        }
    }

    std::string ParameterList::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<parameterList>", level);
        if(!parameter_list_.empty()) {
            oss << JackFragment::to_xml(parameter_list_[0].type, level + 1);
            oss << JackFragment::to_xml(parameter_list_[0].name, level + 1);

            for(size_t i = 0; i < commas_.size(); ++i) {
                oss << JackFragment::to_xml(commas_[i], level + 1);
                oss << JackFragment::to_xml(parameter_list_[i + 1].type, level + 1);
                oss << JackFragment::to_xml(parameter_list_[i + 1].name, level + 1);
            }
        }
        oss << JackFragment::get_line("</parameterList>", level);

        return oss.str();
    }

    const std::vector<ParameterList::Parameter>& ParameterList::parameters() const {
        return parameter_list_;
    }

}
