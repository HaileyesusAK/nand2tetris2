#include <fstream>
#include <iostream>
#include <filesystem>
#include "array_term.hpp"
#include "class.hpp"
#include "class_var_dec.hpp"
#include "do_statement.hpp"
#include "expression.hpp"
#include "expression_list.hpp"
#include "identifier_term.hpp"
#include "integer_term.hpp"
#include "if_statement.hpp"
#include "gmock/gmock.h"
#include "keyword_term.hpp"
#include "let_statement.hpp"
#include "method_call_term.hpp"
#include "parameter_list.hpp"
#include "parenthesized_term.hpp"
#include "parenthesized_term.hpp"
#include "return_statement.hpp"
#include "token.hpp"
#include "tokenizer.hpp"
#include "string_term.hpp"
#include "subroutine_body.hpp"
#include "subroutine_call_term.hpp"
#include "subroutine_dec.hpp"
#include "subroutine_var_dec.hpp"
#include "syntax_tree.hpp"
#include "unaryop_term.hpp"
#include "utils.hpp"
#include "while_statement.hpp"

using namespace ntt;
using namespace std;
using namespace testing;
namespace fs = std::filesystem;

const fs::path DATA_DIR = fs::path{TEST_DIR} / "data";

class FParser : public Test {
    public:
        template <typename T>
        bool parse(const std::string& input_file, const std::string& expected_output_file) {
            ifstream ifs { DATA_DIR / input_file };
            Tokenizer tokenizer {ifs};

            fs::path output_file { DATA_DIR / "tmp.xml" };
            {
                ofstream ofs {output_file};
                ofs << T(tokenizer).to_xml();
            }

            return Utils::cmpFiles(output_file, DATA_DIR / expected_output_file);
        }
};

TEST_F(FParser, HandlesIdentifierTerm) {
    ASSERT_THAT(parse<IdentifierTerm>("identifier_term.jack", "identifier_term.xml"), Eq(true));
}

TEST_F(FParser, HandlesIntegerTerm) {
    ASSERT_THAT(parse<IntegerTerm>("integer_term.jack", "integer_term.xml"), Eq(true));
}

TEST_F(FParser, HandlesString) {
    ASSERT_THAT(parse<StringTerm>("string_term.jack", "string_term.xml"), Eq(true));
}

TEST_F(FParser, HandlessKeyword) {
    ASSERT_THAT(parse<KeywordTerm>("keyword_term.jack", "keyword_term.xml"), Eq(true));
}

TEST_F(FParser, HandlesExpression) {
    ASSERT_THAT(parse<Expression>("expression.jack", "expression.xml"), Eq(true));
}

TEST_F(FParser, HandlesArrayTerm) {
    ASSERT_THAT(parse<ArrayTerm>("array_term.jack", "array_term.xml"), Eq(true));
}

TEST_F(FParser, HandlesParenthesizedTerm) {
    ASSERT_THAT(parse<ParenthesizedTerm>("parenthesized_term.jack", "parenthesized_term.xml"), Eq(true));
}

TEST_F(FParser, HandlesUnaryTerm) {
    ASSERT_THAT(parse<UnaryOpTerm>("unaryop_terms.jack", "unaryop_terms.xml"), Eq(true));
}

TEST_F(FParser, HandlesExpressionList) {
    ASSERT_THAT(parse<ExpressionList>("expression_list.jack", "expression_list.xml"), Eq(true));
}

TEST_F(FParser, HandlesSubroutineCall) {
    ASSERT_THAT(parse<SubroutineCallTerm>("subroutine_call.jack", "subroutine_call.xml"), Eq(true));
}

TEST_F(FParser, HandlesMethodCall) {
    ASSERT_THAT(parse<MethodCallTerm>("method_call.jack", "method_call.xml"), Eq(true));
}

TEST_F(FParser, HandlesVariableAssignment) {
    ASSERT_THAT(parse<LetStatement>("let_statement_variable.jack", "let_statement_variable.xml"), Eq(true));
}

TEST_F(FParser, HandlesArrayElementAssignment) {
    ASSERT_THAT(parse<LetStatement>("let_statement_array.jack", "let_statement_array.xml"), Eq(true));
}

TEST_F(FParser, HandlesDoStatement) {
    ASSERT_THAT(parse<DoStatement>("do.jack", "do.xml"), Eq(true));
}

