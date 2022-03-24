#ifndef __STATEMENT_FACTORY_H__
#define __STATEMENT_FACTORY_H__

#include <memory>
#include "statement.hpp"
#include "tokenizer.hpp"


namespace ntt {

    /* Jack statement factory class */
    class StatementFactory {
        public:
            static std::unique_ptr<Statement> parse(Tokenizer&);

        private:
            enum class StatementID { DO, IF, LET, RETURN, WHILE };
    };
}

#endif
