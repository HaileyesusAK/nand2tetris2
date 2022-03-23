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

    void IfStatement::statements_to_xml(std::ostringstream& oss, const StatementList& statements, size_t level) {
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
        oss << if_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << left_parenthesis_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << expression_.to_xml(level + 1);
        oss << right_parenthesis_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << left_brace_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        if(!statements_.empty())
            IfStatement::statements_to_xml(oss, statements_, level + 1);
        oss << right_brace_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;

        if(else_.has_value()) {
            const auto& else_part = else_.value();

            oss << else_part.else_keyword .to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
            oss << else_part.left_parenthesis.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
            IfStatement::statements_to_xml(oss, else_part.statements, level + 1);
            oss << else_part.right_parenthesis.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        }

        oss << JackFragment::get_line("</ifStatement>", level);

        return oss.str();
    }

}
