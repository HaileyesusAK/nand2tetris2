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
        static inline const string CLASS_NAME = "Test";
        
        template <typename T>
        auto compile(CodeGenerator& generator, const std::string& jack_file) {
            ifstream ifs { DATA_DIR / jack_file };
            Tokenizer tokenizer { ifs };
            T jack_element { tokenizer };

            return generator.compile(jack_element);
        }

        template <typename T>
        bool compile(CodeGenerator& generator, const std::string& jack_file, const std::string& expected_vm_file) {
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
    CodeGenerator generator {CLASS_NAME};
	ASSERT_THAT(compile<ParameterList>(generator, "parameter_list_empty.jack"), Eq(0));
}

TEST_F(FCodeGenerator, CompilesNonEmptyParameterList) {
    CodeGenerator generator {CLASS_NAME};
	ASSERT_THAT(compile<ParameterList>(generator, "parameter_list.jack"), Eq(2));
}

TEST_F(FCodeGenerator, UpdatesSymbolTableWhenCompilingNonEmptyParameterList) {
    CodeGenerator generator {CLASS_NAME};

    compile<ParameterList>(generator, "parameter_list.jack");
    auto entry = generator.symbol_table().get_entry("a");

	ASSERT_THAT(entry.type, Eq("int"));
	ASSERT_THAT(entry.index, Eq(0));
	ASSERT_THAT(entry.kind, Eq(SymbolKind::ARGUMENT));
}

TEST_F(FCodeGenerator, HandlesDuplicateEntryParameterList) {
    CodeGenerator generator {CLASS_NAME};
    ASSERT_THROW(compile<ParameterList>(generator, "parameter_list_dup.jack"), runtime_error);
}

TEST_F(FCodeGenerator, CompilesMethodVariableDeclaration) {
    CodeGenerator generator {CLASS_NAME};
	ASSERT_THAT(compile<SubroutineVarDec>(generator, "multi_var_dec.jack"), Eq(2));
}

TEST_F(FCodeGenerator, UpdatesSymbolTableWhenCompilingVariableDeclaration) {
    CodeGenerator generator {CLASS_NAME};

    compile<SubroutineVarDec>(generator, "multi_var_dec.jack");
    auto entry = generator.symbol_table().get_entry("weight");

	ASSERT_THAT(entry.type, Eq("int"));
	ASSERT_THAT(entry.index, Eq(1));
	ASSERT_THAT(entry.kind, Eq(SymbolKind::LOCAL));
}

TEST_F(FCodeGenerator, HandlesDuplicateVariableDeclaration) {
    CodeGenerator generator {CLASS_NAME};
    ASSERT_THROW(compile<SubroutineVarDec>(generator, "multi_var_dec_dup.jack"), runtime_error);
}

