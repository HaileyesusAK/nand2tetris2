#include <fstream>
#include "token.hpp"
#include "gmock/gmock.h"

using namespace ntt;
using namespace std;
using namespace testing;

TEST(TokenParser, HandlesEmptyStream) {
    ifstream ifs;
    auto token = Token::parse(ifs);
    ASSERT_THAT(token.type(), Eq(TokenType::UNKNOWN));
    ASSERT_THAT(token.value(), Eq(""));
}

TEST(TokenParser, HandlesSymbols) {
    string file_name {"test.jack"};
    {
        ofstream ofs {file_name};
        ofs << "{" << endl;
    }

    ifstream ifs {file_name};
    auto token = Token::parse(ifs);
    ASSERT_THAT(token.type(), Eq(TokenType::SYMBOL));
    ASSERT_THAT(token.value(), Eq("{"));
}
