#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_set>

namespace ntt {

	enum class TokenType { SYMBOL, KEYWORD, IDENTIFIER, INTEGER, STRING };

    class NoTokenErr : public std::runtime_error {
        public:
            NoTokenErr() : std::runtime_error("No more tokens") {}
    };

	class Token {
		public:
			explicit Token(const std::string& value, const TokenType& type)
                           : value_(value), type_(type) {}

			static Token parse(std::ifstream&);

            std::string to_xml() const;

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

            static void remove_leading_ws(std::ifstream&);

            static bool is_symbol(char);

            static bool is_keyword(const std::string&);

            static bool is_identifier(const std::string&);

            static bool is_integer(const std::string&);
	};
}


#endif
