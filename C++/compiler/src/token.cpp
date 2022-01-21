#include <fstream>
#include <string>
#include "token.hpp"

namespace ntt {

    bool Token::is_symbol(char c) { return symbols_.count(c) != 0; }

    TokenType Token::type() const { return type_; }

    std::string Token::value() const { return value_; }

	Token Token::parse(std::ifstream& ifs) {
        if(ifs.good()) {
            char c = ifs.get();
            if(Token::is_symbol(c)) {
                return Token {std::string {c}, TokenType::SYMBOL};
            }
        }
        return Token {""};
    }
}
