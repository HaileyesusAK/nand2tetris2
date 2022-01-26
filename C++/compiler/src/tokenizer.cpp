#include <fstream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {

    void Tokenizer::to_xml(std::ifstream& ifs, std::ofstream& ofs) {
        if(!ifs.is_open() || !ofs.is_open())
            throw std::runtime_error("file is not open");

        ofs << "<tokens>" << std::endl;
        while(ifs.peek() != EOF) {
            /* remove any whitespace characters */
            std::ws(ifs);
            if(ifs.peek() == EOF)
                break;

            ofs << Token::parse(ifs).to_xml() << std::endl;
        }
        ofs << "</tokens>" << std::endl;
    }

}
