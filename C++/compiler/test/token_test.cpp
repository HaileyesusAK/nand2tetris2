#include <fstream>
#include <stdexcept>
#include "token.hpp"
#include "gmock/gmock.h"

using namespace ntt;
using namespace std;
using namespace testing;

TEST(TokenParser, HandlesEmptyStream) {
    ifstream ifs;
    auto token = Token::parse(ifs);
    ASSERT_THAT(token.value(), Eq(""));
    ASSERT_THAT(token.type(), Eq(TokenType::UNKNOWN));
}

TEST(TokenParser, HandlesSymbols) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "{" << endl;
    }

    ifstream ifs {file_name};
    auto token = Token::parse(ifs);
    ASSERT_THAT(token.value(), Eq("{"));
    ASSERT_THAT(token.type(), Eq(TokenType::SYMBOL));
}

TEST(TokenParser, HandlesSymbolsAfterSingleLineComment) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "// comment ..." << endl;
        ofs << ".";
    }

    ifstream ifs {file_name};
    auto token = Token::parse(ifs);
    ASSERT_THAT(token.value(), Eq("."));
    ASSERT_THAT(token.type(), Eq(TokenType::SYMBOL));
}

TEST(TokenParser, HandlesLineCommentWithoutNewLine) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "// comment ...";
    }

    ifstream ifs {file_name};
    auto token = Token::parse(ifs);
    ASSERT_THAT(token.value(), Eq(""));
    ASSERT_THAT(token.type(), Eq(TokenType::UNKNOWN));
}

TEST(TokenParser, IgnoresWhitespaceCharacters) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "\t\n\r\f\v ,";
    }

    ifstream ifs {file_name};
    auto token = Token::parse(ifs);
    ASSERT_THAT(token.value(), Eq(","));
    ASSERT_THAT(token.type(), Eq(TokenType::SYMBOL));
}

TEST(TokenParser, HandlesKeywords) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "// comment ..." << endl;
        ofs << "// comment ..." << endl;
        ofs << "field";
    }

    ifstream ifs {file_name};
    auto token = Token::parse(ifs);
    ASSERT_THAT(token.value(), Eq("field"));
    ASSERT_THAT(token.type(), Eq(TokenType::KEYWORD));
}

TEST(TokenParser, HandlesIdentifiers) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "age_of_empire_;//...";
    }

    ifstream ifs {file_name};
    auto token = Token::parse(ifs);
    ASSERT_THAT(token.value(), Eq("age_of_empire_"));
    ASSERT_THAT(token.type(), Eq(TokenType::IDENTIFIER));
}

TEST(TokenParser, HandlesIntegers) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "123" << endl;
    }

    ifstream ifs {file_name};
    auto token = Token::parse(ifs);
    ASSERT_THAT(token.value(), Eq("123"));
    ASSERT_THAT(token.type(), Eq(TokenType::INTEGER));
}

TEST(TokenParser, HandlesStrings) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "\"str\"";
    }

    ifstream ifs {file_name};
    auto token = Token::parse(ifs);
    ASSERT_THAT(token.value(), Eq("str"));
    ASSERT_THAT(token.type(), Eq(TokenType::STRING));
}

TEST(TokenParser, ThrowsExceptionForUnterminatedString) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "\"str";
    }

    ifstream ifs {file_name};
    ASSERT_THROW(Token::parse(ifs), domain_error);
}

TEST(TokenParser, ThrowsExceptionForNewlineInString) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "\"str\nstr\"";
    }

    ifstream ifs {file_name};
    ASSERT_THROW(Token::parse(ifs), domain_error);
}

TEST(TokenParser, ThrowsExceptionForUnknownToken) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "123456";
    }

    ifstream ifs {file_name};
    ASSERT_THROW(Token::parse(ifs), domain_error);
}

TEST(TokenParser, HandlesMultilineComments) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};

        ofs << "/*" << endl;
        ofs << "comment ... " << endl;
        ofs << "comment ... " << endl;
        ofs << "*/" << endl;

        ofs << "/*" << endl;
        ofs << "comment ... " << endl;
        ofs << "comment ... " << endl;
        ofs << "*/" << endl;
        ofs << "~";
    }

    ifstream ifs {file_name};
    auto token = Token::parse(ifs);
    ASSERT_THAT(token.type(), Eq(TokenType::SYMBOL));
    ASSERT_THAT(token.value(), Eq("~"));
}
