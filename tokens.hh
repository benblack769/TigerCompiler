#pragma once
enum Tokens {
    INTEGER = 257,
    STRING,
    SEMICOLON,
    IDENTIFIER,
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
