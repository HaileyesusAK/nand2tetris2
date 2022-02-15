#include <fstream>
#include <variant>
#include "code_generator.hpp"
#include "syntax_tree.hpp"
#include "symbol_table.hpp"


namespace ntt {

    /*
        classVarDec : ('static' | 'field') type varName(','varName)* ';'
    */
    void CodeGenerator::compile_class_var_dec(const SyntaxTree& tree) {
        // assert(tree.gen_type() == CodeGeneratorType::CLASS_VAR_DEC);

        auto kind = std::get<Token>(tree.children()[0]).value() == "static" ? SymbolKind::STATIC : SymbolKind::FIELD;
        const auto& type = std::get<Token>(tree.children()[1]).value();

        for(auto it = tree.children().begin() + 2; it != tree.children().end() - 1; it += 2) {
            symbol_table_.insert(std::get<Token>(*it).value(), type, kind);
        }
    }

    void CodeGenerator::write(std::ofstream& ofs) {
        for(const auto& vm_command : vm_commands_)
            ofs << vm_command << std::endl;
    }
}