TEST_F(FParser, HandlesDoStatementToMethod) {
    ASSERT_THAT(parse<DoStatement>("do_method_call.jack", "do_method_call.xml"), Eq(true));
}

TEST_F(FParser, HandlesVoidReturnStatement) {
    ASSERT_THAT(parse<ReturnStatement>("return_void.jack", "return_void.xml"), Eq(true));
}

TEST_F(FParser, HandlesIfStatement) {
    ASSERT_THAT(parse<IfStatement>("if.jack", "if.xml"), Eq(true));
}

TEST_F(FParser, HandlesIfElseStatement) {
    ASSERT_THAT(parse<IfStatement>("if_else.jack", "if_else.xml"), Eq(true));
}

TEST_F(FParser, HandlesWhileStatement) {
    ASSERT_THAT(parse<WhileStatement>("while.jack", "while.xml"), Eq(true));
}

TEST_F(FParser, HandlesSingleVariableDeclaration) {
    ASSERT_THAT(parse<SubroutineVarDec>("single_var_dec.jack", "single_var_dec.xml"), Eq(true));
}

TEST_F(FParser, HandlesMultiVariableDeclaration) {
    ASSERT_THAT(parse<SubroutineVarDec>("multi_var_dec.jack", "multi_var_dec.xml"), Eq(true));
}

TEST_F(FParser, HandlesParameterList) {
    ASSERT_THAT(parse<ParameterList>("parameter_list.jack", "parameter_list.xml"), Eq(true));
}

TEST_F(FParser, HandlesSubroutineDec) {
    ASSERT_THAT(parse<SubroutineDec>("subroutine_dec.jack", "subroutine_dec.xml"), Eq(true));
}

TEST_F(FParser, HandlesSubroutineBody) {
    ASSERT_THAT(parse<SubroutineBody>("subroutine_body.jack", "subroutine_body.xml"), Eq(true));
}

TEST_F(FParser, HandlesClassVarDec){
    ASSERT_THAT(parse<ClassVarDec>("class_var_dec.jack", "class_var_dec.xml"), Eq(true));
}

TEST_F(FParser, HandlesEmptyClass){
    ASSERT_THAT(parse<Class>("class_empty.jack", "class_empty.xml"), Eq(true));
}

TEST_F(FParser, HandlesClassWithData){
    ASSERT_THAT(parse<Class>("class_data.jack", "class_data.xml"), Eq(true));
}

TEST_F(FParser, HandlesClassWitMethod){
    ASSERT_THAT(parse<Class>("class_method.jack", "class_method.xml"), Eq(true));
}

TEST_F(FParser, ParsesSquareDanceMain){
    /*
       Main.jack: initializes and starts a new "square dance" session.
     */

    fs::path input_rel_path = fs::path{"Square"} / fs::path{"Main.jack"};
    fs::path expected_output_rel_path = fs::path {"expected"} / fs::path{"Square"} / fs::path{"Main.xml"};
    ASSERT_THAT(parse<Class>(input_rel_path, expected_output_rel_path), Eq(true));
}

TEST_F(FParser, ParsesSquareDanceSquare){
    /*
       Square.jack: implements an animated square.
     */

    fs::path input_rel_path = fs::path{"Square"} / fs::path{"Square.jack"};
    fs::path expected_output_rel_path = fs::path {"expected"} / fs::path{"Square"} / fs::path{"Square.xml"};
    ASSERT_THAT(parse<Class>(input_rel_path, expected_output_rel_path), Eq(true));
}

TEST_F(FParser, ParsesSquareDanceSquareGame){
    /*
       SquareGame.jack: runs the show according to the game rules.
     */

    fs::path input_rel_path = fs::path{"Square"} / fs::path{"SquareGame.jack"};
    fs::path expected_output_rel_path = fs::path {"expected"} / fs::path{"Square"} / fs::path{"SquareGame.xml"};
    ASSERT_THAT(parse<Class>(input_rel_path, expected_output_rel_path), Eq(true));
}

TEST_F(FParser, ParsesArrayMain){
    /*
       Main.jack: computes the average of a user-supplied sequence of integers using an
       array data structure and array manipulation command
     */

    fs::path input_rel_path = fs::path{"ArrayTest"} / fs::path{"Main.jack"};
    fs::path expected_output_rel_path = fs::path {"expected"} / fs::path{"ArrayTest"} / fs::path{"Main.xml"};
    ASSERT_THAT(parse<Class>(input_rel_path, expected_output_rel_path), Eq(true));
}
