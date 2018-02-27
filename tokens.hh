#pragma once
#include <vector>
#include <string>
enum Tokens {
    INTEGER = 257,
    STRING,
    ARRAY_KW,
    SEMICOLON,
    IDENTIFIER,
    EOL,
    WHITESPACE,
    BAD_TOKEN
};

std::vector<Tokens> build_token_list(std::string input);
