#include <array>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include "gmock/gmock.h"
#include "vmtranslator.h"


using namespace testing;
namespace fs = std::filesystem;
static const fs::path DATA_DIR = fs::path(TEST_DIR) / "data";
static const fs::path EXPECTED_DATA_DIR = DATA_DIR / "expected";
static const fs::path TOOLS_DIR = fs::path(NT_SUITE);

class VMTranslator : public Test {
    public:
        VmTranslator translator;
        void append_push_D(InstList& instructions) {
            const static InstList push_D_inst {
                "@SP", "A=M", "M=D", "@SP", "M=M+1"
            };

            for(const auto& inst:push_D_inst)
                instructions.push_back(inst);
        }

        std::pair<std::string, int> run_simulator(const fs::path& tst_path) {
            std::ostringstream cmd_os;
            fs::path executable_path = TOOLS_DIR / "CPUEmulator.sh";
            cmd_os << executable_path << " " << tst_path << " 2>&1";

            std::array<char, 128> buffer;
            std::string result;
            auto pipe = popen(cmd_os.str().c_str(), "r");
            if (!pipe) throw std::runtime_error("popen() failed!");

            while (!feof(pipe)) {
                if (fgets(buffer.data(), 128, pipe) != nullptr)
                    result += buffer.data();
            }

            return std::make_pair(result, pclose(pipe));

        }

        std::pair<std::string, int> run_simulator(const char* asm_file) {
            fs::path tst_path = EXPECTED_DATA_DIR / asm_file;
            tst_path.replace_extension(".tst");
            return run_simulator(tst_path);
        }

        std::pair<std::string, int> run_simulator(const InstList& asm_instructions,
                                                  const std::string& asm_file)
        {
            fs::path asm_path = EXPECTED_DATA_DIR / asm_file;
            std::ofstream ofs {asm_path};
            for(const auto& inst : asm_instructions)
                ofs << inst << std::endl;

            fs::path tst_path = asm_path;
            tst_path.replace_extension(".tst");
            return run_simulator(tst_path);
        }

};

TEST_F(VMTranslator, TranslatesBinaryArithmeticCommands) {
    InstList expected_result {
        "@SP", "AM=M-1", "D=M", "@SP", "A=M-1", "M=M-D"};
    auto result = translator.translate(BinaryAluOp::SUB);
    ASSERT_THAT(result, Eq(expected_result));
}

