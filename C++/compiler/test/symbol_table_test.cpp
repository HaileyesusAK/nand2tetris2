#include "symbol_table.hpp"
#include "gmock/gmock.h"

using namespace ntt;
using namespace testing;

TEST(SymbolTable, CanInsertANewSymbol) {
    SymbolTable table;

    ASSERT_THAT(table.insert("x", "int", SymbolKind::FIELD), Eq(true));
}

TEST(SymbolTable, DoesNotInsertADuplicateSymbol) {
    SymbolTable table;

    table.insert("x", "int", SymbolKind::FIELD);
    ASSERT_THAT(table.insert("x", "int", SymbolKind::FIELD), Eq(false));
}

TEST(SymbolTable, IsAwareOfScope) {
    SymbolTable table;

    table.insert("x", "int", SymbolKind::FIELD);
    ASSERT_THAT(table.insert("x", "int", SymbolKind::ARGUMENT), Eq(true));
}

TEST(SymbolTable, RetrievingGivesPreferenceToSubroutineScope) {
    SymbolTable table;

    table.insert("y", "int", SymbolKind::FIELD);
    table.insert("x", "char", SymbolKind::ARGUMENT);
    table.insert("y", "char", SymbolKind::ARGUMENT);
    ASSERT_THAT(table.get_index("y"), Eq(1));
}

TEST(SymbolTable, CanRetrieveSymbol) {
    SymbolTable table;

    table.insert("x", "char", SymbolKind::ARGUMENT);
    table.insert("x", "int", SymbolKind::FIELD);
    table.insert("y", "int", SymbolKind::FIELD);
    ASSERT_THAT(table.get_index("y"), Eq(1));
}
