#ifndef __WHILE_STATEMENT_H__
#define __WHILE_STATEMENT_H__

#include <memory>
#include <string>
#include <vector>
#include "expression.hpp"
#include "statement.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {

    /* represents a while statement */
    class WhileStatement : public Statement {
        public:
            WhileStatement(Tokenizer&);

            std::string to_xml(size_t level = 0) const override;

            const Expression& expression() const;

            using StatementList = std::vector<std::unique_ptr<Statement>>;
            const StatementList& statements() const;

            Type type() const override;

        private:
            Token while_;
            Token left_parenthesis_;
            Expression expression_;
            Token right_parenthesis_;
            Token left_brace_;
            StatementList statements_;
            Token right_brace_;

            static StatementList parse_statements_(Tokenizer&);
    };
}

#endif
