#include <stdexcept>
#include "gmock/gmock.h"
#include "symbol_table.h"

using namespace std;
using namespace testing;

TEST(HackSymbolTable, HasPredefinedSymbols) {
    SymbolTable table;

    ASSERT_THAT(table.get("SCREEN"), Eq(16384));
}

TEST(HackSymbolTable, CanAddVariable) {
    SymbolTable table;
    table.insert("sum");

    ASSERT_THAT(table.insert("avg"), Eq(17)); // variables start from address 16 onward
}

TEST(HackSymbolTable, DoesNotAddDuplicateVariable) {
    SymbolTable table;
    table.insert("sum");
    table.insert("avg");

    ASSERT_THAT(table.insert("sum"), Eq(16));
}

TEST(HackSymbolTable, CanAddLabel) {
    SymbolTable table;

    ASSERT_THAT(table.insert("END", 38), Eq(38));
}

TEST(HackSymbolTable, DoesNotAddDuplicateLabel) {
    SymbolTable table;

    table.insert("END", 38);
    ASSERT_THAT(table.insert("END", 50), Eq(38));
}

TEST(HackSymbolTable, CanQueryLabelAddress) {
    SymbolTable table;

    table.insert("END", 38);
    ASSERT_THAT(table.get("END"), Eq(38));
}

TEST(HackSymbolTable, CanQueryVariableAddress) {
    SymbolTable table;

    table.insert("i");
    table.insert("j");
    ASSERT_THAT(table.get("j"), Eq(17));
}

TEST(HackSymbolTable, FailsWhenQueryingUnknownSymbol) {
    SymbolTable table;
    ASSERT_THROW(table.get("j"), runtime_error);
}
