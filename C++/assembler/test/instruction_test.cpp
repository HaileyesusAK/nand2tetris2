#include <stdexcept>
#include <string>
#include "gmock/gmock.h"
#include "instruction.h"
#include "symbol_table.h"

using namespace std;
using namespace testing;

TEST(HackInstruction, CompactsInstructions) {
	string inst {"D = D-A; JMP		//comment\n"};
	ASSERT_THAT(Instruction::compact(inst), Eq("D=D-A;JMP"));
}

TEST(HackInstruction, DetectsCInstructionWithInvalidDst) {
	ASSERT_THROW(CInstruction {"d=D+A"}, InvalidDstErr);
}

TEST(HackInstruction, DetectsCInstructionWithInvalidComp) {
	ASSERT_THROW(CInstruction {"D=D&D"}, InvalidCompErr);
}

TEST(HackInstruction, DetectsCInstructionWithInvalidJmp) {
	ASSERT_THROW(CInstruction {"D=D-A;jmp"}, InvalidJmpErr);
}

TEST(HackInstruction, EncodesCInstructionMissingDstJmp) {
	CInstruction inst {"D+A"};
    ASSERT_THAT(inst.encode(), Eq(57472));	// == 1110000010000000
}

TEST(HackInstruction, EncodesCInstructionMissingJmp) {
	CInstruction inst {"A=D&M"};
    ASSERT_THAT(inst.encode(), Eq(61472)); // == 1111000000100000
}

TEST(HackInstruction, EncodesCInstructionMissingDst) {
	CInstruction inst {"!M;JMP"};
    ASSERT_THAT(inst.encode(), Eq(64583)); // == 1111110001000111
}

TEST(HackInstruction, EncodesCompleteCInstruction) {
    CInstruction inst { "AMD=D-A;JGT" };
    ASSERT_THAT(inst.encode(), Eq(58617)); // == 1110010011111001
}

TEST(HackInstruction, DetectsAInstructionWithInvalidConstant) {
	ASSERT_THROW(AInstruction {"@33333"}, runtime_error);
}

TEST(HackInstruction, EncodesAInstructionWithConstantValue) {
    AInstruction inst {"@35"};
    SymbolTable table;
    ASSERT_THAT(inst.encode(table), Eq(static_cast<uint16_t>(35)));
}

TEST(HackInstruction, EncodesAInstructionWithVariable) {
    SymbolTable table;
    table.insert("LOOP_BEG", 50);
    AInstruction inst {"@LOOP_BEG"};
    ASSERT_THAT(inst.encode(table), Eq(static_cast<uint16_t>(50)));
}
