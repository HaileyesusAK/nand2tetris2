#include <filesystem>
#include <fstream>
#include "gmock/gmock.h"
#include "integer_term.hpp"
#include "tokenizer.hpp"
#include "utils.hpp"

using namespace ntt;
using namespace std;
using namespace testing;

namespace fs = std::filesystem;
const fs::path DATA_DIR = fs::path{TEST_DIR} / "data";

TEST(Term, ParsesInteger) {
    fs::path input_path { DATA_DIR / "integer_term.jack"};
    ifstream ifs {input_path};
    Tokenizer tokenizer {ifs};

    fs::path output_file { DATA_DIR / "tmp.xml" };
    {
        ofstream ofs { output_file };
        ofs << IntegerTerm(tokenizer).to_xml();
    }

    ASSERT_THAT(Utils::cmpFiles(output_file, DATA_DIR / "integer_term.xml"), Eq(true));
}
