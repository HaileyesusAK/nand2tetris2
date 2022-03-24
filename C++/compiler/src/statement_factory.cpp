#include <string>
#include <unordered_map>
#include "do_statement.hpp"
#include "if_statement.hpp"
#include "let_statement.hpp"
#include "return_statement.hpp"
#include "statement_factory.hpp"
#include "tokenizer.hpp"
#include "while_statement.hpp"

namespace ntt {

    /*
        statement  : letStatement | ifStatement | whileStatement | doStatement | returnStatement
    */
    std::unique_ptr<Statement> StatementFactory::parse(Tokenizer& tokenizer) {

        const static std::unordered_map<std::string, StatementID> statement_map {
            {"do", StatementID::DO},
            {"if", StatementID::IF},
            {"let", StatementID::LET},
            {"return", StatementID::RETURN},
            {"while", StatementID::WHILE}
        };

        if(!tokenizer.has_token())
            return nullptr;

        try {
            switch(statement_map.at(tokenizer.peek().value())) {
                case StatementID::DO: return std::make_unique<DoStatement>(tokenizer);
                case StatementID::LET: return std::make_unique<LetStatement>(tokenizer);
                case StatementID::RETURN: return std::make_unique<ReturnStatement>(tokenizer);
                case StatementID::WHILE: return std::make_unique<WhileStatement>(tokenizer);
                case StatementID::IF: return std::make_unique<IfStatement>(tokenizer);
            }
        }
        catch(std::out_of_range&) {
            return nullptr;
        }

        return nullptr;
    }
}
