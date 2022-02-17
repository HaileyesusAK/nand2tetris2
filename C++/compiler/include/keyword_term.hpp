#ifndef __KEYWORD_TERM_H__
#define __KEYWORD_TERM_H__

#include "types.hpp"

namespace ntt {
    class KeywordTerm : public Term {
        public:
            KeywordTerm(const Keyword& keyword) : keyword_(keyword) {}

        private:
            Keyword keyword_;
    };
}

#endif
