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
static const fs::path TOOLS_DIR = fs::path(TEST_DIR) / "tools";

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

        std::pair<std::string, int> run_simulator(const std::vector<std::string>& asm_instructions,
                                                  const std::string& asm_file)
        {
            fs::path asm_path = EXPECTED_DATA_DIR / asm_file;
            std::ofstream ofs {asm_path};
            for(const auto& inst : asm_instructions)
                ofs << inst << std::endl;

            fs::path tst_path = asm_path;
            tst_path.replace_extension(".tst");

            fs::path cmp_path = asm_path;
            cmp_path.replace_extension(".cmp");

            fs::path executable_path = TOOLS_DIR / "CPUEmulator.sh";
            std::ostringstream cmd_os;
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
};

TEST_F(VMTranslator, TranslatesBinaryArithmeticCommands) {
    std::vector<std::string> expected_result {
        "@SP", "AM=M-1", "D=M", "@SP", "A=M-1", "M=M-D"};
    auto result = translator.translate(BinaryAluOp::SUB);
    ASSERT_THAT(result, Eq(expected_result));
}

TEST_F(VMTranslator, UpdatesStackAfterBinaryArithmeticCommandTranslation) {
    auto result = run_simulator(translator.translate(BinaryAluOp::SUB), "sub.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesRelationalCommands) {
    std::vector<std::string> expected_result {
        "@SP", "AM=M-1", "D=M",
        "@SP", "A=M-1", "D=M-D", "M=-1",
        "@12", "D;JEQ", "@SP", "A=M-1", "M=0"
    };
    auto result = translator.translate(RelOp::EQ, 0);
    ASSERT_THAT(result, Eq(expected_result));
}

TEST_F(VMTranslator, UpdatesStackAfterRelationalCommandTranslation) {
    auto result = run_simulator(translator.translate(RelOp::EQ, 0), "eq.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesUnaryAluCommands) {
    std::vector<std::string> expected_result { "@SP", "A=M-1", "M=!M" };
    auto result = translator.translate(UnaryOp::NOT);
    ASSERT_THAT(result, Eq(expected_result));
}

TEST_F(VMTranslator, UpdatesStackAfterUnaryAluCommandTranslation) {
    auto result = run_simulator(translator.translate(UnaryOp::NOT), "not.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
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

TEST_F(VMTranslator, UpdatesStackAfterPushingFromNamedSegment) {
    auto instructions = translator.translate_push(Segment::ARG, 5);
    auto result = run_simulator(instructions, "pushargument.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesPushingFromStaticSegment) {
    uint16_t i = 5;
    std::string filename {"pong.vm"};

    std::vector<std::string> expected_result {
        "@" + filename + "." + std::to_string(i),
        "D=M"
    };
    append_push_D(expected_result);

    auto result = translator.translate_push_static(filename, i);
    ASSERT_THAT(result, Eq(expected_result));
}

TEST_F(VMTranslator, UpdatesStackAFterPushingFromStaticSegment) {
    auto instructions = translator.translate_push_static("test.vm", 5);
    auto result = run_simulator(instructions, "pushstatic.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesPushingFromConstantSegment) {
    uint16_t i = 3459;

    std::vector<std::string> expected_result { "@" + std::to_string(i), "D=A"};
    append_push_D(expected_result);
    auto result = translator.translate_push_constant(i);
    ASSERT_THAT(result, Eq(expected_result));
}

TEST_F(VMTranslator, UpdatesStackAFterPushingFromConstantSegment) {
    auto instructions = translator.translate_push_constant(5);
    auto result = run_simulator(instructions, "pushconstant.asm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}
