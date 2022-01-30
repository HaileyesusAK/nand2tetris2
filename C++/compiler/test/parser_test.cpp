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
            return cmp_xml(get_parser(input_file).parse_term(), expected_output_file);
        }

        bool parse_exp(std::string input_file, std::string expected_output_file) {
            return cmp_xml(get_parser(input_file).parse_exp(), expected_output_file);
        }

        bool parse_exp_list(std::string input_file, std::string expected_output_file) {
            return cmp_xml(get_parser(input_file).parse_exp_list(), expected_output_file);
        }

        bool parse_let_statement(std::string input_file, std::string expected_output_file) {
            return cmp_xml(get_parser(input_file).parse_let_statement(), expected_output_file);
        }

        bool parse_do_statement(std::string input_file, std::string expected_output_file) {
            return cmp_xml(get_parser(input_file).parse_do_statement(), expected_output_file);
        }

        bool parse_return_statement(std::string input_file, std::string expected_output_file) {
            return cmp_xml(get_parser(input_file).parse_return_statement(), expected_output_file);
        }

    private:
        Parser get_parser(const std::string& input_file) {
            fs::path input_path { DATA_DIR / input_file };
            ifstream ifs {input_path};
            return ntt::Parser(ifs);
        }

        bool cmp_xml(Tree tree, const std::string& expected_output_file) {
        
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

TEST_F(FParser, HandlesVariableAssignment) {
    ASSERT_THAT(parse_let_statement("let_statement_variable.jack", "let_statement_variable.xml"), Eq(true));
}

TEST_F(FParser, HandlesArrayElementAssignment) {
    ASSERT_THAT(parse_let_statement("let_statement_array.jack", "let_statement_array.xml"), Eq(true));
}

TEST_F(FParser, HandlesDoStatement) {
    ASSERT_THAT(parse_do_statement("do.jack", "do.xml"), Eq(true));
}

TEST_F(FParser, HandlesVoidReturnStatement) {
    ASSERT_THAT(parse_return_statement("return_void.jack", "return_void.xml"), Eq(true));
}
