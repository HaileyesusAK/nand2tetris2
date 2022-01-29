#ifndef __SYNTAX_TREE__
#define __SYNTAX_TREE__

#include <fstream>
#include <memory>
#include <sstream>
#include <vector>

#include "token.hpp"

namespace ntt {

    class SyntaxTree;

    using Tree = std::unique_ptr<SyntaxTree>;

    class SyntaxTree {

        public:
            explicit SyntaxTree(const Token& root) : root_(root) {}

            std::string to_xml(int level = 0);

        private:
            Token root_;

            std::string label_;

            std::vector<Tree> children_;

            void write_line(std::ostringstream&, const std::string&, int level);
    };
}

#endif
