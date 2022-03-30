#ifndef __LET_STATEMENT_H__
#define __LET_STATEMENT_H__

#include <functional>
#include <optional>
#include <string>
#include "expression.hpp"
#include "statement.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {

    /* represents a let statment */
    class LetStatement : public Statement {
        public:
            LetStatement(Tokenizer&);

            std::string to_xml(size_t level = 0) const override;

            const Token& variable() const;

            std::optional<std::reference_wrapper<const Expression>> index_expression() const;

            const Expression& assignment_expression() const;

        private:
            struct IndexExpression {
                Token left_bracket;
                Expression expression;
                Token right_bracket;

                IndexExpression(Tokenizer&);
            };
            Token let_;
            Token var_name_;
            std::optional<IndexExpression> index_expression_;
            Token eq_;
            Expression expression_;
            Token semicolon_;

            static std::optional<IndexExpression> parse_index_exp_(Tokenizer&);
    };

}

#endif
