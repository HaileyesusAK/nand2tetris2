#ifndef __ARRAY_TERM_H__
#define __ARRAY_TERM_H__ 

#include <cstdint>
#include <string>
#include "term.hpp"
#include "expression.hpp"

namespace ntt {
    class ArrayTerm : public Term {
        public:
            ArrayTerm(const std::string& identifier, Expression&& expression) :
                identifier_(identifier), expression_(std::move(expression)) {}

        private:
            std::string identifier_;
            Expression expression_;
    };
}

#endif
