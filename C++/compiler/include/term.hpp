#ifndef __TERM_H__
#define __TERM_H__

#include <string>
#include "token.hpp"

namespace ntt {

    class Term {
        public:
            virtual std::string to_xml() const = 0;

        protected:
            std::string to_xml_(const Token&) const;
    };
}

#endif
