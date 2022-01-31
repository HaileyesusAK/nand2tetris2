#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include <deque>
#include <fstream>
#include <string>
#include <unordered_set>
#include "token.hpp"

namespace ntt {

    class NoTokenErr : public std::runtime_error {
        public:
            NoTokenErr() : std::runtime_error("No more tokens") {}
    };

	class Tokenizer {
		public:

            explicit Tokenizer(std::ifstream& ifs);
            
            Token get();

            void put(const Token& token);

            const Token& peek();

            bool has_token();

            void to_xml(std::ofstream&);

            Token consume_identifier();

            Token consume_keyword(const std::unordered_set<std::string>&);

            Token consume_symbol(const std::string&);

            Token consume_type();
    
        private:

			static const inline std::unordered_set<char> symbols_ {
				'{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*',
				'/', '&', '|', '<', '>', '=', '_', '~'
            };

            static const inline std::unordered_set<std::string> keywords_ {
                "class", "constructor", "function", "method", "field", "static",
                "var", "int", "char", "boolean", "void", "true", "false", "null",
                "this", "let", "do", "if", "else", "while", "return"
            };

            std::deque<Token> tokens_;

            static void remove_leading_ws(std::ifstream&);

            static bool is_symbol(char);

            static bool is_keyword(const std::string&);

            static bool is_identifier(const std::string&);

            static bool is_integer(const std::string&);
	};

}

#endif
