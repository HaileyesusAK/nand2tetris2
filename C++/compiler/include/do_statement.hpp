#ifndef __DO_STATEMENT_H__
#define __DO_STATEMENT_H__

#include <string>
#include <variant>
#include "method_call_term.hpp"
#include "statement.hpp"
#include "subroutine_call_term.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {

    class DoStatement : public Statement {
        public:
            DoStatement(Tokenizer&);

            std::string to_xml(size_t level = 0) const override;
			
            using CallTerm = std::variant<MethodCallTerm, SubroutineCallTerm>;
			const CallTerm& call_term() const;

        private:
            struct CallTermVisitor{
                size_t level;
                CallTermVisitor(size_t l): level(l) {}

                template <typename T>
                std::string operator()(const T& term) {
                    return term.to_xml(level);
                }
            };

            Token do_;
            CallTerm call_term_;
            Token semicolon_;

            static CallTerm parse_call_term_(Tokenizer&);
    };
}

#endif
