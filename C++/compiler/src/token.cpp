#include <algorithm>
#include <cctype>
#include <fstream>
#include <istream>
#include <stdexcept>
#include <string>
#include "token.hpp"

namespace ntt {

    bool Token::is_symbol(char token) { return symbols_.count(token) != 0; }

    bool Token::is_keyword(const std::string& token) { return keywords_.count(token) != 0; }

    bool Token::is_identifier(const std::string& token) {
        /*
            An identifier is a sequence of letters, digits, and underscore ('_')
            not starting with a digit.
        */

        if(token.empty())
            return false;

        if(std::isdigit(token.front()))
            return false;

        auto is_valid = [](unsigned char c) { return std::isalnum(c) || c == '_'; };
        return std::all_of(token.begin(), token.end(), is_valid);
    }

    bool Token::is_integer(const std::string& token) {
        /* An integer is a decimal number in the range 0 .. 32767 */

        if(token.empty())
            return false;

        try {
            std::size_t pos;
            auto n = std::stoul(token, &pos);
            return n <= 32767 && pos == token.size();
        }
        catch(const std::exception&) {
            return false;
        }
    }

    TokenType Token::type() const { return type_; }

    std::string Token::value() const { return value_; }

    void Token::remove_leading_ws(std::ifstream& ifs) {
        while(true) {
            std::ws(ifs);
            if(!ifs.good())
                return;

            char c = ifs.get();
            if(c == '/' && ifs.peek() == '/') {
                while(ifs.good() && ifs.get() != '\n');
            }
            else if(c == '/' && ifs.peek() == '*') {
                ifs.get(); // consume '*'
                while(ifs.good() && !(ifs.get() == '*' && ifs.peek() == '/'));
                ifs.get(); // consume '/'
            }
            else {
                ifs.putback(c);
                break;
            }
        }
    }

	Token Token::parse(std::ifstream& ifs) {
        Token::remove_leading_ws(ifs);
        if(!ifs.good())
            return Token {""};

        char c = ifs.get();
        if(Token::is_symbol(c)) {
            return Token {std::string {c}, TokenType::SYMBOL};
        }
        else if(c == '"') {
            /*
                A string is a sequence of zero or more characters surrounded by double quotes.
                It cannot contain a newline character.
            */

            std::string token;
            bool has_new_line = false;
            while(ifs.good() && !has_new_line) {
                c = ifs.get();
                if(c == '"')
                    break;

                token.push_back(c);
                has_new_line = (c == '\n');
            }

            if(ifs.good() && !has_new_line)
              return Token {token, TokenType::STRING};
        }
        else {
            ifs.putback(c);
			std::string token;
			ifs >> token;

			if(Token::is_keyword(token))
				return Token {token, TokenType::KEYWORD};
			else if(Token::is_identifier(token))
				return Token {token, TokenType::IDENTIFIER};
            else if(Token::is_integer(token))
				return Token {token, TokenType::INTEGER};
        }

        return Token {""};
    }
}
