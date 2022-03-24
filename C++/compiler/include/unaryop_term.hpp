#ifndef __UNIARY_OP_TERM_H__
#define __UNIARY_OP_TERM_H__


#include <memory>
#include <string>
#include "term.hpp"

namespace ntt {

    /* represents a term with unary operator */
    class UnaryOpTerm : public Term {

        public:
            UnaryOpTerm(Tokenizer&);

            std::string to_xml(size_t level = 0) const override;

        private:
            Token unary_op_;
            std::unique_ptr<Term> term_;
    };
}

#endif
