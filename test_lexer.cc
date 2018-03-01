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

TEST_CASE("Whitespace testing", "[basic_case]") {
    SECTION("check different whitespace accepts") {
        auto tok_list = build_token_list(" \t\n\r \r\t ");
        REQUIRE(tok_list[0] == WHITESPACE);
        REQUIRE(tok_list.size() == 1);
    }
}

TEST_CASE("Keyword testing", "[basic_case]") {
    SECTION("array checked") {
        auto tok_list = build_token_list("array");
        REQUIRE(tok_list[0] == ARRAY_KW);
        REQUIRE(tok_list.size() == 1);
    }
}

TEST_CASE("Operator testing", "[basic_case]") {
    SECTION("some operators checked") {
        auto tok_list = build_token_list("<><|><==>");
        auto expected = { LRCOMPARISON, LESSTHAN, VERTICAL, GREATERTHAN, LESSEQ, EQUAL, GREATERTHAN };
        REQUIRE(expected.size() == tok_list.size());
        REQUIRE(std::equal(expected.begin(), expected.end(), tok_list.begin()));
    }
}

TEST_CASE("Number testing", "[basic_case]") {
    SECTION("basic numbers checked") {
        auto tok_list = build_token_list("012331912 123212");
        auto expected = { INTEGER, WHITESPACE, INTEGER };
        REQUIRE(expected.size() == tok_list.size());
        REQUIRE(std::equal(expected.begin(), expected.end(), tok_list.begin()));
    }
    SECTION("letters not in numbers") {
        auto tok_list = build_token_list("01233absda");
        REQUIRE(tok_list[0] == INTEGER);
        REQUIRE(tok_list.size() > 1);
    }
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
    SECTION("check keywork extensions") {
        auto tok_list = build_token_list("ifelse");
        REQUIRE(tok_list[0] == IDENTIFIER);
        REQUIRE(tok_list.size() == 1);
    }
    SECTION("check normal identifier splitting") {
        auto tok_list = build_token_list("Hi TheRe");
        REQUIRE(tok_list[0] == IDENTIFIER);
        REQUIRE(tok_list[2] == IDENTIFIER);
        REQUIRE(tok_list.size() == 3);
    }
    SECTION("check operator identifier splitting") {
        auto tok_list = build_token_list("Asl;{kjals!djkA+Sdk");
        REQUIRE(tok_list[0] == IDENTIFIER);
        REQUIRE(tok_list[3] == IDENTIFIER);
        REQUIRE(tok_list[5] == IDENTIFIER);
        REQUIRE(tok_list[7] == IDENTIFIER);
        REQUIRE(tok_list.size() == 8);
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
    //not checking other escapes in this homework
    //full escape regex: \"([^\\]|(\\(a|b|f|n|r|t|v|\"|\\|([0-3][0-7][0-7])|(x[0-9a-fA-F][0-9a-fA-F]))))*\"
    /*SECTION("check other escapes positive") {
        auto tok_list = build_token_list("\"hi \\x1a \\\\ \\a \\f \\t \\366 \\001 \"");
        REQUIRE(tok_list[0] == STRING);
        REQUIRE(tok_list.size() == 1);
    }
    SECTION("check other escapes negative") {
        REQUIRE(build_token_list("\" \\400 \"")[0] == BAD_TOKEN);
        REQUIRE(build_token_list("\" \\xl5 \"")[0] == BAD_TOKEN);
        REQUIRE(build_token_list("\" \\m \"")[0] == BAD_TOKEN);
        REQUIRE(build_token_list("\" \\ \"")[0] == BAD_TOKEN);
    }*/
    SECTION("check multiline strings") {
        auto tok_list = build_token_list("\"h \n \"");
        REQUIRE(tok_list[0] == STRING);
        REQUIRE(tok_list.size() == 1);
    }
}

// tests for: integers, operators, comments


TEST_CASE("Test comments") {
	auto tok_list = build_token_list("/* hello */");
	REQUIRE(tok_list.size() == 0);

	SECTION("nested comments") {
		auto tok_list = build_token_list("/* hello /* hello2 */ */");
		REQUIRE(tok_list.size() == 0);
	}
}