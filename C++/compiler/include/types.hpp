#ifndef __TYPES_H__
#define __TYPES_H__

namespace ntt {

    enum class CodeGeneratorType {
        INTEGER_TERM, STRING_TERM, KEYWORD_TERM, IDENTIFIER_TERM, ARRAY_TERM,
        METHOD_CALL_TERM, FUNCTION_CALL_TERM,
        PARENTHESIS_TERM, UNARY_TERM, EXPRESSION, EXPRESSION_LIST, IF, LET, DO,
        WHILE, RETURN, STATEMENTS, VAR_DEC, PARAMETER_LIST, SUBROUTINE_BODY,
        SUBROUTINE_DEC, CLASS_VAR_DEC, CLASS
    };

    enum class Keyword { TRUE, FALSE, NULL_, THIS };

    enum class BinaryOp { ADD, SUB };
}

#endif
