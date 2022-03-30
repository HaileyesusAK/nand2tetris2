#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include "code_generator.hpp"
#include "gmock/gmock.h"
#include "types.hpp"
#include "tokenizer.hpp"
#include "utils.hpp"

using namespace ntt;
using namespace std;
using namespace testing;

namespace fs = std::filesystem;

const fs::path DATA_DIR = fs::path{TEST_DIR} / "data";

class FCodeGenerator : public Test {
    public:
        CodeGenerator generator {"Test"};

        template <typename T> 
        auto compile(const std::string& jack_file) {
            ifstream ifs { DATA_DIR / jack_file };
            Tokenizer tokenizer { ifs };
            T jack_element { tokenizer };

            return generator.compile(jack_element);
        }

        template <typename T> 
        bool compile(const std::string& jack_file, const std::string& expected_vm_file) {
            ifstream ifs { DATA_DIR / jack_file };
            Tokenizer tokenizer { ifs };
            T jack_element { tokenizer };

            {
                ofstream ofs { DATA_DIR / "tmp.vm" };
                generator.compile(jack_element);
                std::copy(generator.vm_commands().begin(),
                          generator.vm_commands().end(),
                          ostream_iterator<string>(ofs, "\n"));
            }

            return Utils::cmpFiles(DATA_DIR / "tmp.vm", DATA_DIR / expected_vm_file);
        }
};

TEST_F(FCodeGenerator, CompilesEmptyParameterList) {
    ASSERT_THAT(compile<ParameterList>("parameter_list_empty.jack"), Eq(0));
}

TEST_F(FCodeGenerator, CompilesNonEmptyParameterList) {
    ASSERT_THAT(compile<ParameterList>("parameter_list.jack"), Eq(2));
}

TEST_F(FCodeGenerator, UpdatesSymbolTableWhenCompilingNonEmptyParameterList) {
    compile<ParameterList>("parameter_list.jack");
    auto entry = generator.symbol_table().get_entry("a");

    ASSERT_THAT(entry.type, Eq("int"));
    ASSERT_THAT(entry.index, Eq(0));
    ASSERT_THAT(entry.kind, Eq(SymbolKind::ARGUMENT));
}

TEST_F(FCodeGenerator, HandlesDuplicateEntryParameterList) {
    ASSERT_THROW(compile<ParameterList>("parameter_list_dup.jack"), runtime_error);
}

TEST_F(FCodeGenerator, CompilesMethodVariableDeclaration) {
    ASSERT_THAT(compile<SubroutineVarDec>("multi_var_dec.jack"), Eq(2));
}

TEST_F(FCodeGenerator, UpdatesSymbolTableWhenCompilingVariableDeclaration) {
    compile<SubroutineVarDec>("multi_var_dec.jack");
    auto entry = generator.symbol_table().get_entry("weight");

    ASSERT_THAT(entry.type, Eq("int"));
    ASSERT_THAT(entry.index, Eq(1));
    ASSERT_THAT(entry.kind, Eq(SymbolKind::LOCAL));
}

TEST_F(FCodeGenerator, HandlesDuplicateVariableDeclaration) {
    ASSERT_THROW(compile<SubroutineVarDec>("multi_var_dec_dup.jack"), runtime_error);
}

TEST_F(FCodeGenerator, CompilesIntegerTerm) {
    ASSERT_THAT(compile<IntegerTerm>("integer_term.jack", "integer_term.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesStringTerm) {
    ASSERT_THAT(compile<StringTerm>("string_term.jack", "string_term.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesKeywordTerm) {
    ASSERT_THAT(compile<KeywordTerm>("keyword_term.jack", "keyword_term.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesIdentifierTerm) {
    compile<SubroutineVarDec>("multi_var_dec.jack");
    ASSERT_THAT(compile<IdentifierTerm>("identifier_term2.jack", "identifier_term2.vm"), Eq(true));
}

TEST_F(FCodeGenerator, HandlesUndeclaredIdentifierTerm) {
    compile<SubroutineVarDec>("multi_var_dec.jack");
    ASSERT_THROW(compile<IdentifierTerm>("identifier_term.jack", "identifier_term.vm"), runtime_error);
}

TEST_F(FCodeGenerator, CompilesArrayTerm) {
    {
        ofstream ofs { DATA_DIR / "tmp.jack" };
        ofs << "var Array Point;";
    }

    compile<SubroutineVarDec>("tmp.jack");
    ASSERT_THAT(compile<ArrayTerm>("array_term.jack", "array_term.vm"), Eq(true));
}

TEST_F(FCodeGenerator, HandlesUndeclaredArrayVariable) {
    {
        ofstream ofs { DATA_DIR / "tmp.jack" };
        ofs << "var Array blabla;";
    }

    compile<SubroutineVarDec>("tmp.jack");
    ASSERT_THROW(compile<ArrayTerm>("array_term.jack", "array_term.vm"), runtime_error);
}

TEST_F(FCodeGenerator, CompilesParenthesizedTerm) {
    ASSERT_THAT(compile<ParenthesizedTerm>("parenthesized_term.jack", "parenthesized_term.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesSubroutineCallTerm) {
    {
        ofstream ofs { DATA_DIR / "tmp.jack" };
        ofs << "var int bmi;";
    }
    compile<SubroutineVarDec>("tmp.jack");
    ASSERT_THAT(compile<SubroutineCallTerm>("subroutine_call.jack", "subroutine_call.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesMethodCallTerm) {
    {
        ofstream ofs { DATA_DIR / "tmp.jack" };
        ofs << "var int a, b;";
    }
    compile<SubroutineVarDec>("tmp.jack");

    {
        ofstream ofs { DATA_DIR / "tmp.jack" };
        ofs << "var Shape square;";
    }
    compile<SubroutineVarDec>("tmp.jack");

    ASSERT_THAT(compile<MethodCallTerm>("method_call.jack", "method_call.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesStaticMethodCallTerm) {
    ASSERT_THAT(compile<MethodCallTerm>("static_method_call.jack", "static_method_call.vm"), Eq(true));
}
