#ifndef __INTEGER_TERM_H__
#define __INTEGER_TERM_H__

#include <cstdint>
#include "term.hpp"

namespace ntt {
    class IntegerTerm : public Term {
        public:
            IntegerTerm(uint16_t value) : value_(value) {}

        private:
            uint16_t value_;
    };
}

#endif
