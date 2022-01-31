#include <fstream>
#include <filesystem>
#include <stdexcept>
#include "tokenizer.hpp"
#include "token.hpp"
#include "gmock/gmock.h"

using namespace ntt;
using namespace std;
using namespace testing;
namespace fs = std::filesystem;

const fs::path DATA_DIR = fs::path{TEST_DIR} / "data";

class TokenizerTester : public Test {
    public:
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

        bool tokenize(const fs::path& jack_file_path, const fs::path& expected_file_path) {
            fs::path input_path = DATA_DIR / jack_file_path;
            fs::path output_path { input_path };
            output_path.replace_extension(".xml");

            ifstream ifs {input_path};
            ofstream ofs {output_path};

            Tokenizer(ifs).to_xml(ofs);
            fs::path expected_output_path = DATA_DIR / fs::path{"expected"} / expected_file_path;
            return cmpFiles(output_path, expected_output_path);
        }
};


TEST_F(TokenizerTester, InvalidFileStream) {
    ifstream ifs;
    ofstream ofs;

    ASSERT_THROW(Tokenizer(ifs).to_xml(ofs), runtime_error);
}

TEST_F(TokenizerTester, HandlesSymbols) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "{" << endl;
    }

    ifstream ifs {file_name};
    auto tokenizer = Tokenizer(ifs);
    auto token = tokenizer.get();
    ASSERT_THAT(token.value(), Eq("{"));
    ASSERT_THAT(token.type(), Eq(TokenType::SYMBOL));
}

TEST_F(TokenizerTester, HandlesSymbolsAfterSingleLineComment) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "// comment ..." << endl;
        ofs << ".";
    }

    ifstream ifs {file_name};
    Tokenizer tokenizer(ifs);
    auto token = tokenizer.get();
    ASSERT_THAT(token.value(), Eq("."));
    ASSERT_THAT(token.type(), Eq(TokenType::SYMBOL));
}

TEST_F(TokenizerTester, IgnoresWhitespaceCharacters) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "\t\n\r\f\v ,";
    }

    ifstream ifs {file_name};
    Tokenizer tokenizer {ifs};
    auto token = tokenizer.get();
    ASSERT_THAT(token.value(), Eq(","));
    ASSERT_THAT(token.type(), Eq(TokenType::SYMBOL));
}

TEST_F(TokenizerTester, HandlesKeywords) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "// comment ..." << endl;
        ofs << "// comment ..." << endl;
        ofs << "field";
    }

    ifstream ifs {file_name};
    Tokenizer tokenizer {ifs};
    auto token = tokenizer.get();
    ASSERT_THAT(token.value(), Eq("field"));
    ASSERT_THAT(token.type(), Eq(TokenType::KEYWORD));
}

TEST_F(TokenizerTester, HandlesIdentifiers) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "age_of_empire_;//...";
    }

    ifstream ifs {file_name};
    Tokenizer tokenizer {ifs};
    auto token = tokenizer.get();
    ASSERT_THAT(token.value(), Eq("age_of_empire_"));
    ASSERT_THAT(token.type(), Eq(TokenType::IDENTIFIER));
}

TEST_F(TokenizerTester, HandlesIntegers) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "123" << endl;
    }

    ifstream ifs {file_name};
    Tokenizer tokenizer {ifs};
    auto token = tokenizer.get();
    ASSERT_THAT(token.value(), Eq("123"));
    ASSERT_THAT(token.type(), Eq(TokenType::INTEGER));
}

TEST_F(TokenizerTester, HandlesStrings) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "\"str\"";
    }

    ifstream ifs {file_name};
    Tokenizer tokenizer {ifs};
    auto token = tokenizer.get();
    ASSERT_THAT(token.value(), Eq("str"));
    ASSERT_THAT(token.type(), Eq(TokenType::STRING));
}

TEST_F(TokenizerTester, ThrowsExceptionForUnterminatedString) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "\"str";
    }

    ifstream ifs {file_name};
    ASSERT_THROW(Tokenizer {ifs}, domain_error);
}

TEST_F(TokenizerTester, ThrowsExceptionForNewlineInString) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "\"str\nstr\"";
    }

    ifstream ifs {file_name};
    ASSERT_THROW(Tokenizer {ifs}, domain_error);
}

