#include <fstream>
#include "parser.hpp"
#include "token.hpp"
#include "gmock/gmock.h"

using namespace ntt;
using namespace std;
using namespace testing;


TEST(Parser, HandlesIntegerConstantTerm) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "5";
    }

    ifstream ifs {file_name};
    auto parser = Parser(ifs);
    auto tree = parser.parse_term();
    ASSERT_THAT(tree->to_xml(), "<integerConstant> 5 </integerConstant>\n");
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
    ASSERT_THAT(tree->to_xml(), "<stringConstant> ciao </stringConstant>\n");
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
    ASSERT_THAT(tree->to_xml(), "<keyword> this </keyword>\n");
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
    ASSERT_THAT(tree->to_xml(), "<identifier> age </identifier>\n");
}
