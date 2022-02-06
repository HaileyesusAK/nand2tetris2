#ifndef __SYNTAX_TREE__
#define __SYNTAX_TREE__

#include <fstream>
#include <memory>
#include <sstream>
#include <vector>
#include <variant>
#include "token.hpp"

namespace ntt {

    class SyntaxTree;
    using Tree = std::shared_ptr<SyntaxTree>;
    using Node = std::variant<Token, Tree>;

    class SyntaxTree {

        public:
            SyntaxTree(const std::string& label) : label_(label) {}

            std::string to_xml(int level = 0) const;

            void add_child(const Node& node);


        private:
            std::string label_;

            std::vector<Node> children_;

            void write_line(std::ostringstream&, const std::string&, int level) const;

            void open_tag(std::ostringstream&, int) const;

            void close_tag(std::ostringstream&, int) const;
            
            friend class NodePrinter;
    };
}

#endif
