#include <fstream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {

    Token Tokenizer::get() {
        if(tokens_.empty())
            tokens_.push(Token::parse(ifs_));

        auto token = tokens_.front();
        tokens_.pop();
        return token;
    }

    bool Tokenizer::has_token() {
        tokens_.push(Token::parse(ifs_));
        return tokens_.front().type() != TokenType::UNKNOWN;
    }

    void Tokenizer::to_xml(std::ofstream& ofs) {
        if(!ofs.is_open())
            throw std::runtime_error("file is not open");

        ofs << "<tokens>" << std::endl;
        while(has_token()) {
            auto token = get();
            //ofs << Token::parse(ifs).to_xml() << std::endl;
            ofs << token.to_xml() << std::endl;
        }
        ofs << "</tokens>" << std::endl;
    }
}
