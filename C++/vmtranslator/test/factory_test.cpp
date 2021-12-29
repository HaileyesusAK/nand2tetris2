#include "gmock/gmock.h"
#include "add.h"
#include "push.h"
#include "pop.h"
#include "push_static.h"
#include "pop_static.h"
#include "factory.h"

using namespace std;
using namespace vm_command;

TEST(CommandFactory, CreatesALUCommand) {
    auto command_ptr = Factory::create_instance("add", "");
    ASSERT_NE(dynamic_cast<Add*>(command_ptr.get()), nullptr);
}

TEST(CommandFactory, CreatesPushCommand) {
    auto command_ptr = Factory::create_instance("push argument 5", "");
    ASSERT_NE(dynamic_cast<Push*>(command_ptr.get()), nullptr);
}

TEST(CommandFactory, CreatesPopCommand) {
    auto command_ptr = Factory::create_instance("pop local 5", "");
    ASSERT_NE(dynamic_cast<Pop*>(command_ptr.get()), nullptr);
}

TEST(CommandFactory, CreatesPushStaticCommand) {
    auto command_ptr = Factory::create_instance("push static 5", "pong.vm");
    ASSERT_NE(dynamic_cast<PushStatic*>(command_ptr.get()), nullptr);
}

TEST(CommandFactory, CreatesPopStaticCommand) {
    auto command_ptr = Factory::create_instance("pop static 5", "pong.vm");
    ASSERT_NE(dynamic_cast<PopStatic*>(command_ptr.get()), nullptr);
}

TEST(CommandFactory, ValidatesCommandType) {
    ASSERT_THROW(Factory::create_instance("blabla", ""), invalid_argument);
}

TEST(CommandFactory, ValidatesCommandIndex) {
    ASSERT_THROW(Factory::create_instance("push local 123456", ""), invalid_argument);
}
