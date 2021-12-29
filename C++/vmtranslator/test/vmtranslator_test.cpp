#include <algorithm>
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
using InstList = std::vector<std::string>;

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

        std::pair<std::string, int> run_simulator(const std::string& vm_file) {
            fs::path vm_path = EXPECTED_DATA_DIR / vm_file;

            VmTranslator translator;
            translator.translate(vm_path);

            fs::path tst_path = vm_path;
            tst_path.replace_extension(".tst");

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

        std::string create_tmp_file(const std::vector<std::string>& content) {
            auto tmpf = std::tmpfile();

            //Linux-specific way of retrieving the file path
            auto filepath = fs::read_symlink(fs::path("/proc/self/fd") / std::to_string(fileno(tmpf)));
            std::ofstream ofs {filepath};
            std::copy(content.begin(),
                      content.end(),
                      std::ostream_iterator<std::string>(ofs, "\n"));

            return filepath;
        }

        bool cmpFiles(const fs::path& p1, const fs::path& p2) {
            std::ifstream file1(p1), file2(p2);

            std::string line1, line2;
            while(!file1.eof() && !file2.eof()) {
                std::getline(file1, line1);
                std::getline(file2, line2);
                if(line1 != line2) {
                    return false;
                }
            }

            return (file1.eof() && file2.eof());
        }

        bool translates_to_asm(const std::string& vm_file,
                               const std::vector<std::string>& expected_asm_content) {

            auto vm_path = EXPECTED_DATA_DIR / vm_file;
            auto asm_path = vm_path;
            asm_path.replace_extension(".asm");
            auto expected_asm_file = create_tmp_file(expected_asm_content);

            translator.translate(vm_path);
            return cmpFiles(expected_asm_file, asm_path);
        }

};

TEST_F(VMTranslator, TranslatesBinaryArithmeticCommands) {
    auto matched = translates_to_asm("sub.vm", { "@SP", "AM=M-1", "D=M", "@SP", "A=M-1", "M=M-D" });
    ASSERT_THAT(matched, Eq(true));
}

TEST_F(VMTranslator, UpdatesStackAfterBinaryArithmeticCommandTranslation) {
    auto result = run_simulator("sub.vm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesRelationalCommands) {
    auto matched = translates_to_asm("eq.vm", { "@SP", "AM=M-1", "D=M", "@SP", "A=M-1", "D=M-D",
                                                "M=-1", "@12", "D;JEQ", "@SP", "A=M-1", "M=0" });
    ASSERT_THAT(matched, Eq(true));
}

TEST_F(VMTranslator, UpdatesStackAfterRelationalCommandTranslation) {
    auto result = run_simulator("eq.vm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}


TEST_F(VMTranslator, TranslatesUnaryAluCommands) {
    auto matched = translates_to_asm("not.vm", { "@SP", "A=M-1", "M=!M" });
    ASSERT_THAT(matched, Eq(true));
}

TEST_F(VMTranslator, UpdatesStackAfterUnaryAluCommandTranslation) {
    auto result = run_simulator("not.vm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}


TEST_F(VMTranslator, TranslatesPushingFromNamedSegment) {
    InstList expected_result {
        "@ARG", "D=M", "@5", "A=D+A", "D=M"
    };
    append_push_D(expected_result);

    auto matched = translates_to_asm("pushargument.vm", expected_result);
    ASSERT_THAT(matched, Eq(true));
}

TEST_F(VMTranslator, UpdatesStackAfterPushingFromNamedSegment) {
    auto result = run_simulator("pushargument.vm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}


TEST_F(VMTranslator, TranslatesPopToNamedSegment) {
    InstList expected_result {
        "@5",  "D=A", "@ARG", "M=D+M",
        "@SP", "AM=M-1", "D=M",
        "@ARG", "A=M", "M=D", "@5", "D=A", "@ARG", "M=M-D"
    };

    auto matched = translates_to_asm("popargument.vm", expected_result);
    ASSERT_THAT(matched, Eq(true));
}

TEST_F(VMTranslator, UpdatesStackAfterPopToNamedSegment) {
    auto result = run_simulator("popargument.vm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesPushingFromStaticSegment) {
    InstList expected_result { "@pushstatic.vm.5", "D=M" };
    append_push_D(expected_result);

    auto matched = translates_to_asm("pushstatic.vm", expected_result);
    ASSERT_THAT(matched, Eq(true));
}

TEST_F(VMTranslator, UpdatesStackAFterPushingFromStaticSegment) {
    auto result = run_simulator("pushstatic.vm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesPopToStaticSegment) {
    auto matched = translates_to_asm("popstatic.vm", { "@SP", "AM=M-1", "D=M", "@popstatic.vm.5", "M=D" });
    ASSERT_THAT(matched, Eq(true));
}

TEST_F(VMTranslator, UpdatesStackAFterPopToStaticSegment) {
    auto result = run_simulator("popstatic.vm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesPushingFromConstantSegment) {
    InstList expected_result { "@5", "D=A"};
    append_push_D(expected_result);

    auto matched = translates_to_asm("pushconstant.vm", expected_result);
    ASSERT_THAT(matched, Eq(true));
}

TEST_F(VMTranslator, UpdatesStackAFterPushingFromConstantSegment) {
    auto result = run_simulator("pushconstant.vm");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}
