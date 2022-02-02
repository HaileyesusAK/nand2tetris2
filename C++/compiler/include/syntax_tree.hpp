#ifndef __SYNTAX_TREE__
#define __SYNTAX_TREE__

#include <fstream>
#include <memory>
#include <sstream>
#include <vector>
#include "token.hpp"

namespace ntt {

    class SyntaxTree;

    using Tree = std::shared_ptr<SyntaxTree>;

    class SyntaxTree {

        public:
            SyntaxTree(const std::string& label) : label_(label) {}

            virtual std::string to_xml(int level = 0);

            void add_child(Tree tree);

        protected:
            void write_line(std::ostringstream&, const std::string&, int level);

        private:
            std::string label_;

            std::vector<Tree> children_;
    };

    class Leaf final : public SyntaxTree {
        public:
            Leaf(const Token& token) : SyntaxTree(""), token_(token) {}

            std::string to_xml(int level) override;

        private:
            Token token_;
    };
}

#endif
