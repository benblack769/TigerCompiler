cmake_minimum_required (VERSION 3.5)

set(CPP_SOURCE_DIR ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

SET (BASE_SRC
        symbol_table.cc
        semantic_check.cc
        catch.cc
        ast_specifics/dec_list_impl.cc
        semantic_check.cc)



#add_executable(test_lexer test_lexer.cc ${BASE_SRC})
#add_executable(test_parser test_parser.cc ${BASE_SRC})
#add_executable(test_semantics test_semantics.cc ${BASE_SRC})
add_executable(parse_and_print parse_and_print.cc ${BASE_SRC})
target_include_directories(parse_and_print PRIVATE .)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")

#set_property(TARGET test_lexer PROPERTY CXX_STANDARD 14)
#set_property(TARGET test_lexer PROPERTY CXX_STANDARD_REQUIRED ON)

#set_property(TARGET test_parser PROPERTY CXX_STANDARD 14)
#set_property(TARGET test_parser PROPERTY CXX_STANDARD_REQUIRED ON)

#set_property(TARGET test_semantics PROPERTY CXX_STANDARD 14)
#set_property(TARGET test_semantics PROPERTY CXX_STANDARD_REQUIRED ON)

set_property(TARGET parse_and_print PROPERTY CXX_STANDARD 14)
set_property(TARGET parse_and_print PROPERTY CXX_STANDARD_REQUIRED ON)
