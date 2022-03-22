#include <iomanip>
#include <sstream>
#include "term.hpp"
#include "token.hpp"

namespace ntt {

    std::string Term::to_xml_(const Token& token, size_t level) {
        std::ostringstream oss;

        oss << JackFragment::get_line("<term>", level);
        oss << token.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << JackFragment::get_line("</term>", level);

        return oss.str();
    }
}
