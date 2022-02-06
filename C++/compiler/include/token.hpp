#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>

namespace ntt {

	enum class TokenType { SYMBOL, KEYWORD, IDENTIFIER, INTEGER, STRING };

	class Token {
		public:
			explicit Token(const std::string& value, const TokenType& type, size_t line_no = 0, size_t col_no = 0)
                           : value_(value), type_(type), line_no_(line_no), col_no_(col_no) {}

            std::string to_xml(int level = 0, int indent_width = 0) const;

            std::string value() const;

            TokenType type() const;
            
            size_t line_no() const;

            size_t col_no() const;

            std::string pos() const;

		private:
			std::string value_;

			TokenType type_;

            size_t line_no_;

            size_t col_no_;
	};
}

#endif
