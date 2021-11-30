#include <string>
#include <vector>
#include "gmock/gmock.h"
#include "vmtranslator.h"


using namespace testing;

class VMTranslator : public Test {
    public:
        VmTranslator translator;
        void append_push_D(std::vector<std::string>& instructions) {
            const static std::vector<std::string> push_D_inst {
                "@SP", "A=M", "M=D", "@SP", "M=M+1"
            };

            for(const auto& inst:push_D_inst)
                instructions.push_back(inst);
        }
};

TEST_F(VMTranslator, TranslatesBinaryArithmeticCommands) {
    std::vector<std::string> expected_result {
        "@SP", "AM=M-1", "D=M", "@SP", "A=M-1", "M=M-D"};
    auto result = translator.translate(BinaryAluOp::SUB);
    ASSERT_THAT(result, Eq(expected_result));
}

TEST_F(VMTranslator, TranslatesRelationalCommands) {
    std::vector<std::string> expected_result {
        "@SP", "AM=M-1", "D=M", "@SP", "A=M-1", "D=M-D", "M=-1", "@10", "D;JEQ", "M=0"};
    auto result = translator.translate(RelOp::EQ, 0);
    ASSERT_THAT(result, Eq(expected_result));
}

TEST_F(VMTranslator, TranslatesUnaryAluCommands) {
    std::vector<std::string> expected_result { "@SP", "A=M-1", "M=!M" };
    auto result = translator.translate(UnaryOp::NOT);
    ASSERT_THAT(result, Eq(expected_result));
}

TEST_F(VMTranslator, TranslatesPushingFromNamedSegment) {
    uint16_t i = 1245;
    std::vector<std::string> expected_result {
        "@LCL", "D=M", "@" + std::to_string(i), "A=D+A", "D=M"
    };
    append_push_D(expected_result);

    auto result = translator.translate_push(Segment::LCL, i);
    ASSERT_THAT(result, Eq(expected_result));
}

TEST_F(VMTranslator, TranslatesPushingFromStaticSegment) {
    uint16_t i = 5;
    std::string filename {"pong.vm"};

    std::vector<std::string> expected_result {
        "@" + filename + "." + std::to_string(i),
        "D=A"
    };
    append_push_D(expected_result);

    auto result = translator.translate_push_static(filename, i);
    ASSERT_THAT(result, Eq(expected_result));
}

TEST_F(VMTranslator, TranslatesPushingFromConstantSegment) {
    uint16_t i = 3459;

    std::vector<std::string> expected_result { "@" + std::to_string(i), "D=A"};
    append_push_D(expected_result);
    auto result = translator.translate_push_constant(i);
    ASSERT_THAT(result, Eq(expected_result));
}
