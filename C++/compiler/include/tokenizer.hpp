#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include <fstream>
#include <string>
#include "token.hpp"

namespace ntt {

	class Tokenizer {
		public:
            static void to_xml(std::ifstream&, std::ofstream&);

        private:
            static inline void write_xml(std::ofstream&, const Token&);
	};

}

#endif
