#include <stdexcept>
#include <filesystem>
#include <fstream>
#include "code_generator.hpp"
#include "gmock/gmock.h"
#include "parameter_list.hpp"
#include "tokenizer.hpp"

using namespace ntt;
using namespace std;
using namespace testing;

namespace fs = std::filesystem;

const fs::path DATA_DIR = fs::path{TEST_DIR} / "data";

class FCodeGenerator : public Test {
    public:
        CodeGenerator generator;

        uint16_t compile_parameter_list(const std::string jack_file) {
            ifstream ifs { DATA_DIR / jack_file };
            Tokenizer tokenizer { ifs };
            ParameterList param_list { tokenizer };

            return generator.compile(param_list);
       }
};

TEST_F(FCodeGenerator, CompilesEmptyParameterList) {
    ASSERT_THAT(compile_parameter_list("parameter_list_empty.jack"), Eq(0));
}

TEST_F(FCodeGenerator, CompilesNonEmptyParameterList) {
    ASSERT_THAT(compile_parameter_list("parameter_list.jack"), Eq(2));
}

TEST_F(FCodeGenerator, UpdatesSymbolTableWhenCompilingNonEmptyParameterList) {
    compile_parameter_list("parameter_list.jack");
    auto entry = generator.symbol_table().get_entry("a");

    ASSERT_THAT(entry.type, Eq("int"));
    ASSERT_THAT(entry.index, Eq(0));
    ASSERT_THAT(entry.kind, Eq(SymbolKind::ARGUMENT));
}

TEST_F(FCodeGenerator, HandlesDuplicateEntryParameterList) {
    ASSERT_THROW(compile_parameter_list("parameter_list_dup.jack"), runtime_error);
}
