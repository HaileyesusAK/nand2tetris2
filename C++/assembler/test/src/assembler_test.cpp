#include "gmock/gmock.h"
#include "assembler.h"

using namespace testing;

class HackAssembler : public Test {
    public:
        Assembler assembler;
};

TEST_F(HackAssembler, TranslatesAInstructionWithLiteral) {
    ASSERT_THAT(assembler.translate("@33"), Eq("0000000000100001"));
}

TEST_F(HackAssembler, TranslatesAInstructionWithSymbol) {
    assembler.add_symbol("m");
    assembler.add_symbol("n");
    ASSERT_THAT(assembler.translate("@n"), Eq("0000000000010001"));
}

TEST_F(HackAssembler, TranslatesCInstructionMissingDstJmp) {
    ASSERT_THAT(assembler.translate("D+A"), Eq("1110000010000000"));
}

TEST_F(HackAssembler, TranslatesCInstructionMissingJmp) {
    ASSERT_THAT(assembler.translate("A=D&M"), Eq("1111000000100000"));
}

TEST_F(HackAssembler, TranslatesCInstructionMissingDst) {
    ASSERT_THAT(assembler.translate("!M;JMP"), Eq("1111110001000111"));
}

TEST_F(HackAssembler, TranslatesCompleteCInstruction) {
    ASSERT_THAT(assembler.translate("AMD=D-A;JGT"), Eq("1110010011111001"));
}

TEST_F(HackAssembler, CanAddSymbol) {
    ASSERT_THAT(assembler.add_symbol("n"), Eq(true));
}

TEST_F(HackAssembler, RefusesToAddDuplicateSymbol) {
    assembler.add_symbol("n");
    ASSERT_THAT(assembler.add_symbol("n"), Eq(false));
}

TEST_F(HackAssembler, ResetsSymbolTable) {
    assembler.add_symbol("m");
    assembler.reset_symbol_table();
    ASSERT_THAT(assembler.add_symbol("m"), Eq(true));
}
