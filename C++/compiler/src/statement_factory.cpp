#include <string>
#include <unordered_map>
#include "statement_factory.hpp"
#include "token.hpp"

namespace ntt {

    std::unique_ptr<Statement> StatementFactory::parse(Tokenizer& tokenizer) {

        const static std::unordered_map<std::string, StatementID> statement_map {
            {"do", StatementID::DO},
            {"let", StatementID::LET},
            {"return", StatementID::RETURN}
        };

        if(!tokenizer.has_token())
            return nullptr;

        try {
            switch(statement_map.at(tokenizer.peek().value())) {
                case StatementID::DO: return std::make_unique<DoStatement>(tokenizer);
                case StatementID::LET: return std::make_unique<LetStatement>(tokenizer);
                case StatementID::RETURN: return std::make_unique<ReturnStatement>(tokenizer);
            }
        }
        catch(std::out_of_range&) {
            return nullptr;
        }

        return nullptr;
    }
}
