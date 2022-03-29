#ifndef __CODE_GENERATOR_H__
#define __CODE_GENERATOR_H__

#include <cstdint>
#include <memory>
#include <string>
#include "types.hpp"
#include "symbol_table.hpp"

namespace ntt {

    class CodeGenerator {
        public:
            /* add the parameters in the symbol table and return their count */
            uint16_t compile(const ParameterList& param_list);

            /* add the parameters in the symbol table and return their count */
            uint16_t compile(const SubroutineVarDec& var_dec);
          
            /* compile the expression and return the corresponding vm code */
            std::string compile(const Expression& expression);

            /* compile the term and return the corresponding vm code */
            std::string compile(const std::unique_ptr<Term>& term);

            std::string compile(const ArrayTerm& term);

            std::string compile(const IdentifierTerm& term);

            std::string compile(const IntegerTerm& term);

            std::string compile(const KeywordTerm& term);

            std::string compile(const StringTerm& term);

            const SymbolTable& symbol_table() const;

        private:

            static std::string segment(const SymbolKind& kind);

            SymbolTable symbol_table_;
    };
}

#endif
