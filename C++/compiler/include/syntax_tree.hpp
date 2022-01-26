#ifndef __SYNTAX_TREE__
#define __SYNTAX_TREE_

#include <fstream>
#include <memory>
#include <vector>

#include "token.hpp"

namespace ntt {

    class SyntaxTree;

    using Tree = std::unique_ptr<SyntaxTree>;

    class SyntaxTree {

        public:
            SyntaxTree(const Token& root) : root_(root) {}

            void add_child(Tree sub_tree);

            static Tree parse(std::ifstream&);

        private:
            Token root_;
            std::vector<Tree> children_;

            static Tree parse_var_dec(std::ifstream&);

            static Token consume_type(std::ifstream&);

            static Token consume_identifier(std::ifstream&);

            static Token consume_symbol(std::ifstream&, const std::string&);

    };
}

#endif
