#include <fstream>
#include <regex>
#include <string>
#include <unordered_set>
#include "tokenizer.hpp"
#include <stdexcept>
#include <iostream>

namespace ntt {

    using Set = std::unordered_set<std::string>;

    Token Tokenizer::get_next() {

        static Set valid_symbols {"{", "}", "(", ")", "[", "]", ".", ",", ";", "+", "-", "*", "/", "&",
                                  "|", "<", ">", "=", "_", "~"};

        std::regex token_regex { R"(\{)" };

        if(tokens_.empty()) {
            std::string line;
            try {
                std::sregex_iterator pos { line.begin(), line.end(), token_regex };
                std::sregex_iterator end;
                while(std::getline(input_, line)) {
                    while(pos != end) {
                        tokens_.emplace(pos->str(), TokenType::SYMBOL);
                    }
                }
            }
            catch (const std::exception& ex) {
                std::cout << ex.what() << std::endl;
            }
        }

        auto token = tokens_.front();
        tokens_.pop();
        return token;
    }

}
