#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include <fstream>

namespace ntt {

	class Tokenizer {
		public:
            static void parse(std::ifstream&, std::ofstream&);
	};

}

#endif
