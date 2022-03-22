#include <filesystem>
#include <fstream>
#include "array_term.hpp"
#include "gmock/gmock.h"
#include "identifier_term.hpp"
#include "integer_term.hpp"
#include "method_call_term.hpp"
#include "keyword_term.hpp"
#include "parenthesized_term.hpp"
#include "string_term.hpp"
#include "subroutine_call_term.hpp"
#include "tokenizer.hpp"
#include "utils.hpp"

using namespace ntt;
using namespace std;
using namespace testing;

namespace fs = std::filesystem;
const fs::path DATA_DIR = fs::path{TEST_DIR} / "data";

class FTerm : public Test {
    public:
        template <typename Parser>
        bool cmp_parser_output(string jack_file, string xml_file) {
            ifstream ifs { DATA_DIR / jack_file};
            Tokenizer tokenizer {ifs};

            fs::path output_file { DATA_DIR / "tmp.xml" };
            {
                ofstream ofs { output_file };
                ofs << Parser(tokenizer).to_xml();
            }

            return Utils::cmpFiles(output_file, DATA_DIR / xml_file);
        }
};

TEST_F(FTerm, ParsesInteger) {
    ASSERT_THAT(cmp_parser_output<IntegerTerm>("integer_term.jack", "integer_term.xml"), Eq(true));
}

TEST_F(FTerm, ParsesString) {
    ASSERT_THAT(cmp_parser_output<StringTerm>("string_term.jack", "string_term.xml"), Eq(true));
}

TEST_F(FTerm, ParsesKeyword) {
    ASSERT_THAT(cmp_parser_output<KeywordTerm>("keyword_term.jack", "keyword_term.xml"), Eq(true));
}

TEST_F(FTerm, ParsesIdentifier) {
    ASSERT_THAT(cmp_parser_output<IdentifierTerm>("identifier_term.jack", "identifier_term.xml"), Eq(true));
}

TEST_F(FTerm, ParsesArray) {
    ASSERT_THAT(cmp_parser_output<ArrayTerm>("array_term.jack", "array_term.xml"), Eq(true));
}

TEST_F(FTerm, ParsesParenthesized) {
    ASSERT_THAT(cmp_parser_output<ParenthesizedTerm>("parenthesized_term.jack", "parenthesized_term.xml"), Eq(true));
}

TEST_F(FTerm, ParsesSubroutine) {
    ASSERT_THAT(cmp_parser_output<SubroutineCallTerm>("subroutine_call.jack", "subroutine_call.xml"), Eq(true));
}

TEST_F(FTerm, ParsesMethod) {
    ASSERT_THAT(cmp_parser_output<MethodCallTerm>("method_call.jack", "method_call.xml"), Eq(true));
}
