#include <fstream>
#include <stdexcept>
#include <string>
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {

    void Tokenizer::parse(std::ifstream& ifs, std::ofstream& ofs) {
        if(!ifs.is_open() || !ofs.is_open())
            throw std::runtime_error("file is not open");
    }

}
