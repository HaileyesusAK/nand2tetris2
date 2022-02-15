#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include <cstdint>
#include <string>
#include <unordered_map>

namespace ntt {
    enum class Scope { CLASS, SUBROUTINE };

    enum class SymbolKind { FIELD, STATIC, LOCAL, ARGUMENT };

    struct SymbolTableEntry {
        std::string type;
        SymbolKind kind;
        uint16_t index;
    };

    using Key = std::pair<std::string, Scope>;

    //std::unordered_map uses std::hash for its key but std::hash doesn't work for std::pair
    struct KeyHash {
        template <class T1, class T2>
        std::size_t operator() (const std::pair<T1, T2> &pair) const noexcept {
            return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
        }
    };

    class SymbolTable {
        public:
            bool insert(const std::string& name, const std::string& type, const SymbolKind& kind);
            uint16_t get_index(const std::string& name);
            const SymbolKind& get_kind(const std::string& name);
            const std::string& get_type(const std::string& name);
            const SymbolTableEntry& get_entry(const std::string& name);

        private:
            std::unordered_map<Key, SymbolTableEntry, KeyHash> table_;
            uint16_t static_index_= 0;
            uint16_t field_index_ = 0;
            uint16_t local_index_ = 0;
            uint16_t argument_index_ = 0;
    };
}
#endif
