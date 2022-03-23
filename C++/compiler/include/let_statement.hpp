#ifndef __LET_STATEMENT_H__
#define __LET_STATEMENT_H__

#include <optional>
#include <string>
#include <tuple>
#include "expression.hpp"
#include "statement.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {

    class LetStatement : public Statement {

        public:
            LetStatement(Tokenizer&);

            std::string to_xml(size_t level = 0) const override;

        private:
            Token let_;

            Token var_name_;

            using IndexExpression = std::optional<std::tuple<Token, Expression, Token>>;

            IndexExpression index_expression_;

            Token eq_;

            Expression expression_;

            Token semicolon_;

            static IndexExpression parse_index_exp(Tokenizer&);
    };

}

#endif
