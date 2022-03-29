#include <stdexcept>
#include <filesystem>
#include <fstream>
#include "code_generator.hpp"
#include "gmock/gmock.h"
#include "types.hpp"
#include "tokenizer.hpp"

using namespace ntt;
using namespace std;
using namespace testing;

namespace fs = std::filesystem;

const fs::path DATA_DIR = fs::path{TEST_DIR} / "data";

class FCodeGenerator : public Test {
    public:
        CodeGenerator generator;

        template <typename T> 
        auto compile(const std::string& jack_file) {
            ifstream ifs { DATA_DIR / jack_file };
            Tokenizer tokenizer { ifs };
            T jack_element { tokenizer };

            return generator.compile(jack_element);
        }
};

TEST_F(FCodeGenerator, CompilesEmptyParameterList) {
    ASSERT_THAT(compile<ParameterList>("parameter_list_empty.jack"), Eq(0));
}

TEST_F(FCodeGenerator, CompilesNonEmptyParameterList) {
    ASSERT_THAT(compile<ParameterList>("parameter_list.jack"), Eq(2));
}

TEST_F(FCodeGenerator, UpdatesSymbolTableWhenCompilingNonEmptyParameterList) {
    compile<ParameterList>("parameter_list.jack");
    auto entry = generator.symbol_table().get_entry("a");

    ASSERT_THAT(entry.type, Eq("int"));
    ASSERT_THAT(entry.index, Eq(0));
    ASSERT_THAT(entry.kind, Eq(SymbolKind::ARGUMENT));
}

TEST_F(FCodeGenerator, HandlesDuplicateEntryParameterList) {
    ASSERT_THROW(compile<ParameterList>("parameter_list_dup.jack"), runtime_error);
}

TEST_F(FCodeGenerator, CompilesMethodVariableDeclaration) {
    ASSERT_THAT(compile<SubroutineVarDec>("multi_var_dec.jack"), Eq(2));
}

TEST_F(FCodeGenerator, UpdatesSymbolTableWhenCompilingVariableDeclaration) {
    compile<SubroutineVarDec>("multi_var_dec.jack");
    auto entry = generator.symbol_table().get_entry("weight");

    ASSERT_THAT(entry.type, Eq("int"));
    ASSERT_THAT(entry.index, Eq(1));
    ASSERT_THAT(entry.kind, Eq(SymbolKind::LOCAL));
}

TEST_F(FCodeGenerator, HandlesDuplicateVariableDeclaration) {
    ASSERT_THROW(compile<SubroutineVarDec>("multi_var_dec_dup.jack"), runtime_error);
}
