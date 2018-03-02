#pragma once

// keywords [not able to be used as identifiers]:
/*
‘array’, ‘if’, ‘then’, ‘else’, ‘while’, ‘for’,
‘to’, ‘do’,
‘let’, ‘in’, ‘end’,
‘of’, ‘break’,
‘nil’, ‘function’, ‘var’,
‘type’, ‘import’ and ‘primitive’
*/
enum Tokens {
    INTEGER = 257,
    STRING,
    IDENTIFIER,
    KEYWORD, // see comment above
    //EOL, // \n\r , \r\n , \r , \n
    WHITESPACE, // space or tab
    BAD_TOKEN, // catch all
    LPAREN, // (
    RPAREN, // )
    COMMA, // ,
    COLON, // :
    SEMICOLON, // :
    LBRACK, // [
    RBRACK, // ]
    LBRACE, // {
    RBRACE, // }
    PERIOD, // .
    PLUS, // +
    MINUS, // -
    ASTERISK, // *
    FSLASH, // /
    EQUAL, // =
    LRCOMPARISON, // <>
    LESSTHAN, // <
    GREATERTHAN, // >
    LESSEQ, // <=
    GREATEREQ, // >=
    AMPERSAND, // &
    VERTICAL, // |
//<<<<<<< HEAD
  //  COLONEQ // :=
  //  COMMENT // /* */

//=======
    COLONEQ, // :=
    ARRAY_KW
    ,IF_KW
    ,THEN_KW
    ,ELSE_KW
    ,WHILE_KW
    ,FOR_KW
    ,TO_KW
    ,DO_KW
    ,LET_KW
    ,IN_KW
    ,END_KW
    ,OF_KW
    ,BREAK_KW
    ,NIL_KW
    ,FUNCTION_KW
    ,VAR_KW
    ,TYPE_KW
//>>>>>>> fc698916eb933db8b973d4edc03567856c500676
};
