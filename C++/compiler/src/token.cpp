#include <algorithm>
#include <cctype>
#include <fstream>
#include <istream>
#include <string>
#include "token.hpp"

namespace ntt {

    bool Token::is_symbol(char c) { return symbols_.count(c) != 0; }

    bool Token::is_keyword(const std::string& word) { return keywords_.count(word) != 0; }

    bool Token::is_identifier(const std::string& word) {
        /*
            An identifier is a sequence of letters, digits, and underscore ('_')
            not starting with a digit.
        */
        
        if(word.empty())
            return false;

        if(std::isdigit(word.front()))
            return false;

        auto is_valid = [](unsigned char c) { return std::isalnum(c) || c == '_'; };
        return std::all_of(word.begin(), word.end(), is_valid); 
    }

    TokenType Token::type() const { return type_; }

    std::string Token::value() const { return value_; }

    void Token::remove_leading_ws(std::ifstream& ifs) {
        while(true) {
            std::ws(ifs);   
            if(!ifs.good())
                return;
       
            char c = ifs.get();
            if(c == '/' && ifs.peek() == '/')
                while(ifs.good() && ifs.get() != '\n');
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
        else {
            ifs.putback(c);
			std::string word;
			ifs >> word;

			if(Token::is_keyword(word))
				return Token {word, TokenType::KEYWORD};
			else if(Token::is_identifier(word))
				return Token {word, TokenType::IDENTIFIER};

        }

        return Token {""};
    }
}
