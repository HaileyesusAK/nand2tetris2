#include <fstream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {
    
    void Tokenizer::write_xml(std::ofstream& ofs, const Token& token) {
        static const std::unordered_map<TokenType, std::string> tag {
            {TokenType::STRING, "stringConstant"},
            {TokenType::INTEGER, "integerConstant"},
            {TokenType::SYMBOL, "symbol"},
            {TokenType::IDENTIFIER, "identifier"},
            {TokenType::KEYWORD, "keyword"}
        };

        static const std::unordered_map<std::string, std::string> xml_symbols { 
            {">", "&gt;"}, {"<", "&lt;"}, {"\"", "&quot;"}, {"&", "&amp;"}
        };

        ofs << "<" << tag.at(token.type()) << "> ";
        if(xml_symbols.count(token.value()))
            ofs << xml_symbols.at(token.value());
        else
            ofs << token.value();
        ofs << " </" << tag.at(token.type()) << ">" << std::endl;
    }

    void Tokenizer::to_xml(std::ifstream& ifs, std::ofstream& ofs) {
        if(!ifs.is_open() || !ofs.is_open())
            throw std::runtime_error("file is not open");

        ofs << "<tokens>" << std::endl;
        while(ifs.peek() != EOF) {
            /* remove any whitespace characters */
            std::ws(ifs);
            if(ifs.peek() == EOF)
                break;

            write_xml(ofs, Token::parse(ifs));
        }
        ofs << "</tokens>" << std::endl;
    }

}
