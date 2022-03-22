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

            Token consume_integer();

            Token consume_string();
    
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

            void tokenize(std::ifstream&);

            static void remove_leading_ws(std::ifstream&, size_t&, size_t&);

            static bool is_symbol(char);

            static bool is_keyword(const std::string&);

            static bool is_identifier(const std::string&);

            static bool is_integer(const std::string&);

            static void consume_multiline_comment(std::ifstream&, size_t&, size_t&);
	};

}

#endif
