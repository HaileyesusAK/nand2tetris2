#include <iomanip>
#include <iostream>
#include <stack>
#include <deque>
#include <string>
#include <sstream>
#include <variant>
#include "syntax_tree.hpp"
#include "tokenizer.hpp"
#include "types.hpp"

constexpr int INDENT_WIDTH = 4;

using namespace std;

namespace ntt {

    struct Item {
        Node node;
        bool opened;
    };

    class NodePrinter {
        public:
            NodePrinter(std::ostringstream& oss) : oss_(oss) {}

            void print(const Tree& tree, int level) {
                stack_.clear();
                level_ = level;

                stack_.push_back({tree, false});
                while(!stack_.empty()) {
                    // print the current node and push all its children onto the stack_
                    std::visit(*this, stack_.back().node);
                }
            }

            void operator()(const Token& token) {
                //It must be the case that (stack_.top().node == token) is true
                oss_ << token.to_xml(level_, INDENT_WIDTH) << std::endl;
                stack_.pop_back();
            }

            void operator()(const Tree& tree) {
                auto top = stack_.back(); //It must be the case that (top.node == tree) is true
                stack_.pop_back();

                if(top.opened) {
                    level_--;
                    tree->close_tag(oss_, level_);
                }
                else {
                    tree->open_tag(oss_, level_);
                    level_++;
                    top.opened = true;
                    stack_.push_back(top);

                    for(auto it = tree->children_.rbegin(); it != tree->children_.rend(); ++it) {
                        stack_.push_back({*it, false});
                    }
                }
            }

        private:
            std::ostringstream& oss_;
            std::deque<Item> stack_;
            int level_;
    };

    void SyntaxTree::write_line(std::ostringstream& oss, const std::string& s, int level) const {
        oss << std::setw(s.size() + INDENT_WIDTH * level) << s << std::endl;
    }

    void SyntaxTree::open_tag(std::ostringstream& oss, int level) const {
        write_line(oss, "<" + SyntaxTree::tags_.at(gen_type_) + ">", level);
    }

    void SyntaxTree::close_tag(std::ostringstream& oss, int level) const {
        write_line(oss, "</" + SyntaxTree::tags_.at(gen_type_) + ">", level);
    }

    std::string SyntaxTree::to_xml(int level) const {
        std::ostringstream oss;
        NodePrinter printer {oss};

        printer.print(std::make_shared<SyntaxTree>(*this), level);
        return oss.str();
    }

    void SyntaxTree::add_child(const Node& node) {
        children_.push_back(node);
    }

    void SyntaxTree::set_gen_type(const CodeGeneratorType& type) {
        gen_type_ = type;
    }
    
    const std::vector<Node>& SyntaxTree::children() const {
        return children_;
    }
    
    CodeGeneratorType SyntaxTree::gen_type() const {
        return gen_type_;
    }
}
