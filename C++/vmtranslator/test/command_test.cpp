#include <string>
#include "gmock/gmock.h"
#include "command_types.h"
#include "utils.h"


using namespace std;
using namespace vm_command;

#define _Eq(x) (testing::Eq((x)))	// this is to avoid conflict between testing::Eq and vm_command::Eq


TEST(VMCommand, TranslatesAdd) {
    auto result = Utils::run_cpu_emulator(Add{}, "add.asm");
    ASSERT_THAT(result.second, _Eq(0)) << result.first;
}

TEST(VMCommand, TranslatesSub) {
    auto result = Utils::run_cpu_emulator(Sub{}, "sub.asm");
    ASSERT_THAT(result.second, _Eq(0)) << result.first;
}

TEST(VMCommand, TranslatesAnd) {
    auto result = Utils::run_cpu_emulator(And{}, "and.asm");
    ASSERT_THAT(result.second, _Eq(0)) << result.first;
}

TEST(VMCommand, TranslatesOr) {
    auto result = Utils::run_cpu_emulator(Or{}, "or.asm");
    ASSERT_THAT(result.second, _Eq(0)) << result.first;
}

TEST(VMCommand, TranslatesEq) {
    auto result = Utils::run_cpu_emulator(Eq{}, "eq.asm");
    ASSERT_THAT(result.second, _Eq(0)) << result.first;
}

TEST(VMCommand, TranslatesLt) {
    auto result = Utils::run_cpu_emulator(Lt{}, "lt.asm");
    ASSERT_THAT(result.second, _Eq(0)) << result.first;
}

TEST(VMCommand, TranslatesGt) {
    auto result = Utils::run_cpu_emulator(Gt{}, "gt.asm");
    ASSERT_THAT(result.second, _Eq(0)) << result.first;
}

TEST(VMCommand, TranslatesNot) {
    auto result = Utils::run_cpu_emulator(Not{}, "not.asm");
    ASSERT_THAT(result.second, _Eq(0)) << result.first;
}

TEST(VMCommand, TranslatesNeg) {
    auto result = Utils::run_cpu_emulator(Neg{}, "neg.asm");
    ASSERT_THAT(result.second, _Eq(0)) << result.first;
}

TEST(VMCommand, TranslatesPush) {
    auto result = Utils::run_cpu_emulator(Push {"argument", 5}, "pushargument.asm");
    ASSERT_THAT(result.second, _Eq(0)) << result.first;
}

TEST(VMCommand, ValidatesPushCommand) {
    ASSERT_THROW(Push("static ", 5), invalid_argument);
}

TEST(VMCommand, TranslatesPushStatic) {
    auto result = Utils::run_cpu_emulator(PushStatic {"pushstatic.vm", 5}, "pushstatic.asm");
    ASSERT_THAT(result.second, _Eq(0)) << result.first;
}

TEST(VMCommand, TranslatesPop) {
    auto result = Utils::run_cpu_emulator(Pop{"argument", 5}, "popargument.asm");
    ASSERT_THAT(result.second, _Eq(0)) << result.first;
}

TEST(VMCommand, ValidatesPopCommand) {
    ASSERT_THROW(Pop("constant", 5), invalid_argument);
}

TEST(VMCommand, TranslatesPopStatic) {
    auto result = Utils::run_cpu_emulator(PopStatic {"popstatic.vm", 5}, "popstatic.asm");
    ASSERT_THAT(result.second, _Eq(0)) << result.first;
}

TEST(VMCommand, CompactsCommand) {
    string command {"  push constant 7   // pushes 7 on the constant segment"};
    Command::compact(command);

    ASSERT_THAT(command, _Eq("push constant 7"));
}
