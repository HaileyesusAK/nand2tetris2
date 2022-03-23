#include <sstream>
#include "statement_factory.hpp"
#include "subroutine_body.hpp"

namespace ntt {

    SubroutineBody::SubroutineBody(Tokenizer& tokenizer)
        : left_brace_(tokenizer.consume_symbol("{")),
          var_decs_(SubroutineBody::parse_var_decs_(tokenizer)),
          statements_(SubroutineBody::parse_statements_(tokenizer)),
          right_brace_(tokenizer.consume_symbol("}"))
    {}


    std::string SubroutineBody::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<subroutineBody>", level);
        oss << left_brace_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        for(const auto& var_dec : var_decs_)
            oss << var_dec.to_xml(level + 1);

        oss << JackFragment::get_line("<statements>", level + 1);
        for(const auto& statement : statements_)
            oss << statement->to_xml(level + 2);
        oss << JackFragment::get_line("</statements>", level + 1);
        oss << right_brace_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;

        oss << JackFragment::get_line("</subroutineBody>", level);

        return oss.str();
    }

    std::vector<SubroutineVarDec> SubroutineBody::parse_var_decs_(Tokenizer& tokenizer) {
        static const std::unordered_set<std::string> STATEMENT_PREFIXES {
            "do", "if", "let", "return", "while"
        };

        std::vector<SubroutineVarDec> var_decs;
        while(tokenizer.has_token()) {
            const auto& token = tokenizer.peek();
            if(STATEMENT_PREFIXES.count(token.value()) || token.value() == "}")
                break;

            var_decs.emplace_back(SubroutineVarDec(tokenizer));
        }

        return var_decs;
    }

    SubroutineBody::Statements SubroutineBody::parse_statements_(Tokenizer& tokenizer) {
        Statements statements;
        while(tokenizer.has_token() && tokenizer.peek().value() != "}")
            statements.emplace_back(StatementFactory::parse(tokenizer));

        return statements;
    }

}
