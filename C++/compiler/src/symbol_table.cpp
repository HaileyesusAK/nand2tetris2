#include <string>
#include <utility>
#include "symbol_table.hpp"

namespace ntt {

    bool SymbolTable::insert(const std::string& name, const std::string& type, const SymbolKind& kind) {
        uint16_t index;
        Scope scope;

        switch(kind) {
            case SymbolKind::STATIC:
                index = static_index_++;
                scope = Scope::CLASS;
            break;

            case SymbolKind::FIELD:
                index = field_index_++;
                scope = Scope::CLASS;
            break;

            case SymbolKind::ARGUMENT:
                index = argument_index_++;
                scope = Scope::SUBROUTINE;
            break;

            case SymbolKind::LOCAL:
                index = local_index_++;
                scope = Scope::SUBROUTINE;
            break;
        }

        return table_.insert(std::make_pair<Key, SymbolTableEntry>(std::make_pair(name, scope),
                                            {type, kind, index})).second;
    }

    const SymbolTableEntry& SymbolTable::get_entry(const std::string& name) const {
        try {
            return table_.at(std::make_pair(name, Scope::SUBROUTINE));
        }
        catch (std::out_of_range& err) {
            return table_.at(std::make_pair(name, Scope::CLASS));
        }
    }

    uint16_t SymbolTable::get_index(const std::string& name) const {
        return get_entry(name).index;
    }

    const SymbolKind& SymbolTable::get_kind(const std::string& name) const {
        return get_entry(name).kind;
    }

    const std::string& SymbolTable::get_type(const std::string& name) const {
        return get_entry(name).type;
    }

}
