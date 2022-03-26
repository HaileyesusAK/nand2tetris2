#include <sstream>
#include "statement_factory.hpp"
#include "subroutine_body.hpp"

namespace ntt {

    /* subroutineBody : '{' varDec* statements '}' */
    SubroutineBody::SubroutineBody(Tokenizer& tokenizer)
        : left_brace_(tokenizer.consume_symbol("{")),
          var_decs_(SubroutineBody::parse_var_decs_(tokenizer)),
          statements_(SubroutineBody::parse_statements_(tokenizer)),
          right_brace_(tokenizer.consume_symbol("}"))
    {}

    std::string SubroutineBody::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<subroutineBody>", level);
        oss << JackFragment::to_xml(left_brace_, level + 1);
        for(const auto& var_dec : var_decs_)
            oss << var_dec.to_xml(level + 1);

        oss << JackFragment::get_line("<statements>", level + 1);
        for(const auto& statement : statements_)
            oss << statement->to_xml(level + 2);
        oss << JackFragment::get_line("</statements>", level + 1);
        oss << JackFragment::to_xml(right_brace_, level + 1);

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

            /* variable declarations end when statements begin or end of subroutine body is reached */
            if(STATEMENT_PREFIXES.count(token.value()) || token.value() == "}")
                break;

            var_decs.emplace_back(SubroutineVarDec(tokenizer));
        }

        return var_decs;
    }

    SubroutineBody::Statements SubroutineBody::parse_statements_(Tokenizer& tokenizer) {
        Statements statements;
        //parse statements until the end of the subroutine's body
        while(tokenizer.has_token() && tokenizer.peek().value() != "}")
            statements.emplace_back(StatementFactory::parse(tokenizer));

        return statements;
    }

}
