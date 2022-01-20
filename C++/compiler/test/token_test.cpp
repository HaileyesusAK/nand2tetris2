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
