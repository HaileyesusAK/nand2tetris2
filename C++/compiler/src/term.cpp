#include <sstream>
#include "term.hpp"

namespace ntt {

    std::string Term::to_xml_(const Token& token, size_t level) {
        std::ostringstream oss;

        oss << JackFragment::get_line("<term>", level);
        oss << JackFragment::to_xml(token, level + 1);
        oss << JackFragment::get_line("</term>", level);

        return oss.str();
    }
}
