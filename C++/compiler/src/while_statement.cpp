#include <sstream>
#include "statement_factory.hpp"
#include "while_statement.hpp"

namespace ntt {

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
        oss << while_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl; 
        oss << left_parenthesis_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl; 
        oss << expression_.to_xml(level + 1);
        oss << right_parenthesis_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl; 
        oss << left_brace_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl; 

        oss << JackFragment::get_line("<statements>", level + 1);
        for(auto& statement : statements_)
            oss << statement->to_xml(level + 2);
        oss << JackFragment::get_line("</statements>", level + 1);

        oss << right_brace_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl; 
        oss << JackFragment::get_line("</whileStatement>", level);

        return oss.str();
    }

}
