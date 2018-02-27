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
    SEMICOLON,
    IDENTIFIER,
    KEYWORD, // see comment above
    EOL, // \n\r , \r\n , \r , \n
    WHITESPACE, // space or tab
    BAD_TOKEN, 
    LPAREN, // (
    RPAREN, // )
    COMMA, // ,
    COLON, // :
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
    COLONEQ // :=

};
