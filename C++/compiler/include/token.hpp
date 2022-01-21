#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <fstream>
#include <string>
#include <unordered_set>

namespace ntt {

	enum class TokenType { UNKNOWN, SYMBOL, KEYWORD, IDENTIFIER, INTEGER };

	class Token {
		public:
			explicit Token(const std::string& value,
                           const TokenType& type = TokenType::UNKNOWN)
                           : value_(value), type_(type) {}

			static Token parse(std::ifstream&);

            static void remove_leading_ws(std::ifstream&);

            static bool is_symbol(char);

            static bool is_keyword(const std::string&);

            static bool is_identifier(const std::string&);

            static bool is_integer(const std::string&);

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
