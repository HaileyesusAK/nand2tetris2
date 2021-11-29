#include <string>
#include <vector>
#include "gmock/gmock.h"
#include "vmtranslator.h"


using namespace testing;

class VMTranslator : public Test {
    public:
        VmTranslator translator;
};

TEST_F(VMTranslator, TranslatesBinaryArithmeticCommands) {
    std::vector<std::string> expected_result {
        "@SP", "AM=M-1", "D=M", "@SP", "A=M-1", "M=M-D"};
    auto result = translator.translate(BinaryAluOp::SUB);
    ASSERT_THAT(expected_result, Eq(result));
}

TEST_F(VMTranslator, TranslatesRelationalCommands) {
    std::vector<std::string> expected_result {
        "@SP", "AM=M-1", "D=M", "@SP", "A=M-1", "D=M-D", "M=-1", "@10", "D;JEQ", "M=0"};
    auto result = translator.translate(RelOp::EQ, 0);
    ASSERT_THAT(expected_result, Eq(result));
}
