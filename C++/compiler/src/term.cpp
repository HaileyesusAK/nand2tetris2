#include <sstream>
#include "term.hpp"
#include "token.hpp"

namespace ntt {

    std::string Term::to_xml_(const Token& token) const {
        
        std::ostringstream oss;

        oss << "<term>" << std::endl;
        oss << token.to_xml(1, 4) << std::endl;
        oss << "</term>" << std::endl;

        return oss.str();
    }
}
