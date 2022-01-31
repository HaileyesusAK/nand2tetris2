#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <unordered_set>
#include "token.hpp"
#include "tokenizer.hpp"

#include <iostream>

namespace ntt {

    Tokenizer::Tokenizer(std::ifstream& ifs) {
        if(ifs.eof())
            throw NoTokenErr();

        while(ifs.peek() != EOF) {
            Tokenizer::remove_leading_ws(ifs);
            if(ifs.eof())
                break;

            char c = ifs.get();
            if(Tokenizer::is_symbol(c)) {
                tokens_.emplace_back(std::string {c}, TokenType::SYMBOL);
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

                if(!ifs.good() || has_new_line)
                    throw std::domain_error("invalid string");

                tokens_.emplace_back(token, TokenType::STRING);
            }
            else {
                ifs.putback(c);

                std::string token;

                /* read all characters until a whitespace or a symbol that is not an underscore is found */
                auto is_valid = [](char c) {
                    return (!std::isspace(c) && (c == '_' || !Tokenizer::is_symbol(c)));
                };
                while((c=ifs.get()) && ifs.good() && is_valid(c))
                    token.push_back(c);

                // putback the last invalid character consumed
                ifs.putback(c);        

                if(Tokenizer::is_keyword(token))
                    tokens_.emplace_back(token, TokenType::KEYWORD);
                else if(Tokenizer::is_identifier(token))
                    tokens_.emplace_back(token, TokenType::IDENTIFIER);
                else if(Tokenizer::is_integer(token))
                    tokens_.emplace_back(token, TokenType::INTEGER);
                else
                    throw std::domain_error("invalid string");
            }
        }
    }

    bool Tokenizer::is_symbol(char token) { return symbols_.count(token) != 0; }

    bool Tokenizer::is_keyword(const std::string& token) { return keywords_.count(token) != 0; }

    bool Tokenizer::is_identifier(const std::string& token) {
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

    bool Tokenizer::is_integer(const std::string& token) {
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

    void Tokenizer::remove_leading_ws(std::ifstream& ifs) {
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

    Token Tokenizer::get() {
        auto token = tokens_.front();
        tokens_.pop_front();
        return token;
    }

    void Tokenizer::put(const Token& token) {
        tokens_.push_front(token);
    }
    
    const Token& Tokenizer::peek() { 
        if(tokens_.empty())
            throw std::domain_error("empty token stream");

        return tokens_.front();
    }

    bool Tokenizer::has_token() {
        return !tokens_.empty();
    }

    Token Tokenizer::consume_identifier() {
        if(tokens_.empty())
            throw std::domain_error("empty token stream");

        auto token = tokens_.front();
        if(token.type() != TokenType::IDENTIFIER)
            throw std::domain_error("token is not an identifier");

        tokens_.pop_front();
        return token;
    }
    
    Token Tokenizer::consume_keyword(const std::unordered_set<std::string>& values) {
        if(tokens_.empty())
            throw std::domain_error("empty token stream");

        auto token = tokens_.front();
        if(token.type() != TokenType::KEYWORD)
            throw std::domain_error("token is not a keyword");

        if(!values.empty() && !values.count(token.value()))
            throw std::domain_error("unexpected keyword");
            
        tokens_.pop_front();
        return token;
    }

    Token Tokenizer::consume_type() {
        static const std::unordered_set<std::string> builtin_types { "int", "char", "boolean" };
        if(tokens_.empty())
            throw std::domain_error("empty token stream");

        auto token = tokens_.front();
        if(token.type() != TokenType::KEYWORD && token.type() != TokenType::IDENTIFIER)
            throw std::domain_error("token is not a type");

        if(token.type() == TokenType::KEYWORD && !builtin_types.count(token.value()))
            throw std::domain_error("token is not a type");

        tokens_.pop_front();
        return token;
    }

    Token Tokenizer::consume_symbol(const std::string& value) {
        if(tokens_.empty())
            throw std::domain_error("empty token stream");

        auto token = tokens_.front();
        if(token.type() != TokenType::SYMBOL)
            throw std::domain_error("token is not a symbol");

        if(token.value() != value)
            throw std::domain_error("unexpected symbol");
            
        tokens_.pop_front();
        return token;
    }

    void Tokenizer::to_xml(std::ofstream& ofs) {
        if(!ofs.is_open())
            throw std::runtime_error("file is not open");

        ofs << "<tokens>" << std::endl;
        while(has_token()) {
            auto token = get();
            ofs << token.to_xml() << std::endl;
        }
        ofs << "</tokens>" << std::endl;
    }
}
