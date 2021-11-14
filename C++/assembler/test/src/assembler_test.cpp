#include <filesystem>
#include "gmock/gmock.h"
#include "assembler.h"

using namespace testing;
namespace fs = std::filesystem;
static const fs::path DATA_DIR = fs::path(TEST_DIR) / "data";

class HackAssembler : public Test {
    public:
        Assembler assembler;
};

TEST_F(HackAssembler, TranslatesAInstructionWithLiteral) {
    ASSERT_THAT(assembler.translate("@33"), Eq("0000000000100001"));
}

TEST_F(HackAssembler, TranslatesAInstructionWithSymbol) {
    assembler.add_variable("m");
    assembler.add_variable("n");
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
    ASSERT_THAT(assembler.add_variable("n"), Eq(true));
}

TEST_F(HackAssembler, RefusesToAddDuplicateSymbol) {
    assembler.add_variable("n");
    ASSERT_THAT(assembler.add_variable("n"), Eq(false));
}

TEST_F(HackAssembler, ResetsSymbolTable) {
    assembler.add_variable("m");
    assembler.reset_symbol_table();
    ASSERT_THAT(assembler.add_variable("m"), Eq(true));
}

TEST_F(HackAssembler, TranslatesInstructionWithSpaces) {
    ASSERT_THAT(assembler.translate("AMD = D-A; JGT"), Eq("1110010011111001"));
}

TEST_F(HackAssembler, TranslatesInstructionWithComment) {
    ASSERT_THAT(assembler.translate("@4 // comment ..."), Eq("0000000000000100"));
}

TEST_F(HackAssembler, IgnoresBlankLine) {
    ASSERT_THAT(assembler.translate("   "), Eq(""));
}

TEST_F(HackAssembler, TranslatesAInstructionWithPredefinedSymbol) {
    ASSERT_THAT(assembler.translate("@KBD"), Eq("0110000000000000"));
}

TEST_F(HackAssembler, BuildsSymbolTable) {
	assembler.reset_symbol_table();
	assembler.build_symbol_table(fs::path(DATA_DIR / "Mult.asm"));
	
	ASSERT_THAT(assembler.get_address("R1"), Eq(1));
	ASSERT_THAT(assembler.get_address("sum"), Eq(17));
	ASSERT_THAT(assembler.get_address("LOOP_END"), Eq(18));
	ASSERT_THAT(assembler.get_address("END"), Eq(24));
}
