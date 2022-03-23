#ifndef __DO_STATEMENT_H__
#define __DO_STATEMENT_H__

#include <string>
#include "jack_fragment.hpp"
#include "subroutine_call_term.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {

    class DoStatement : public JackFragment {
        public:
            DoStatement(Tokenizer&);

            std::string to_xml(size_t level = 0) const override;

        private:
            Token do_;

            SubroutineCallTerm call_term_;

            Token semicolon_;
    };
}

#endif
