CXX=g++   # Can switch to g++ if desired
CC=gcc   # Can switch to g++ if desired

# CXX=g++-6
CFLAGS=-g -O2 -Wall -pedantic
CXXFLAGS=-g -O2 -std=c++14   #-Werror
#CXXFLAGS=-O3 -std=c++17 -Wall -pedantic -Wextra -Werror
LDFLAGS=$(CXXFLAGS)

all: test_parser test_lexer

lex.yy.c: tiger.l
	flex $^

tiger.tab.c tiger.tab.h: tiger.y
	bison -d $^

tiger.tab.o: tiger.tab.c
	$(CXX) $(CXXFLAGS)  -Wno-write-strings -Wno-deprecated -c -o $@ $^

lex.yy.o: lex.yy.c
	$(CXX) $(CXXFLAGS) -c -o $@ $^

ast.o: ast.cpp ast.hh
	$(CXX) $(CXXFLAGS) -c -o $@  ast.cpp

catch.o: catch.cc
	$(CXX) $(CXXFLAGS) $(LIBS) -c -o $@ $^

test_parser.o: test_parser.cc
	$(CXX) $(CXXFLAGS) $(LIBS) -c -o $@ $^

test_parser: tiger.tab.o lex.yy.o ast.o catch.o test_parser.o
	$(CXX) $(LDFLAGS) $(LIBS) -o $@ $^

test_lexer.o: test_lexer.cc
	$(CXX) $(LDFLAGS) $(LIBS) -c -o $@ $^

test_lexer: lex.yy.o test_lexer.o catch.o
	$(CXX) $(LDFLAGS) $(LIBS) -o $@ $^


test: all
	./test_lexer
	./test_parser

clean:
	rm -f *.o test_parser tiger.tab.* lex.yy.c test_lexer
