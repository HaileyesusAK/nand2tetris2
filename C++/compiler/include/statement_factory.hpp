#ifndef __STATEMENT_FACTORY_H__
#define __STATEMENT_FACTORY_H__

#include <memory>
#include "statement.hpp"
#include "tokenizer.hpp"


namespace ntt {

    class StatementFactory {

        public:
            static std::unique_ptr<Statement> parse(Tokenizer&);

        private:
            enum class StatementID { DO, LET, RETURN, WHILE }; 
    };
}

#endif
