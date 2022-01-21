#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <fstream>
#include <string>
#include <unordered_set>

namespace ntt {

	enum class TokenType { UNKNOWN, SYMBOL, KEYWORD };

	class Token {
		public:
			explicit Token(const std::string& value,
                           const TokenType& type = TokenType::UNKNOWN)
                           : value_(value), type_(type) {}

			static Token parse(std::ifstream& ifs);

            static void remove_leading_ws(std::ifstream& ifs);
            
            static bool is_symbol(char c);

            static bool is_keyword(const std::string& word);

            std::string value() const;

            TokenType type() const;

		private:
			static const inline std::unordered_set<char> symbols_ {
				'{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', 
				'/', '&', '|', '<', '>', '=', '_', '~'
            };

            static const inline std::unordered_set<std::string> keywords_ {
                "class", "constructor", "function", "method", "field", "static",
                "var", "int", "char", "boolean", "void", "true", "false", "null",
                "this", "let", "do", "if", "else", "while", "return"
            };

			std::string value_;

			TokenType type_;
	};
}


#endif
