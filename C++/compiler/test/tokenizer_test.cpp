#include <fstream>
#include <stdexcept>
#include "tokenizer.hpp"
#include "gmock/gmock.h"

using namespace ntt;
using namespace std;
using namespace testing;

TEST(Tokenizer, InvalidFileStream) {
    ifstream ifs;
    ofstream ofs;

    ASSERT_THROW(Tokenizer::parse(ifs, ofs), runtime_error);
}