TEST_F(FCodeGenerator, CompilesIntegerTerm) {
    CodeGenerator generator {CLASS_NAME};
	ASSERT_THAT(compile<IntegerTerm>(generator, "integer_term.jack", "integer_term.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesStringTerm) {
    CodeGenerator generator {CLASS_NAME};
	ASSERT_THAT(compile<StringTerm>(generator, "string_term.jack", "string_term.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesKeywordTerm) {
    CodeGenerator generator {CLASS_NAME};
	ASSERT_THAT(compile<KeywordTerm>(generator, "keyword_term.jack", "keyword_term.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesIdentifierTerm) {
    CodeGenerator generator {CLASS_NAME};
    compile<SubroutineVarDec>(generator, "multi_var_dec.jack");
	ASSERT_THAT(compile<IdentifierTerm>(generator, "identifier_term2.jack", "identifier_term2.vm"), Eq(true));
}

TEST_F(FCodeGenerator, HandlesUndeclaredIdentifierTerm) {
    CodeGenerator generator {CLASS_NAME};
    compile<SubroutineVarDec>(generator, "multi_var_dec.jack");
    ASSERT_THROW(compile<IdentifierTerm>(generator, "identifier_term.jack", "identifier_term.vm"), runtime_error);
}

TEST_F(FCodeGenerator, CompilesArrayTerm) {
    {
        ofstream ofs { DATA_DIR / "tmp.jack" };
        ofs << "var Array Point;";
    }

    CodeGenerator generator {CLASS_NAME};
    compile<SubroutineVarDec>(generator, "tmp.jack");
	ASSERT_THAT(compile<ArrayTerm>(generator, "array_term.jack", "array_term.vm"), Eq(true));
}

TEST_F(FCodeGenerator, HandlesUndeclaredArrayVariable) {
    {
        ofstream ofs { DATA_DIR / "tmp.jack" };
        ofs << "var Array blabla;";
    }

    CodeGenerator generator {CLASS_NAME};
    compile<SubroutineVarDec>(generator, "tmp.jack");
    ASSERT_THROW(compile<ArrayTerm>(generator, "array_term.jack", "array_term.vm"), runtime_error);
}

TEST_F(FCodeGenerator, CompilesParenthesizedTerm) {
    CodeGenerator generator {CLASS_NAME};
	ASSERT_THAT(compile<ParenthesizedTerm>(generator, "parenthesized_term.jack", "parenthesized_term.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesSubroutineCallTerm) {
    {
        ofstream ofs { DATA_DIR / "tmp.jack" };
        ofs << "var int bmi;";
    }

    CodeGenerator generator {CLASS_NAME};
    compile<SubroutineVarDec>(generator, "tmp.jack");
	ASSERT_THAT(compile<SubroutineCallTerm>(generator, "subroutine_call.jack", "subroutine_call.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesMethodCallTerm) {
    CodeGenerator generator {CLASS_NAME};

    {
        ofstream ofs { DATA_DIR / "tmp.jack" };
        ofs << "var int a, b;";
    }
    compile<SubroutineVarDec>(generator, "tmp.jack");

    {
        ofstream ofs { DATA_DIR / "tmp.jack" };
        ofs << "var Shape square;";
    }
    compile<SubroutineVarDec>(generator, "tmp.jack");

	ASSERT_THAT(compile<MethodCallTerm>(generator, "method_call.jack", "method_call.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesStaticMethodCallTerm) {
    CodeGenerator generator {CLASS_NAME};
	ASSERT_THAT(compile<MethodCallTerm>(generator, "static_method_call.jack", "static_method_call.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesUnaryOpTerm) {
    CodeGenerator generator {CLASS_NAME};
	ASSERT_THAT(compile<UnaryOpTerm>(generator, "unaryop_terms.jack", "unaryop_terms.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesSimpleLetStatement) {
    CodeGenerator generator {CLASS_NAME};
    {
        ofstream ofs { DATA_DIR / "tmp.jack" };
        ofs << "var int age;";
    }
    compile<SubroutineVarDec>(generator, "tmp.jack");
	ASSERT_THAT(compile<LetStatement>(generator, "let_statement_variable.jack", "let_statement_variable.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesArrayAssignmentLetStatement) {
    CodeGenerator generator {CLASS_NAME};
    {
        ofstream ofs { DATA_DIR / "tmp.jack" };
        ofs << "var int x, y;";
    }
    compile<SubroutineVarDec>(generator, "tmp.jack");

    {
        ofstream ofs { DATA_DIR / "tmp.jack" };
        ofs << "var Array Point;";
    }
    compile<SubroutineVarDec>(generator, "tmp.jack");

	ASSERT_THAT(compile<LetStatement>(generator, "let_statement_array.jack", "let_statement_array.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesDoStatement) {
    CodeGenerator generator {CLASS_NAME};
	ASSERT_THAT(compile<DoStatement>(generator, "do_method_call.jack", "do_method_call.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesReturnStatement) {
    CodeGenerator generator {CLASS_NAME};
	ASSERT_THAT(compile<ReturnStatement>(generator, "return_exp.jack", "return_exp.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesWhileStatement) {
    CodeGenerator generator {CLASS_NAME};
    {
        ofstream ofs { DATA_DIR / "tmp.jack" };
        ofs << "var int a;";
    }
    compile<SubroutineVarDec>(generator, "tmp.jack");
	ASSERT_THAT(compile<WhileStatement>(generator, "while.jack", "while.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesIfStatement) {
    CodeGenerator generator {CLASS_NAME};
    {
        ofstream ofs { DATA_DIR / "tmp.jack" };
        ofs << "var int a;";
    }
    compile<SubroutineVarDec>(generator, "tmp.jack");
	ASSERT_THAT(compile<IfStatement>(generator, "if.jack", "if.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesIfElseStatement) {
    CodeGenerator generator {CLASS_NAME};
    {
        ofstream ofs { DATA_DIR / "tmp.jack" };
        ofs << "var int a,b;";
    }
    compile<SubroutineVarDec>(generator, "tmp.jack");
	ASSERT_THAT(compile<IfStatement>(generator, "if_else.jack", "if_else.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesSubroutineBody) {
    CodeGenerator generator {CLASS_NAME};
	ASSERT_THAT(compile<SubroutineBody>(generator, "subroutine_body2.jack", "subroutine_body2.vm"), Eq(true));
}

TEST_F(FCodeGenerator, CompilesStaticClassVariableDeclaration) {
    CodeGenerator generator {CLASS_NAME};
	ASSERT_THAT(compile<SubroutineBody>(generator, "subroutine_body2.jack", "subroutine_body2.vm"), Eq(true));
    
    compile<ClassVarDec>(generator, "static_class_var_dec.jack");
    auto entry = generator.symbol_table().get_entry("height");

	ASSERT_THAT(entry.type, Eq("int"));
	ASSERT_THAT(entry.index, Eq(1));
	ASSERT_THAT(entry.kind, Eq(SymbolKind::STATIC));
}
