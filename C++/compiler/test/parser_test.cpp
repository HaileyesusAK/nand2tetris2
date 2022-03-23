#include <fstream>
#include <iostream>
#include <filesystem>
#include "expression.hpp"
#include "expression_list.hpp"
#include "gmock/gmock.h"
#include "let_statement.hpp"
#include "parser.hpp"
#include "token.hpp"
#include "tokenizer.hpp"
#include "syntax_tree.hpp"
#include "utils.hpp"

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
            return cmp_xml<Expression>(input_file, expected_output_file);
        }

        bool parse_exp_list(std::string input_file, std::string expected_output_file) {
            return cmp_xml<ExpressionList>(input_file, expected_output_file);
        }

        bool parse_let_statement(std::string input_file, std::string expected_output_file) {
            return cmp_xml<LetStatement>(input_file, expected_output_file);
        }

        bool parse_do_statement(std::string input_file, std::string expected_output_file) {
            return cmp_xml(get_parser(input_file).parse_do_statement(), expected_output_file);
        }

        bool parse_return_statement(std::string input_file, std::string expected_output_file) {
            return cmp_xml(get_parser(input_file).parse_return_statement(), expected_output_file);
        }

        bool parse_if_statement(std::string input_file, std::string expected_output_file) {
            return cmp_xml(get_parser(input_file).parse_if_statement(), expected_output_file);
        }

        bool parse_while_statement(std::string input_file, std::string expected_output_file) {
            return cmp_xml(get_parser(input_file).parse_while_statement(), expected_output_file);
        }

        bool parse_var_dec(std::string input_file, std::string expected_output_file) {
            return cmp_xml(get_parser(input_file).parse_var_dec(), expected_output_file);
        }

        bool parse_parameter_list(std::string input_file, std::string expected_output_file) {
            return cmp_xml(get_parser(input_file).parse_parameter_list(), expected_output_file);
        }

        bool parse_subroutine_body(std::string input_file, std::string expected_output_file) {
            return cmp_xml(get_parser(input_file).parse_subroutine_body(), expected_output_file);
        }

        bool parse_subroutine_dec(std::string input_file, std::string expected_output_file) {
            return cmp_xml(get_parser(input_file).parse_subroutine_dec(), expected_output_file);
        }

        bool parse_class_var_dec(std::string input_file, std::string expected_output_file) {
            return cmp_xml(get_parser(input_file).parse_class_var_dec(), expected_output_file);
        }

        bool parse_class(std::string input_file, std::string expected_output_file) {
            return cmp_xml(get_parser(input_file).parse_class(), expected_output_file);
        }

        bool cmpFiles(const fs::path& p1, const fs::path& p2) {
            ifstream file1(p1), file2(p2);

            string line1, line2;
            while(!file1.eof() && !file2.eof()) {
                std::getline(file1, line1);
                std::getline(file2, line2);
                
                if(line1 != line2)
                    return false;
            }

            return (file1.eof() && file2.eof());
        }

        bool parse(const fs::path& jack_file_path, const fs::path& expected_file_path) {
            fs::path input_path = DATA_DIR / jack_file_path;
            fs::path output_path { input_path };
            output_path.replace_extension(".xml");

            {
                ifstream ifs {input_path};
                ofstream ofs {output_path};
                ofs << Parser{ifs}.parse_class()->to_xml();
            }

            fs::path expected_output_path = DATA_DIR / fs::path{"expected"} / expected_file_path;
            return cmpFiles(output_path, expected_output_path);
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

        template <typename Parser>
        bool cmp_xml(const std::string& input_file, const std::string& expected_output_file) {
            fs::path input_path { DATA_DIR / input_file };
            ifstream ifs {input_path};
            Tokenizer tokenizer {ifs};

            fs::path output_file { DATA_DIR / "tmp.xml" };
            {
                ofstream ofs {output_file};
                ofs << Parser(tokenizer).to_xml();
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

TEST_F(FParser, HandlesIfStatement) {
    ASSERT_THAT(parse_if_statement("if.jack", "if.xml"), Eq(true));
}

TEST_F(FParser, HandlesIfElseStatement) {
    ASSERT_THAT(parse_if_statement("if_else.jack", "if_else.xml"), Eq(true));
}

TEST_F(FParser, HandlesWhileStatement) {
    ASSERT_THAT(parse_while_statement("while.jack", "while.xml"), Eq(true));
}

TEST_F(FParser, HandlesSingleVariableDeclaration) {
    ASSERT_THAT(parse_var_dec("single_var_dec.jack", "single_var_dec.xml"), Eq(true));
}

TEST_F(FParser, HandlesMultiVariableDeclaration) {
    ASSERT_THAT(parse_var_dec("multi_var_dec.jack", "multi_var_dec.xml"), Eq(true));
}

TEST_F(FParser, HandlesParameterList) {
    ASSERT_THAT(parse_parameter_list("parameter_list.jack", "parameter_list.xml"), Eq(true));
}

TEST_F(FParser, HandlesSubroutineDec) {
    ASSERT_THAT(parse_subroutine_dec("subroutine_dec.jack", "subroutine_dec.xml"), Eq(true));
}

TEST_F(FParser, HandlesSubroutineBody) {
    ASSERT_THAT(parse_subroutine_body("subroutine_body.jack", "subroutine_body.xml"), Eq(true));
}

TEST_F(FParser, HandlesClassVarDec){
    ASSERT_THAT(parse_class_var_dec("class_var_dec.jack", "class_var_dec.xml"), Eq(true));
}

TEST_F(FParser, HandlesEmptyClass){
    ASSERT_THAT(parse_class("class_empty.jack", "class_empty.xml"), Eq(true));
}

TEST_F(FParser, HandlesClassWithData){
    ASSERT_THAT(parse_class("class_data.jack", "class_data.xml"), Eq(true));
}

TEST_F(FParser, HandlesClassWitMethod){
    ASSERT_THAT(parse_class("class_method.jack", "class_method.xml"), Eq(true));
}

TEST_F(FParser, ParsesSquareDanceMain){
    /*
       Main.jack: initializes and starts a new "square dance" session.
     */

    fs::path input_rel_path = fs::path{"Square"} / fs::path{"Main.jack"};
    fs::path expected_output_rel_path = fs::path{"Square"} / fs::path{"Main.xml"};
    ASSERT_THAT(parse(input_rel_path, expected_output_rel_path), Eq(true));
}

TEST_F(FParser, ParsesSquareDanceSquare){
    /*
       Square.jack: implements an animated square.
     */

    fs::path input_rel_path = fs::path{"Square"} / fs::path{"Square.jack"};
    fs::path expected_output_rel_path = fs::path{"Square"} / fs::path{"Square.xml"};
    ASSERT_THAT(parse(input_rel_path, expected_output_rel_path), Eq(true));
}

TEST_F(FParser, ParsesSquareDanceSquareGame){
    /*
       SquareGame.jack: runs the show according to the game rules.
     */

    fs::path input_rel_path = fs::path{"Square"} / fs::path{"SquareGame.jack"};
    fs::path expected_output_rel_path = fs::path{"Square"} / fs::path{"SquareGame.xml"};
    ASSERT_THAT(parse(input_rel_path, expected_output_rel_path), Eq(true));
}

TEST_F(FParser, ParsesArrayMain){
    /*
       Main.jack: computes the average of a user-supplied sequence of integers using an
       array data structure and array manipulation command
     */

    fs::path input_rel_path = fs::path{"ArrayTest"} / fs::path{"Main.jack"};
    fs::path expected_output_rel_path = fs::path{"ArrayTest"} / fs::path{"Main.xml"};
    ASSERT_THAT(parse(input_rel_path, expected_output_rel_path), Eq(true));
}
