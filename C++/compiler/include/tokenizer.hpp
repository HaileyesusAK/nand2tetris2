#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include <fstream>
#include <queue>
#include <string>
#include "token.hpp"

namespace ntt {

	class Tokenizer {
		public:

            explicit Tokenizer(std::ifstream& ifs);
            
            Token get();

            const Token& peek();

            bool has_token();

            void to_xml(std::ofstream&);

            Token consume_identifier();

            Token consume_keyword(const std::string&);

            Token consume_symbol(const std::string&);

        private:

            std::queue<Token> tokens_;
	};

}

#endif
