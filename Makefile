
CXX=clang++   # Can switch to g++ if desired
# CXX=g++-6
CXXFLAGS=-g -O3 -std=c++14 -Wall -pedantic -Wextra #-Werror
#CXXFLAGS=-O3 -std=c++17 -Wall -pedantic -Wextra -Werror
LDFLAGS=$(CXXFLAGS)

all: test_lexer

flex: tiger.l
	flex $^

lex.o: flex
	$(CXX) $(LDFLAGS) $(LIBS) -c -o $@ lex.yy.c

catch.o: catch.cc
	$(CXX) $(LDFLAGS) $(LIBS) -c -o $@ $^

test_lexer.o: test_lexer.cc
	$(CXX) $(LDFLAGS) $(LIBS) -c -o $@ $^

test_lexer: lex.o test_lexer.o catch.o
	$(CXX) $(LDFLAGS) $(LIBS) -o $@ $^

test: all
	./test_lexer

clean:
	rm -f *.o test_lexer lex.yy.c
