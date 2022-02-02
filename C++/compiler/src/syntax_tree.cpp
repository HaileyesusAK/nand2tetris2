#include <iomanip>
#include <string>
#include <sstream>

#include "syntax_tree.hpp"
#include "tokenizer.hpp"

constexpr int INDENT_WIDTH = 4;

namespace ntt {

    void SyntaxTree::write_line(std::ostringstream& oss, const std::string& s, int level) {
        oss << std::setw(s.size() + INDENT_WIDTH * level) << s << std::endl;
    }

    std::string SyntaxTree::to_xml(int level) {
        std::ostringstream oss;

        write_line(oss, "<" + label_ + ">", level);

        for(auto &sub_tree : children_)
            oss << sub_tree->to_xml(level + 1);

        write_line(oss, "</" + label_ + ">", level);

        return oss.str();
    }
    
    void SyntaxTree::add_child(Tree tree) {
        children_.push_back(tree);
    }

    std::string Leaf::to_xml(int level) {
        std::ostringstream oss;
        write_line(oss, token_.to_xml(), level);
        return oss.str();
    }
}
