#include <fstream>
#include <istream>
#include <string>
#include "token.hpp"

namespace ntt {

    bool Token::is_symbol(char c) { return symbols_.count(c) != 0; }

    TokenType Token::type() const { return type_; }

    std::string Token::value() const { return value_; }

	Token Token::parse(std::ifstream& ifs) {
        if(!ifs.good())
            return Token {""};

        std::ws(ifs);   
        if(!ifs.good())
            return Token {""};

        char c = ifs.get();
        if(Token::is_symbol(c)) {
            // if a single line comment, consume all characters until the end of the line
            if(c == '/' && ifs.peek() == '/') {
                ifs.get();
                while(ifs.good() && ifs.get() != '\n');

                c = ifs.get();
                if(ifs.good())
                    return Token {std::string {c}, TokenType::SYMBOL};
            }
            else
                return Token {std::string {c}, TokenType::SYMBOL};
        }

        return Token {""};
    }
}
