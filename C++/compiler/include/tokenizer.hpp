#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include <fstream>
#include <queue>
#include <string>
#include "token.hpp"

namespace ntt {

	class Tokenizer {
		public:

            Tokenizer(std::ifstream& ifs) : ifs_(ifs) {}
            
            Token get();

            bool has_token();

            void to_xml(std::ofstream&);

        private:

            std::ifstream& ifs_;

            std::queue<Token> tokens_;
	};

}

#endif
