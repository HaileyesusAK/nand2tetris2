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

class FParser : public Test {

    public:
        bool parse_term(std::string input_file, std::string expected_output_file) {
            fs::path input_path { DATA_DIR / input_file };
            ifstream ifs {input_path};
            auto parser = ntt::Parser(ifs);
            auto tree = parser.parse_term();

            fs::path output_file { DATA_DIR / "tmp.xml" };
            {
                ofstream ofs {output_file};
                ofs << tree->to_xml();
            }

            return Utils::cmpFiles(output_file, DATA_DIR / expected_output_file);
        }

        bool parse_exp(std::string input_file, std::string expected_output_file) {
            fs::path input_path { DATA_DIR / input_file };
            ifstream ifs {input_path};
            auto parser = ntt::Parser(ifs);
            auto tree = parser.parse_exp();

            fs::path output_file { DATA_DIR / "tmp.xml" };
            {
                ofstream ofs {output_file};
                ofs << tree->to_xml();
            }

            return Utils::cmpFiles(output_file, DATA_DIR / expected_output_file);
        }

        bool parse_exp_list(std::string input_file, std::string expected_output_file) {
            fs::path input_path { DATA_DIR / input_file };
            ifstream ifs {input_path};
            auto parser = ntt::Parser(ifs);
            auto tree = parser.parse_exp_list();

            fs::path output_file { DATA_DIR / "tmp.xml" };
            {
                ofstream ofs {output_file};
                ofs << tree->to_xml();
            }

            return Utils::cmpFiles(output_file, DATA_DIR / expected_output_file);
        }
         
};

TEST_F(FParser, ThrowsExceptionForInvalidKeywordConstant) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "int";
    }

    ifstream ifs {file_name};
    auto parser = Parser(ifs);
    ASSERT_THROW(parser.parse_term(), runtime_error);
}

TEST_F(FParser, HandlesIdentifierTerm) {
    ASSERT_THAT(parse_term("identifier_term.jack", "identifier_term.xml"), Eq(true));
}

TEST_F(FParser, HandlesExpression) {
    ASSERT_THAT(parse_exp("expression.jack", "expression.xml"), Eq(true));
}

TEST_F(FParser, HandlesArrayTerm) {
    ASSERT_THAT(parse_term("array_term.jack", "array_term.xml"), Eq(true));
}

TEST_F(FParser, HandlesParenthesizedTerm) {
    ASSERT_THAT(parse_term("parenthesized_term.jack", "parenthesized_term.xml"), Eq(true));
}

TEST_F(FParser, HandlesUnaryTerm) {
    ASSERT_THAT(parse_term("unaryop_terms.jack", "unaryop_terms.xml"), Eq(true));
}

TEST_F(FParser, HandlesExpressionList) {
    ASSERT_THAT(parse_exp_list("expression_list.jack", "expression_list.xml"), Eq(true));
}

TEST_F(FParser, HandlesSubroutineCall) {
    ASSERT_THAT(parse_term("subroutine_call.jack", "subroutine_call.xml"), Eq(true));
}

TEST_F(FParser, HandlesMethodCall) {
    ASSERT_THAT(parse_term("method_call.jack", "method_call.xml"), Eq(true));
}
