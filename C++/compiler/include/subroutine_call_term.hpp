#ifndef __SUBROUTINE_CALL_TERM_H__
#define __SUBROUTINE_CALL_TERM_H__

#include <string>
#include "function_call.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {

    /* represents a subroutine call term */
    class SubroutineCallTerm : public Term {
        public:
            SubroutineCallTerm(Tokenizer& tokenizer);

            std::string to_xml(size_t level = 0) const override;

            Type get_type() const override;

        private:
            FunctionCall call_;
    };
}

#endif
