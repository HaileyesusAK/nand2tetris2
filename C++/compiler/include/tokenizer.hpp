#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include <fstream>
#include <deque>
#include <string>
#include <unordered_set>
#include "token.hpp"

namespace ntt {

	class Tokenizer {
		public:

            explicit Tokenizer(std::ifstream& ifs);
            
            Token get();

            void put(const Token& token);

            const Token& peek();

            bool has_token();

            void to_xml(std::ofstream&);

            Token consume_identifier();

            Token consume_keyword(const std::unordered_set<std::string>&);

            Token consume_symbol(const std::string&);

            Token consume_type();

        private:

            std::deque<Token> tokens_;
	};

}

#endif
