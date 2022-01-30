#include <fstream>
#include <stdexcept>
#include <unordered_set>
#include "token.hpp"
#include "tokenizer.hpp"

#include <iostream>
namespace ntt {

    Tokenizer::Tokenizer(std::ifstream& ifs) {
        while(true) {
            try {
                auto token = Token::parse(ifs);
                tokens_.push_back(token);
            }
            catch(NoTokenErr&) {
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
