#ifndef __SYNTAX_TREE__
#define __SYNTAX_TREE__

#include <fstream>
#include <memory>
#include <type_traits>
#include <vector>
#include "token.hpp"

namespace ntt {

    template<typename T, std::enable_if_t<std::is_same_v<T, std::string> |
                                       std::is_same_v<T, Token>, bool> = true>
    class Syntax {

        public:
            Syntax(const T& value) : value_(value) {}
            std::string to_xml(int level = 0);
            void add_child(std::shared_ptr<Syntax<T>> tree) {
                children_.push_back(tree);
            }

        private:
            T value_;

            template<typename R, std::enable_if_t<std::is_same_v<R, std::string> |
                                               std::is_same_v<R, Token>, bool> = true>
            std::conditional_t<std::is_same_v<T, std::string>, std::vector<Syntax<R>>, bool> children_;
    };

}

#endif
