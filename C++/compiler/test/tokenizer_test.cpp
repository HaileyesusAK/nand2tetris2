#include <fstream>
#include <filesystem>
#include <stdexcept>
#include "tokenizer.hpp"
#include "gmock/gmock.h"

using namespace ntt;
using namespace std;
using namespace testing;
namespace fs = std::filesystem;

fs::path DATA_DIR = fs::path{TEST_DIR} / "data";


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
