CXX=g++   # Can switch to g++ if desired
#CC=gcc   # Can switch to g++ if desired

# CXX=g++-6
#CFLAGS=-g -O0 -Wall -pedantic
CXXFLAGS=-g -O0 -std=c++14  -I . #-Werror
WARNINGS=-Wall -Wextra
#CXXFLAGS=-O3 -std=c++17 -Wall -pedantic -Wextra -Werror
LDFLAGS=$(CXXFLAGS)

all: test_parser test_lexer parse_and_print test_semantics test_translate tc

lex.yy.c: tiger.l
	flex $^

tiger.tab.c tiger.tab.h: tiger.y
	bison -d -v $^

tiger.tab.o: tiger.tab.c
	$(CXX) $(CXXFLAGS)  -Wno-write-strings -Wno-deprecated -c -o $@  $^

lex.yy.o: lex.yy.c
	$(CXX) $(CXXFLAGS) -c -o $@ $^

parse_err.o:  parse_err.cpp
	$(CXX) $(CXXFLAGS) $(WARNINGS) -c -o $@  $^

mips_frame.o: mips_frame.cpp
	$(CXX) $(LDFLAGS) $(WARNINGS) $(LIBS) -c -o $@ $^

catch.o: catch.cc
	$(CXX) $(CXXFLAGS) $(LIBS) -c -o $@ $^

test_parser.o: test_parser.cc
	$(CXX) $(CXXFLAGS) $(WARNINGS) $(LIBS) -c -o $@ $^

test_parser: tiger.tab.o lex.yy.o parse_err.o catch.o test_parser.o temp.o mips_frame.o dec_list_impl.o  symbol_table.o ast_translate.o
	$(CXX) $(LDFLAGS) $(LIBS) -o $@ $^

test_lexer.o: test_lexer.cc
	$(CXX) $(LDFLAGS) $(LIBS) -c -o $@ $^

test_semantics.o: test_semantics.cc
	$(CXX) $(LDFLAGS) $(WARNINGS) $(LIBS) -c -o $@ $^

semantic_check.o: semantic_check.cc
	$(CXX) $(LDFLAGS) $(WARNINGS) $(LIBS) -c -o $@ $^

dec_list_impl.o: ast_specifics/dec_list_impl.cc
	$(CXX) $(LDFLAGS) $(WARNINGS) $(LIBS) -c -o $@ $^

symbol_table.o: symbol_table.cc
	$(CXX) $(LDFLAGS) $(WARNINGS) $(LIBS) -c -o $@ $^

temp.o: temp.cc
	$(CXX) $(LDFLAGS) $(WARNINGS) $(LIBS) -c -o $@ $^

test_semantics: tiger.tab.o lex.yy.o parse_err.o catch.o temp.o mips_frame.o dec_list_impl.o semantic_check.o symbol_table.o test_semantics.o ast_translate.o
	$(CXX) $(LDFLAGS) $(LIBS) -o $@ $^

temp.o: temp.cc
	$(CXX) $(LDFLAGS) $(WARNINGS) $(LIBS) -c -o $@ $^

test_lexer: lex.yy.o test_lexer.o catch.o
	$(CXX) $(LDFLAGS) $(LIBS) -o $@ $^

parse_and_print.o: parse_and_print.cc
	$(CXX) $(LDFLAGS) $(WARNINGS) $(LIBS) -c -o $@ $^

parse_and_print: parse_and_print.o tiger.tab.o lex.yy.o parse_err.o temp.o mips_frame.o dec_list_impl.o semantic_check.o symbol_table.o ast_translate.o
	$(CXX) $(LDFLAGS) $(LIBS) -o $@ $^

ast_translate.o: ast_translate.cc
	$(CXX) $(LDFLAGS) $(WARNINGS) $(LIBS) -c -o $@ $^

test_translate.o: test_translate.cc
	$(CXX) $(LDFLAGS) $(WARNINGS) $(LIBS) -c -o $@ $^

test_translate: tiger.tab.o lex.yy.o parse_err.o catch.o temp.o mips_frame.o dec_list_impl.o ast_translate.o test_translate.o buffman.o symbol_table.o
	$(CXX) $(LDFLAGS) $(LIBS) -o $@ $^

buffman.o: buffman.cc
	$(CXX) $(LDFLAGS) $(WARNINGS) $(LIBS) -c -o $@ $^

tc.o: tc.cc
	$(CXX) $(LDFLAGS) $(WARNINGS) $(LIBS) -c -o $@ $^

tc: tiger.tab.o lex.yy.o parse_err.o tc.o temp.o mips_frame.o dec_list_impl.o ast_translate.o buffman.o symbol_table.o
	$(CXX) $(LDFLAGS) $(LIBS) -o $@ $^

test: all
	./test_lexer
	./test_parser

clean:
	rm -f *.o test_parser tiger.tab.* lex.yy.c test_lexer parse_and_print test_semantics test_translate

clean_c:
	rm -f tiger.tab.c parse_and_print.o
