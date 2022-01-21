#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <fstream>
#include <string>
#include <unordered_set>

namespace ntt {

	enum class TokenType { UNKNOWN, SYMBOL };

	class Token {
		public:
			explicit Token(const std::string& value,
                           const TokenType& type = TokenType::UNKNOWN)
                           : value_(value), type_(type) {}

			static Token parse(std::ifstream& ifs);
            
            static bool is_symbol(char c);

            std::string value() const;

            TokenType type() const;

		private:
			static const inline std::unordered_set<char> symbols_ {
				'{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', 
				'/', '&', '|', '<', '>', '=', '_', '~' };

			std::string value_;

			TokenType type_;
	};
}


#endif
