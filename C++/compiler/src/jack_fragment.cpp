#include <iomanip>
#include <sstream>
#include "jack_fragment.hpp"

namespace ntt {

    std::string JackFragment::get_line(const std::string& s, size_t level) {
        std::ostringstream oss;
        oss << std::setw(s.size() + JackFragment::TAB_WIDTH_ * level) << s << std::endl;
        return oss.str();
    }

    std::string JackFragment::to_xml(const Token& token, size_t level) {
        std::ostringstream oss;
        oss << token.to_xml(level, JackFragment::TAB_WIDTH_) << std::endl;
        return oss.str();
    }
}
