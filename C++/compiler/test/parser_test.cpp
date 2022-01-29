#include <fstream>
#include <filesystem>
#include "parser.hpp"
#include "token.hpp"
#include "utils.hpp"
#include "gmock/gmock.h"

using namespace ntt;
using namespace std;
using namespace testing;
namespace fs = std::filesystem;

const fs::path DATA_DIR = fs::path{TEST_DIR} / "data";

TEST(Parser, ThrowsExceptionForInvalidKeywordConstant) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "int";
    }

    ifstream ifs {file_name};
    auto parser = Parser(ifs);
    ASSERT_THROW(parser.parse_term(), runtime_error);
}

TEST(Parser, HandlesIdentifierTerm) {
    fs::path input_file { DATA_DIR / "identifier_term.jack" };
    ifstream ifs {input_file};
    auto parser = Parser(ifs);
    auto tree = parser.parse_term();

    fs::path output_file { DATA_DIR / "tmp.xml" };
    {
        ofstream ofs {output_file};
        ofs << tree->to_xml();
    }

    ASSERT_THAT(Utils::cmpFiles(output_file, DATA_DIR / "identifier_term.xml"), Eq(true));
}

TEST(Parser, HandlesExpression) {
    fs::path input_file { DATA_DIR / "expression.jack" };
    ifstream ifs {input_file};
    auto parser = Parser(ifs);
    auto tree = parser.parse_exp();

    fs::path output_file { DATA_DIR / "tmp.xml" };
    {
        ofstream ofs {output_file};
        ofs << tree->to_xml();
    }

    ASSERT_THAT(Utils::cmpFiles(output_file, DATA_DIR / "expression.xml"), Eq(true));
}
