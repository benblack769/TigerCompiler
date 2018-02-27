#include "catch.hpp"
#include "tokens.hh"
#include <algorithm>
using namespace std;

TEST_CASE("Basic case", "[basic_case]") {
    auto expected = { STRING, WHITESPACE, INTEGER, WHITESPACE, IDENTIFIER };
    auto tok_list = build_token_list("\"hi there\" 123 argvar");
    REQUIRE(expected.size() == tok_list.size());
    REQUIRE(std::equal(expected.begin(), expected.end(), tok_list.begin()));
}
