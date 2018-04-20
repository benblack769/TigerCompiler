cmake_minimum_required (VERSION 3.5)

set(CPP_SOURCE_DIR ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

SET_SOURCE_FILES_PROPERTIES( tiger.tab.c lex.yy.c PROPERTIES LANGUAGE CXX )

SET (BASE_SRC
        symbol_table.cc
        semantic_check.cc
        ast_specifics/dec_list_impl.cc
        ast_translate.cc
        buffman.cc
        mips_frame.cpp
        parse_err.cpp
        temp.cc
        tiger.tab.c
        lex.yy.c
        mips_munch.cc
        semantic_check.cc)



add_executable(parse_and_print tc.cc ${BASE_SRC})
target_include_directories(parse_and_print PRIVATE .)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set_property(TARGET parse_and_print PROPERTY CXX_STANDARD 14)
set_property(TARGET parse_and_print PROPERTY CXX_STANDARD_REQUIRED ON)
