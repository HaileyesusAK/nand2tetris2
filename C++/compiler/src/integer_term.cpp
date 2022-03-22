#include <string>
#include <sstream>
#include "integer_term.hpp"
#include "tokenizer.hpp"

namespace ntt {

    IntegerTerm::IntegerTerm(Tokenizer& tokenizer) : token_(tokenizer.consume_integer()) {}

    std::string IntegerTerm::to_xml() const {
        std::ostringstream oss;

        oss << "<term>" << std::endl;
        oss << token_.to_xml(1, 4) << std::endl;
        oss << "</term>" << std::endl;

        return oss.str();
    }
}
