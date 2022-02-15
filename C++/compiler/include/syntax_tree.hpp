#ifndef __SYNTAX_TREE__
#define __SYNTAX_TREE__

#include <fstream>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <variant>
#include "token.hpp"
#include "types.hpp"

namespace ntt {

    class SyntaxTree;
    using Tree = std::shared_ptr<SyntaxTree>;
    using Node = std::variant<Token, Tree>;

    class SyntaxTree {

        public:
            SyntaxTree() = default;

            std::string to_xml(int level = 0) const;

            void add_child(const Node& node);

            void set_gen_type(const CodeGeneratorType&);

            const std::vector<Node>& children() const;

        private:

            void write_line(std::ostringstream&, const std::string&, int level) const;

            void open_tag(std::ostringstream&, int) const;

            void close_tag(std::ostringstream&, int) const;

            CodeGeneratorType gen_type_;

            std::vector<Node> children_;

            static inline const std::unordered_map<CodeGeneratorType, std::string> tags_ {
                {CodeGeneratorType::INTEGER_TERM       ,"term"},
                {CodeGeneratorType::STRING_TERM        ,"term"},
                {CodeGeneratorType::KEYWORD_TERM       ,"term"},
                {CodeGeneratorType::IDENTIFIER_TERM    ,"term"},
                {CodeGeneratorType::ARRAY_TERM         ,"term"},
                {CodeGeneratorType::CALL_TERM          ,"term"},
                {CodeGeneratorType::PARENTHESIS_TERM   ,"term"},
                {CodeGeneratorType::UNARY_TERM         ,"term"},
                {CodeGeneratorType::EXPRESSION         ,"expression"},
                {CodeGeneratorType::EXPRESSION_LIST    ,"expressionList"},
                {CodeGeneratorType::IF                 ,"ifStatement"},
                {CodeGeneratorType::LET                ,"letStatement"},
                {CodeGeneratorType::DO                 ,"doStatement"},
                {CodeGeneratorType::WHILE              ,"whileStatement"},
                {CodeGeneratorType::RETURN             ,"returnStatement"},
                {CodeGeneratorType::STATEMENTS         ,"statements"},
                {CodeGeneratorType::VAR_DEC            ,"varDec"},
                {CodeGeneratorType::PARAMETER_LIST     ,"parameterList"},
                {CodeGeneratorType::SUBROUTINE_BODY    ,"subroutineBody"},
                {CodeGeneratorType::SUBROUTINE_DEC     ,"subroutineDec"},
                {CodeGeneratorType::CLASS_VAR_DEC      ,"classVarDec"},
                {CodeGeneratorType::CLASS              ,"class"}
            };
            
            friend class NodePrinter;
    };
}

#endif
