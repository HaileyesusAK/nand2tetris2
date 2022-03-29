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

            std::string compile(const IntegerTerm& term);

            std::string compile(const StringTerm& term);

            std::string compile(const KeywordTerm& term);

            const SymbolTable& symbol_table() const;

        private:
            SymbolTable symbol_table_;
    };
}

#endif
