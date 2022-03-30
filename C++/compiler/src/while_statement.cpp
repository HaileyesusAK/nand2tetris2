#include <sstream>
#include "statement_factory.hpp"
#include "while_statement.hpp"

namespace ntt {

    /*
       whileStatement : 'while' '(' expression ')' '{' statements '}'
    */
    WhileStatement::WhileStatement(Tokenizer& tokenizer)
        : while_(tokenizer.consume_keyword({"while"})),
          left_parenthesis_(tokenizer.consume_symbol("(")),
          expression_(Expression(tokenizer)),
          right_parenthesis_(tokenizer.consume_symbol(")")),
          left_brace_(tokenizer.consume_symbol("{")),
          statements_(WhileStatement::parse_statements_(tokenizer)),
          right_brace_(tokenizer.consume_symbol("}"))
    {}

    WhileStatement::StatementList WhileStatement::parse_statements_(Tokenizer& tokenizer) {
        StatementList statements;

        while(tokenizer.has_token() && tokenizer.peek().value() != "}")
            statements.emplace_back(StatementFactory::parse(tokenizer));

        return statements;
    }

    std::string WhileStatement::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<whileStatement>", level);
        oss << JackFragment::to_xml(while_, level + 1);
        oss << JackFragment::to_xml(left_parenthesis_, level + 1);
        oss << expression_.to_xml(level + 1);
        oss << JackFragment::to_xml(right_parenthesis_, level + 1);
        oss << JackFragment::to_xml(left_brace_, level + 1);

        oss << JackFragment::get_line("<statements>", level + 1);
        for(auto& statement : statements_)
            oss << statement->to_xml(level + 2);
        oss << JackFragment::get_line("</statements>", level + 1);

        oss << JackFragment::to_xml(right_brace_, level + 1);
        oss << JackFragment::get_line("</whileStatement>", level);

        return oss.str();
    }

    const Expression& WhileStatement::expression() const {
        return expression_;
    }

    const WhileStatement::StatementList& WhileStatement::statements() const {
        return statements_;
    }

    Statement::Type WhileStatement::type() const {
        return Statement::Type::WHILE;
    }

}
