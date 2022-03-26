#include <filesystem>
#include <fstream>
#include "do_statement.hpp"
#include "if_statement.hpp"
#include "gmock/gmock.h"
#include "let_statement.hpp"
#include "return_statement.hpp"
#include "statement_factory.hpp"
#include "while_statement.hpp"


using namespace ntt;
using namespace std;
using namespace testing;

namespace fs = std::filesystem;
const fs::path DATA_DIR = fs::path{TEST_DIR} / "data";

class FStatementFactory : public Test {
    public:
        std::unique_ptr<Statement> get_statement(string jack_file) {
            ifstream ifs { DATA_DIR / jack_file};
            Tokenizer tokenizer {ifs};
            return StatementFactory::parse(tokenizer);
        }
};

TEST_F(FStatementFactory, CreatesIntegerStatement) {
    ASSERT_NE(dynamic_cast<DoStatement*>(get_statement("do.jack").get()), nullptr);
}

TEST_F(FStatementFactory, CreatesLetStatement) {
    ASSERT_NE(dynamic_cast<LetStatement*>(get_statement("let_statement_variable.jack").get()), nullptr);
}

TEST_F(FStatementFactory, CreatesReturnStatement) {
    ASSERT_NE(dynamic_cast<ReturnStatement*>(get_statement("return_void.jack").get()), nullptr);
}

TEST_F(FStatementFactory, CreatesWhileStatement) {
    ASSERT_NE(dynamic_cast<WhileStatement*>(get_statement("while.jack").get()), nullptr);
}

TEST_F(FStatementFactory, CreatesIfStatement) {
    ASSERT_NE(dynamic_cast<IfStatement*>(get_statement("if.jack").get()), nullptr);
}
