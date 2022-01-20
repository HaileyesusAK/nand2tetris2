#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include <fstream>
#include <queue>
#include <string>

namespace ntt {

	enum class TokenType { SYMBOL };

	struct Token {
		std::string value;
		TokenType type;

        Token(const std::string& value, const TokenType& type) : value(value), type(type) {}
	};

	class Tokenizer {
		public:
			Tokenizer(std::ifstream& input) : input_(input) {}

			Token get_next();

		private:
			std::ifstream& input_;
            std::queue<Token> tokens_;
	};

}

#endif
