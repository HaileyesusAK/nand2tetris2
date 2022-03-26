#ifndef __JACK_FRAGMENT_H__
#define __JACK_FRAGMENT_H__

#include <string>
#include "token.hpp"

namespace ntt {

    class JackFragment {
        public:

            virtual ~JackFragment() = default;

            virtual std::string to_xml(size_t level = 0) const = 0;

            static std::string get_line(const std::string& s, size_t level);

            static std::string to_xml(const Token& token, size_t level);

            static constexpr size_t TAB_WIDTH_ = 4;
    };
}

#endif
