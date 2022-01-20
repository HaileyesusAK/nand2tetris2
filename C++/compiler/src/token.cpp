#include <fstream>
#include <string>
#include "token.hpp"

namespace ntt {

    TokenType Token::type() const { return type_; }

    std::string Token::value() const { return value_; }

	Token Token::parse(std::ifstream& ifs) {
        return Token {""};
    }
}
