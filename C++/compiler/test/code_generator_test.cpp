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

class FCodeGenerator : public Test {
    public:
        uint16_t compile_parameter_list(const std::string jack_file) {
            ifstream ifs { DATA_DIR / jack_file };
            Tokenizer tokenizer { ifs };
            ParameterList param_list { tokenizer };

            return generator_.compile(param_list);
       }

    private:
        CodeGenerator generator_;
};

TEST_F(FCodeGenerator, CompilesEmptyParameterList) {
    ASSERT_THAT(compile_parameter_list("parameter_list_empty.jack"), Eq(0));
}

TEST_F(FCodeGenerator, CompilesNonEmptyParameterList) {
    ASSERT_THAT(compile_parameter_list("parameter_list.jack"), Eq(2));
}
