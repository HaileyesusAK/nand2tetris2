#ifndef __TERM_H__
#define __TERM_H__

#include <string>
#include "jack_fragment.hpp"
#include "token.hpp"

namespace ntt {


    /* A base class for the various terms of Jack */
    class Term : public JackFragment {
        public:
            enum Type {
                ARRAY, IDENTIFIER, INTEGER, KEYWORD, METHOD_CALL,
                PARENTHESIZED_TERM, STRING, SUBROUTINE_CALL,  UNARY_OP
            };

            virtual std::string to_xml(size_t level = 0) const override = 0;

            virtual ~Term() = default;

            virtual Type get_type() const = 0;

        protected:
            /*
                Return a <term> xml element whose content is the xml
                representation of the supplied token.
            */
            static std::string to_xml_(const Token&, size_t level = 0);
    };
}

#endif
