# $Header: /home/johnl/flnb/code/RCS/Makefile.ch3,v 2.1 2009/11/08 02:53:18 johnl Exp $
# Companion source code for "flex & bison", published by O'Reilly
# Media, ISBN 978-0-596-15597-1
# Copyright (c) 2009, Taughannock Networks. All rights reserved.
# See the README file for license conditions and contact info.

# programs in chapter 3

all:	  calc

calc:	calc.l calc.y ast.hh calc.cc
	bison -d calc.y
	flex -ocalc.lex.c calc.l
	clang++-5.0 -Wno-write-strings -Wno-deprecated -std=c++17 -o $@ calc.tab.c calc.lex.c calc.cc

clean:
	rm -f calc calc.lex.c calc.tab.h calc.tab.c
