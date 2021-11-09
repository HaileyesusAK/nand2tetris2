#include "gmock/gmock.h"
#include "assembler.h"


TEST(HackAssembler, TranslatesAInstructionWithLiteral) {
    Assembler assembler;

    ASSERT_THAT(assembler.translate("@0"), testing::Eq("0000000000000000"));
}

TEST(HackAssembler, TranslatesAInstructionWithSymbol) {
    Assembler assembler;

    ASSERT_THAT(assembler.translate("@variable"), testing::Eq("0000000000010000"));
}
