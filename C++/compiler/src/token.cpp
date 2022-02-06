#include <algorithm>
#include <cctype>
#include <fstream>
#include <istream>
#include <stdexcept>
#include <string>
#include "token.hpp"

namespace ntt {

    std::string Token::to_xml(int level, int indent_width) const {
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

        std::string xml(level * indent_width, ' ');

        xml.append("<" + tag.at(type_) + "> ");
        if(xml_symbols.count(value_))
            xml.append(xml_symbols.at(value_));
        else
            xml.append(value_);

        xml.append(" </");
        xml.append(tag.at(type_));
        xml.append(">");

        return xml;
    }

    TokenType Token::type() const { return type_; }

    std::string Token::value() const { return value_; }

    size_t Token::line_no() const { return line_no_; }

    size_t Token::col_no() const { return col_no_; }

    std::string Token::pos() const { 
        return "(" + std::to_string(line_no_) + ", " + std::to_string(col_no_) + ")";
    }
}
