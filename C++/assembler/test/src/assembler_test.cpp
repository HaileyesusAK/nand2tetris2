#include "gmock/gmock.h"
#include "assembler.h"

using namespace testing;

class HackAssembler : public Test {
    public:
        Assembler assembler;
};

TEST_F(HackAssembler, TranslatesAInstructionWithLiteral) {
    ASSERT_THAT(assembler.translate("@0"), Eq("0000000000000000"));
}

TEST_F(HackAssembler, TranslatesAInstructionWithSymbol) {
    ASSERT_THAT(assembler.translate("@variable"), Eq("0000000000010000"));
}

TEST_F(HackAssembler, TranslatesCInstructionWithNoDstNoJmp) {
    ASSERT_THAT(assembler.translate("D+A"), Eq("1110000010000000"));
}
