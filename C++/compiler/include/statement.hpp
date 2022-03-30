#ifndef __STATEMENT_H__
#define __STATEMENT_H__

#include "jack_fragment.hpp"

namespace ntt {

    /* represents a generic jack statement */
    class Statement : public JackFragment {
        public:
            enum Type { DO, IF, LET, RETURN, WHILE };
            
            virtual Type type() const = 0;
    };
}

#endif
