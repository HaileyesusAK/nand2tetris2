#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>

namespace ntt {

	enum class TokenType { SYMBOL, KEYWORD, IDENTIFIER, INTEGER, STRING };

	class Token {
		public:
			explicit Token(const std::string& value, const TokenType& type)
                           : value_(value), type_(type) {}

            std::string to_xml() const;

            std::string value() const;

            TokenType type() const;

		private:

			std::string value_;

			TokenType type_;
	};
}

#endif
