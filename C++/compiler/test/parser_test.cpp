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

TEST(Parser, HandlesIntegerConstantTerm) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "5";
    }

    ifstream ifs {file_name};
    auto parser = Parser(ifs);
    auto tree = parser.parse_term();
    ASSERT_THAT(tree->to_xml(), "<term>\n    <integerConstant> 5 </integerConstant>\n</term>\n");
}

TEST(Parser, HandlesStringConstantTerm) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "\"ciao\"";
    }

    ifstream ifs {file_name};
    auto parser = Parser(ifs);
    auto tree = parser.parse_term();
    ASSERT_THAT(tree->to_xml(), "<term>\n    <stringConstant> ciao </stringConstant>\n</term>\n");
}

TEST(Parser, HandlesKeywordConstantTerm) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "this";
    }

    ifstream ifs {file_name};
    auto parser = Parser(ifs);
    auto tree = parser.parse_term();
    ASSERT_THAT(tree->to_xml(), "<term>\n    <keyword> this </keyword>\n</term>\n");
}

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
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "age";
    }

    ifstream ifs {file_name};
    auto parser = Parser(ifs);
    auto tree = parser.parse_term();
    ASSERT_THAT(tree->to_xml(), "<term>\n    <identifier> age </identifier>\n</term>\n");
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
