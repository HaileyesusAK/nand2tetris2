#include <sstream>
#include "statement_factory.hpp"
#include "if_statement.hpp"

namespace ntt {

    IfStatement::ElsePart::ElsePart(Tokenizer& tokenizer)
        : else_keyword(tokenizer.consume_keyword({"else"})),
          left_parenthesis(tokenizer.consume_symbol("{")),
          statements(IfStatement::parse_statements_(tokenizer)),
          right_parenthesis(tokenizer.consume_symbol("}"))
          {}

    /*
    ifStatement : 'if' '(' expression ')' '{' statements '}'
                  ('else' '{' statements '}')?
    */
    IfStatement::IfStatement(Tokenizer& tokenizer)
        : if_(tokenizer.consume_keyword({"if"})),
          left_parenthesis_(tokenizer.consume_symbol("(")),
          expression_(Expression(tokenizer)),
          right_parenthesis_(tokenizer.consume_symbol(")")),
          left_brace_(tokenizer.consume_symbol("{")),
          statements_(IfStatement::parse_statements_(tokenizer)),
          right_brace_(tokenizer.consume_symbol("}")),
          else_(IfStatement::parse_else_part_(tokenizer))
    {}

    IfStatement::StatementList IfStatement::parse_statements_(Tokenizer& tokenizer) {
        StatementList statements;

        while(tokenizer.has_token() && tokenizer.peek().value() != "}")
            statements.emplace_back(StatementFactory::parse(tokenizer));

        return statements;
    }

    void IfStatement::statements_to_xml_(std::ostringstream& oss, const StatementList& statements, size_t level) {
        oss << JackFragment::get_line("<statements>", level);
        for(auto& statement : statements)
            oss << statement->to_xml(level + 1);
        oss << JackFragment::get_line("</statements>", level);
    }

    std::optional<IfStatement::ElsePart> IfStatement::parse_else_part_(Tokenizer& tokenizer) {
        if(tokenizer.has_token() && tokenizer.peek().value() == "else") {
            return ElsePart(tokenizer);
        }

        return std::nullopt;
    }

    std::string IfStatement::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<ifStatement>", level);
        oss << JackFragment::to_xml(if_, level + 1);
        oss << JackFragment::to_xml(left_parenthesis_, level + 1);
        oss << expression_.to_xml(level + 1);
        oss << JackFragment::to_xml(right_parenthesis_, level + 1);
        oss << JackFragment::to_xml(left_brace_, level + 1);
        if(!statements_.empty())
            IfStatement::statements_to_xml_(oss, statements_, level + 1);
        oss << JackFragment::to_xml(right_brace_, level + 1);

        if(else_.has_value()) {
            const auto& else_part = else_.value();
            oss << JackFragment::to_xml(else_part.else_keyword, level + 1);
            oss << JackFragment::to_xml(else_part.left_parenthesis, level + 1);
            IfStatement::statements_to_xml_(oss, else_part.statements, level + 1);
            oss << JackFragment::to_xml(else_part.right_parenthesis, level + 1);
        }

        oss << JackFragment::get_line("</ifStatement>", level);

        return oss.str();
    }

    Statement::Type IfStatement::type() const {
        return Statement::Type::IF;
    }

}
