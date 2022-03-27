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

        const static std::unordered_map<std::string, Statement::Type> statement_map {
            {"do", Statement::Type::DO},
            {"if", Statement::Type::IF},
            {"let", Statement::Type::LET},
            {"return", Statement::Type::RETURN},
            {"while", Statement::Type::WHILE}
        };

        if(!tokenizer.has_token())
            return nullptr;

        try {
            switch(statement_map.at(tokenizer.peek().value())) {
                case Statement::Type::DO: return std::make_unique<DoStatement>(tokenizer);
                case Statement::Type::LET: return std::make_unique<LetStatement>(tokenizer);
                case Statement::Type::RETURN: return std::make_unique<ReturnStatement>(tokenizer);
                case Statement::Type::WHILE: return std::make_unique<WhileStatement>(tokenizer);
                case Statement::Type::IF: return std::make_unique<IfStatement>(tokenizer);
            }
        }
        catch(std::out_of_range&) {
            return nullptr;
        }

        return nullptr;
    }
}