TEST_F(TokenizerTester, ThrowsExceptionForUnknownToken) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "123456";
    }

    ifstream ifs {file_name};
    ASSERT_THROW(Tokenizer {ifs}, domain_error);
}

TEST_F(TokenizerTester, HandlesMultilineComments) {
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
    Tokenizer tokenizer {ifs};
    auto token = tokenizer.get();
    ASSERT_THAT(token.type(), Eq(TokenType::SYMBOL));
    ASSERT_THAT(token.value(), Eq("~"));
}

/*
   Square Dance: a simple interactive application, described in project 9.
   The implementation is organized in three classes:
 */
TEST_F(TokenizerTester, ParsesSquareDanceMain){
    /*
       Main.jack: initializes and starts a new "square dance" session.
     */


    fs::path input_rel_path = fs::path{"Square"} / fs::path{"Main.jack"};
    fs::path expected_output_rel_path = fs::path{"Square"} / fs::path{"MainT.xml"};
    ASSERT_THAT(tokenize(input_rel_path, expected_output_rel_path), Eq(true));
}

TEST_F(TokenizerTester, ParsesSquareDanceSquare){
    /*
       Square.jack: implements an animated square.
     */

    fs::path input_rel_path = fs::path{"Square"} / fs::path{"Square.jack"};
    fs::path expected_output_rel_path = fs::path{"Square"} / fs::path{"SquareT.xml"};
    ASSERT_THAT(tokenize(input_rel_path, expected_output_rel_path), Eq(true));
}

TEST_F(TokenizerTester, ParsesSquareDanceSquareGame){
    /*
       SquareGame.jack: runs the show according to the game rules.
     */

    fs::path input_rel_path = fs::path{"Square"} / fs::path{"SquareGame.jack"};
    fs::path expected_output_rel_path = fs::path{"Square"} / fs::path{"SquareGameT.xml"};
    ASSERT_THAT(tokenize(input_rel_path, expected_output_rel_path), Eq(true));
}

TEST_F(TokenizerTester, ParsesArrayMain){
    /*
       Main.jack: computes the average of a user-supplied sequence of integers using an
       array data structure and array manipulation command
     */

    fs::path input_rel_path = fs::path{"ArrayTest"} / fs::path{"Main.jack"};
    fs::path expected_output_rel_path = fs::path{"ArrayTest"} / fs::path{"MainT.xml"};
    ASSERT_THAT(tokenize(input_rel_path, expected_output_rel_path), Eq(true));
}

TEST_F(TokenizerTester, CanExtractToken) {
    string file {"test.jack"};
    {
        ofstream ofs {file};
        ofs << 42;
    }

    ifstream ifs {file};
    ASSERT_THAT(Tokenizer(ifs).get().value(), Eq("42"));
}

TEST_F(TokenizerTester, RecognizesEmptyTokenStream) {

    string file {"test.jack"};
    {
        ofstream ofs {file};
        ofs << 42;
    }

    ifstream ifs {file};
    Tokenizer tokenizer {ifs};
    tokenizer.get();
    ASSERT_THAT(Tokenizer(ifs).has_token(), Eq(false)); 
}

TEST_F(TokenizerTester, TellsIfTokenizerHasTokens) {
    string file {"test.jack"};
    {
        ofstream ofs {file};
        ofs << 42;
    }

    ifstream ifs {file};
    ASSERT_THAT(Tokenizer(ifs).has_token(), Eq(true)); 
}

TEST_F(TokenizerTester, CanPeekFrontOfTokenStream) {
    string file {"test.jack"};
    {
        ofstream ofs {file};
        ofs << 42;
    }

    ifstream ifs {file};
    Tokenizer tokenizer {ifs};
    tokenizer.peek();
    ASSERT_THAT(tokenizer.peek().value(), Eq("42"));
}

TEST_F(TokenizerTester, ThrowsExceptionWhenAttemptsToPeekOnEmptyTokenStream) {
    string file {"tmp.jack"};
    {
        ofstream ofs {file};
        ofs << endl;
    }
    ifstream ifs {"tmp.jack"};
    Tokenizer tokenizer {ifs};
    ASSERT_THROW(tokenizer.peek(), domain_error);
}
