#include <filesystem>
#include <fstream>
#include "array_term.hpp"
#include "gmock/gmock.h"
#include "identifier_term.hpp"
#include "integer_term.hpp"
#include "keyword_term.hpp"
#include "parenthesized_term.hpp"
#include "string_term.hpp"
#include "term_factory.hpp"
#include "tokenizer.hpp"

using namespace ntt;
using namespace std;
using namespace testing;

namespace fs = std::filesystem;
const fs::path DATA_DIR = fs::path{TEST_DIR} / "data";

class FTermFactory : public Test {
    public:
        std::unique_ptr<Term> get_term(string jack_file) {
            ifstream ifs { DATA_DIR / jack_file};
            Tokenizer tokenizer {ifs};
            return TermFactory::parse(tokenizer);
        }
};

TEST_F(FTermFactory, CreatesIntegerTerm) {
    ASSERT_NE(dynamic_cast<IntegerTerm*>(get_term("integer_term.jack").get()), nullptr);
}

TEST_F(FTermFactory, CreatesStringTerm) {
    ASSERT_NE(dynamic_cast<StringTerm*>(get_term("string_term.jack").get()), nullptr);
}

TEST_F(FTermFactory, CreatesKeywordTerm) {
    ASSERT_NE(dynamic_cast<KeywordTerm*>(get_term("keyword_term.jack").get()), nullptr);
}

TEST_F(FTermFactory, CreatesIdentifierTerm) {
    ASSERT_NE(dynamic_cast<IdentifierTerm*>(get_term("identifier_term.jack").get()), nullptr);
}

TEST_F(FTermFactory, CreatesArrayTerm) {
    ASSERT_NE(dynamic_cast<ArrayTerm*>(get_term("array_term.jack").get()), nullptr);
}

TEST_F(FTermFactory, CreatesParenthesizedTerm) {
    ASSERT_NE(dynamic_cast<ParenthesizedTerm*>(get_term("parenthesized_term.jack").get()), nullptr);
}
