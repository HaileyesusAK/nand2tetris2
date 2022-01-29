#include <fstream>
#include <stdexcept>
#include "token.hpp"
#include "tokenizer.hpp"

#include <iostream>
namespace ntt {

    Tokenizer::Tokenizer(std::ifstream& ifs) {
        while(true) {
            try {
                auto token = Token::parse(ifs);
                tokens_.push(token);
            }
            catch(NoTokenErr&) {
                break;
            }
        }
    }

    Token Tokenizer::get() {
        auto token = tokens_.front();
        tokens_.pop();
        return token;
    }
    
    const Token& Tokenizer::peek() { 
        if(tokens_.empty())
            throw std::domain_error("empty token stream");

        return tokens_.front();
    }

    bool Tokenizer::has_token() {
        return !tokens_.empty();
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
