#include "gmock/gmock.h"
#include "command_types.h"
#include "factory.h"

using namespace std;
using namespace vm_command;

class CommandFactory : public testing::Test {
	public:
		Factory factory {"testfile.vim"};
};

TEST_F(CommandFactory, CreatesALUCommand) {
    auto command_ptr = factory.create_instance("add");
    ASSERT_NE(dynamic_cast<Add*>(command_ptr.get()), nullptr);
}

TEST_F(CommandFactory, CreatesPushCommand) {
    auto command_ptr = factory.create_instance("push argument 5");
    ASSERT_NE(dynamic_cast<Push*>(command_ptr.get()), nullptr);
}

TEST_F(CommandFactory, CreatesPopCommand) {
    auto command_ptr = factory.create_instance("pop local 5");
    ASSERT_NE(dynamic_cast<Pop*>(command_ptr.get()), nullptr);
}

TEST_F(CommandFactory, CreatesPushStaticCommand) {
    auto command_ptr = factory.create_instance("push static 5");
    ASSERT_NE(dynamic_cast<PushStatic*>(command_ptr.get()), nullptr);
}

TEST_F(CommandFactory, CreatesPopStaticCommand) {
    auto command_ptr = factory.create_instance("pop static 5");
    ASSERT_NE(dynamic_cast<PopStatic*>(command_ptr.get()), nullptr);
}

TEST_F(CommandFactory, CreatesFunctionCommand) {
    auto command_ptr = factory.create_instance("function add 2");
    ASSERT_NE(dynamic_cast<Function*>(command_ptr.get()), nullptr);
}

TEST_F(CommandFactory, CreatesReturnCommand) {
    auto command_ptr = factory.create_instance("return");
    ASSERT_NE(dynamic_cast<Return*>(command_ptr.get()), nullptr);
}

TEST_F(CommandFactory, CreatesCallCommand) {
    auto command_ptr = factory.create_instance("call add 2");
    ASSERT_NE(dynamic_cast<Call*>(command_ptr.get()), nullptr);
}

TEST_F(CommandFactory, CreatesLabelCommand) {
    auto command_ptr = factory.create_instance("label END");
    ASSERT_NE(dynamic_cast<Label*>(command_ptr.get()), nullptr);
}

TEST_F(CommandFactory, CreatesGotoCommand) {
    auto command_ptr = factory.create_instance("goto END");
    ASSERT_NE(dynamic_cast<Goto*>(command_ptr.get()), nullptr);
}

TEST_F(CommandFactory, CreatesIfGotoCommand) {
    auto command_ptr = factory.create_instance("if-goto END");
    ASSERT_NE(dynamic_cast<IfGoto*>(command_ptr.get()), nullptr);
}

TEST_F(CommandFactory, ValidatesCommandType) {
    ASSERT_THROW(factory.create_instance("blabla"), invalid_argument);
}

TEST_F(CommandFactory, ValidatesCommandIndex) {
    ASSERT_THROW(factory.create_instance("push local 123456"), invalid_argument);
}
