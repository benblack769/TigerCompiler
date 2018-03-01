#include "catch.hpp"
#include "tokens.hh"
#include "test_function.hh"
#include <algorithm>
using namespace std;

TEST_CASE("Basic case", "[basic_case]") {
    auto expected = { STRING, WHITESPACE, INTEGER, WHITESPACE, IDENTIFIER };
    auto tok_list = build_token_list("\"hi there\" 123 argvar");
    REQUIRE(expected.size() == tok_list.size());
    REQUIRE(std::equal(expected.begin(), expected.end(), tok_list.begin()));
}

TEST_CASE("Identifier testing", "[basic_case]") {
    SECTION("check _main accepts") {
        auto tok_list = build_token_list("_main");
        REQUIRE(tok_list[0] == IDENTIFIER);
        REQUIRE(tok_list.size() == 1);
    }
    SECTION("check starting underscores failure") {
        auto tok_list = build_token_list("_argvar");
        REQUIRE(tok_list[0] == BAD_TOKEN);
    }
    SECTION("check numbers in identifiers") {
        auto tok_list = build_token_list("arg0var1");
        REQUIRE(tok_list[0] == IDENTIFIER);
        REQUIRE(tok_list.size() == 1);
    }
    SECTION("check normal identifiers") {
        auto tok_list = build_token_list("Hi TheRe");
        REQUIRE(tok_list[0] == IDENTIFIER);
        REQUIRE(tok_list[1] == WHITESPACE);
        REQUIRE(tok_list[2] == IDENTIFIER);
        REQUIRE(tok_list.size() == 3);
    }
}

TEST_CASE("String testing", "[string testing]") {
    SECTION("check no string termination") {
        auto tok_list = build_token_list("\"hi ");
        REQUIRE(tok_list[0] == BAD_TOKEN);
    }
    SECTION("check escaped quote") {
        auto tok_list = build_token_list("\"hi \\\" \\\\\\\" \"");
        REQUIRE(tok_list[0] == STRING);
        REQUIRE(tok_list.size() == 1);
    }
    SECTION("check string not reach end") {
        auto tok_list = build_token_list(" \"hi \\\\\" 123 ");
        REQUIRE(tok_list[0] == WHITESPACE);
        REQUIRE(tok_list[1] == STRING);
        REQUIRE(tok_list[2] == WHITESPACE);
    }
    SECTION("check other escapes positive") {
        auto tok_list = build_token_list("\"hi \\x1a \\\\ \\a \\f \\t \\366 \\001 \"");
        REQUIRE(tok_list[0] == STRING);
        REQUIRE(tok_list.size() == 1);
    }
    SECTION("check other escapes negative") {
        REQUIRE(build_token_list("\" \\400 \"")[0] == BAD_TOKEN);
        REQUIRE(build_token_list("\" \\xl5 \"")[0] == BAD_TOKEN);
        REQUIRE(build_token_list("\" \\m \"")[0] == BAD_TOKEN);
        REQUIRE(build_token_list("\" \\ \"")[0] == BAD_TOKEN);
    }
    SECTION("check multiline strings") {
        auto tok_list = build_token_list("\"h \n \"");
        REQUIRE(tok_list[0] == STRING);
        REQUIRE(tok_list.size() == 1);
    }
}

// tests for: integers, operators, comments


TEST_CASE("Integer testing", "[integer testing]") {
	SECTION("")

}