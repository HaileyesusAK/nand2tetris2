#include <filesystem>
#include <string>
#include "gmock/gmock.h"
#include "translator.h"
#include "utils.h"


using namespace testing;

namespace fs = std::filesystem;
const fs::path DATA_DIR = fs::path(TEST_DIR) / "data";
const fs::path EXPECTED_DATA_DIR = DATA_DIR / "expected";

class VMTranslator : public Test {
    public:
        Translator translator;

        CPUResult translate(const std::string &vm_file_name) {
            auto vm_path = EXPECTED_DATA_DIR / (vm_file_name + ".vm");
            translator.translate(vm_path);

            return Utils::run_cpu_emulator(vm_file_name + ".asm");
        }

        CPUResult translate_dir(const std::string &dir_name) {
            auto dir_path = EXPECTED_DATA_DIR / dir_name;
            auto asm_path = dir_path / (dir_name + ".asm");
            translator.translate(dir_path);

            std::string asm_file = dir_name + "/" + dir_name + ".asm";

            return Utils::run_cpu_emulator(asm_file);
        }

};

TEST_F(VMTranslator, CreatesOutputFileFromFile) {
	auto vm_file_path = EXPECTED_DATA_DIR / "SimpleAdd.vm";
	auto asm_file_path = vm_file_path;
	asm_file_path.replace_extension(".asm");
	fs::remove(asm_file_path);	//It should be created by the translator

	translator.translate(vm_file_path);
	ASSERT_TRUE(fs::exists(asm_file_path)) << asm_file_path << " is not created";
}

TEST_F(VMTranslator, CreatesOutputFileFromDirectory) {
	auto dir_path = EXPECTED_DATA_DIR / "TranslatorTest";
	auto asm_file_path = dir_path / "TranslatorTest.asm";
	fs::remove(asm_file_path);	//It should be created by the translator

	translator.translate(dir_path);
	ASSERT_TRUE(fs::exists(asm_file_path)) << asm_file_path << " is not created";
}

TEST_F(VMTranslator, TranslatesSimpleAdd) {
/*
    SimpleAdd.vm pushes two constants onto the stack and adds them up.
*/
    auto result = translate("SimpleAdd");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesStackTest) {
/*
    StackTest.vm executes a sequence of arithmetic and logical operations on the stack.
*/
    auto result = translate("StackTest");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesBasicTest) {
/*
    BasicTest.vm executes push/pop operations using the virtual memory segments
    constant, local, argument, this, that, and temp.
*/
    auto result = translate("BasicTest");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesStaticTest) {
/*
    StaticTest.vm executes push/pop operations using the virtual memory segment static.
*/
    auto result = translate("StaticTest");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesPointerTest) {
/*
    PonterTest.vm executes push/pop operations using the virtual memory segments
    pointer, this, and that.
*/
    auto result = translate("PointerTest");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesFunctionDeclaration) {
/*
    SimpleFunction.vm performs a simple calculation and returns the result.
    This program provides a basic test of the implementation of the VM
    commands function and return.
*/
    auto result = translate("SimpleFunction");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}

TEST_F(VMTranslator, TranslatesNestedCalls) {
    auto result = translate_dir("NestedCall");
    ASSERT_THAT(result.second, Eq(0)) << result.first;
}
