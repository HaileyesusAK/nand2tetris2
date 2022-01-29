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

        private:

            std::queue<Token> tokens_;
	};

}

#endif
