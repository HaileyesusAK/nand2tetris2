#include <iomanip>
#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <variant>

#include "syntax_tree.hpp"
#include "tokenizer.hpp"

constexpr int INDENT_WIDTH = 4;

using namespace std;

namespace ntt {

    void SyntaxTree::write_line(std::ostringstream& oss, const std::string& s, int level) const {
        oss << std::setw(s.size() + INDENT_WIDTH * level) << s << std::endl;
    }

    void SyntaxTree::open_tag(std::ostringstream& oss, int level) const {
        write_line(oss, "<" + label_ + ">", level);
    }

    void SyntaxTree::close_tag(std::ostringstream& oss, int level) const {
        write_line(oss, "</" + label_ + ">", level);
    }

    std::string SyntaxTree::to_xml(int level) const {
        std::ostringstream oss;
        
        struct Item {
            Node node;
            bool opened;
        };

        struct NodePrinter {
            std::ostringstream& oss;
            std::stack<Item>& stack;
            int level;

            void operator()(const Token& token) {
                //It must be the case that (stack.top().node == tree) is true
                oss << std::setw(token.to_xml().size() + INDENT_WIDTH * level) << token.to_xml() << std::endl;
                stack.pop();
            }

            void operator()(const Tree& tree) {
                auto top = stack.top(); //It must be the case that (top.node == tree) is true
                stack.pop();

                if(top.opened) {
                    level--;
                    tree->close_tag(oss, level);
                }
                else {
                    tree->open_tag(oss, level);
                    level++;
                    top.opened = true;
                    stack.push(top);

                    for(auto it = tree->children_.rbegin(); it != tree->children_.rend(); ++it) {
                        stack.push({*it, false});
                    }
                }
            }
        };

        std::stack<Item> stack;
        NodePrinter printer{oss, stack, level};

        stack.push({std::make_shared<SyntaxTree>(*this), false});
        while(!stack.empty()) {
            std::visit(printer, stack.top().node);
        }

        return oss.str();
    }

    void SyntaxTree::add_child(const Node& node) {
        children_.push_back(node);
    }
}
