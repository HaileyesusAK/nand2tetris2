#include "code_generator.hpp"
#include "gmock/gmock.h"
#include "parameter_list.hpp"
#include "tokenizer.hpp"
#include <filesystem>
#include <fstream>

using namespace ntt;
using namespace std;
using namespace testing;

namespace fs = std::filesystem;

const fs::path DATA_DIR = fs::path{TEST_DIR} / "data";

TEST(CodeGenerator, CompilesEmptyParameterList) {
    ifstream ifs { DATA_DIR / "parameter_list_empty.jack" }; 
    Tokenizer tokenizer { ifs };
    ParameterList param_list { tokenizer };

    CodeGenerator generator;
    ASSERT_THAT(generator.compile(param_list), Eq(0));
}