TEST_F(VMTranslator, UpdatesStackAfterBinaryArithmeticCommandTranslation) {
    auto result = run_simulator(translator.translate(BinaryAluOp::SUB), "sub.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesRelationalCommands) {
    InstList expected_result {
        "@SP", "AM=M-1", "D=M",
        "@SP", "A=M-1", "D=M-D", "M=-1",
        "@12", "D;JEQ", "@SP", "A=M-1", "M=0"
    };
    auto result = translator.translate(RelOp::EQ);
    ASSERT_THAT(result, Eq(expected_result));
}

TEST_F(VMTranslator, UpdatesStackAfterRelationalCommandTranslation) {
    auto result = run_simulator(translator.translate(RelOp::EQ), "eq.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesUnaryAluCommands) {
    InstList expected_result { "@SP", "A=M-1", "M=!M" };
    auto result = translator.translate(UnaryOp::NOT);
    ASSERT_THAT(result, Eq(expected_result));
}

TEST_F(VMTranslator, UpdatesStackAfterUnaryAluCommandTranslation) {
    auto result = run_simulator(translator.translate(UnaryOp::NOT), "not.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesPushingFromNamedSegment) {
    uint16_t i = 1245;
    InstList expected_result {
        "@LCL", "D=M", "@" + std::to_string(i), "A=D+A", "D=M"
    };
    append_push_D(expected_result);

    auto result = translator.translate_push(PushableSegment::LCL, i);
    ASSERT_THAT(result, Eq(expected_result));
}

TEST_F(VMTranslator, UpdatesStackAfterPushingFromNamedSegment) {
    auto instructions = translator.translate_push(PushableSegment::ARG, 5);
    auto result = run_simulator(instructions, "pushargument.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesPopToNamedSegment) {
    uint16_t i = 5;
    InstList expected_result {
        "@" + std::to_string(i), "D=A", "@LCL", "M=D+M",
        "@SP", "AM=M-1", "D=M",
        "@LCL", "A=M", "M=D", "@" + std::to_string(i), "D=A", "@LCL", "M=M-D"
    };
    auto result = translator.translate_pop(PopableSegment::LCL, i);
    ASSERT_THAT(result, Eq(expected_result));
}

TEST_F(VMTranslator, UpdatesStackAfterPopToNamedSegment) {
    auto instructions = translator.translate_pop(PopableSegment::LCL, 5);
    auto result = run_simulator(instructions, "poplocal.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesPushingFromStaticSegment) {
    uint16_t i = 5;
    std::string filename {"pong.vm"};

    InstList expected_result {
        "@" + filename + "." + std::to_string(i),
        "D=M"
    };
    append_push_D(expected_result);
	translator.set_filename(filename);
    auto result = translator.translate_push(PushableSegment::STATIC, i);
    ASSERT_THAT(result, Eq(expected_result));
}

TEST_F(VMTranslator, UpdatesStackAFterPushingFromStaticSegment) {
    std::string filename {"test.vm"};
	translator.set_filename(filename);
    auto instructions = translator.translate_push(PushableSegment::STATIC, 5);
    auto result = run_simulator(instructions, "pushstatic.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesPopToStaticSegment) {
    uint16_t i = 5;
    std::string filename {"test.vm"};
    InstList expected_result {
        "@SP", "AM=M-1", "D=M",
        "@" + filename + "." + std::to_string(i),
        "M=D"
    };

	translator.set_filename(filename);
    auto result = translator.translate_pop(PopableSegment::STATIC, i);
    ASSERT_THAT(result, Eq(expected_result));
}

TEST_F(VMTranslator, UpdatesStackAFterPopToStaticSegment) {
    std::string filename {"test.vm"};
	translator.set_filename(filename);
    auto instructions = translator.translate_pop(PopableSegment::STATIC, 5);
    auto result = run_simulator(instructions, "popstatic.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesPushingFromConstantSegment) {
    uint16_t i = 3459;

    InstList expected_result { "@" + std::to_string(i), "D=A"};
    append_push_D(expected_result);
    auto result = translator.translate_push(PushableSegment::CONSTANT, i);
    ASSERT_THAT(result, Eq(expected_result));
}

TEST_F(VMTranslator, UpdatesStackAFterPushingFromConstantSegment) {
    auto instructions = translator.translate_push(PushableSegment::CONSTANT, 5);
    auto result = run_simulator(instructions, "pushconstant.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesFileWithBinaryAluCommands) {
    translator.translate(EXPECTED_DATA_DIR / "sub.vm");
    auto result = run_simulator("sub.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesFileWithRelationalCommands) {
    translator.translate(EXPECTED_DATA_DIR / "eq.vm");
    auto result = run_simulator("eq.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesFileUnaryAluCommands) {
    translator.translate(EXPECTED_DATA_DIR / "not.vm");
    auto result = run_simulator("not.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesFileWithPushFromPhysicalSegment) {
    translator.translate(EXPECTED_DATA_DIR / "pushargument.vm");
    auto result = run_simulator("pushargument.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesFileWithPopToPhysicalSegment) {
    translator.translate(EXPECTED_DATA_DIR / "popargument.vm");
    auto result = run_simulator("popargument.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesFileWithPushFromStaticSegment) {
    translator.translate(EXPECTED_DATA_DIR / "pushstatic.vm");
    auto result = run_simulator("pushstatic.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesFileWithPopToStaticSegment) {
    translator.translate(EXPECTED_DATA_DIR / "popstatic.vm");
    auto result = run_simulator("popstatic.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesFileWithPushFromConstantSegment) {
    translator.translate(EXPECTED_DATA_DIR / "pushconstant.vm");
    auto result = run_simulator("pushconstant.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}
