#ifndef __STRING_TERM_H__
#define __STRING_TERM_H__

#include <string>
#include "term.hpp"

namespace ntt {
    class StringTerm : public Term {
        public:
            StringTerm(const std::string& value) : value_(value) {}

        private:
            std::string value_;
    };
}

#endif
