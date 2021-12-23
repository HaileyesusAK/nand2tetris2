#include <exception>
#include <filesystem>
#include <fstream>
#include "gmock/gmock.h"
#include "assembler.h"

using namespace testing;
namespace fs = std::filesystem;
static const fs::path DATA_DIR = fs::path(TEST_DIR) / "data";

class HackAssembler : public Test {
    public:
        Assembler assembler;

		bool cmpFiles(const fs::path& p1, const fs::path& p2) {
            std::ifstream file1(p1), file2(p2);

            std::string line1, line2;
            while(!file1.eof() && !file2.eof()) {
                std::getline(file1, line1);
                std::getline(file2, line2);
                if(line1 != line2)
                    return false;
            }

            return (file1.eof() && file2.eof());
        }
};

TEST_F(HackAssembler, TranslatesFile) {
    std::string input_file_name = "Pong.asm";
    std::string output_file_name = "Pong.hack";

    auto expected_file = DATA_DIR / "expected" / output_file_name;
    auto output_file = DATA_DIR / output_file_name;

    assembler.translate(DATA_DIR / input_file_name);
    ASSERT_THAT(cmpFiles(expected_file, output_file),  Eq(true));
}
