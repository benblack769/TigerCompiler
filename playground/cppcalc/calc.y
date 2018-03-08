/* Companion source code for "flex & bison", published by O'Reilly
 * Media, ISBN 978-0-596-15597-1
 * Copyright (c) 2009, Taughannock Networks. All rights reserved.
 * See the README file for license conditions and contact info.
 * $Header: /home/johnl/flnb/code/RCS/fb3-1.y,v 2.1 2009/11/08 02:53:18 johnl Exp $
 */

/* calculator with AST */

%{
#  include <stdio.h>
#  include <stdlib.h>
#  include <iostream>
#  include <cmath>
#  include "ast.hh"

int yylex();
int yyparse();

using namespace tiger;

template <typename T>
struct Power {
  T operator()(T x, T y) const {
    return pow(x, y);
  }
};

%}

%union {
  tiger::ASTNode::ASTptr node;
  tiger::ASTNode::value_t val;
}

/* declare tokens */
%token <val> NUMBER
%token EOL

%type <node> exp factor efactor term

%%

calclist: /* nothing */
| calclist exp EOL {
     std::cout << "Evaluating expression: " << $2->toStr() << "\t";
     printf("= %4.4g\n", $2->eval());
     printf("> ");
     delete $2;
 }

 | calclist EOL { printf("> "); } /* blank line or a comment */
 ;

exp: factor
 | exp '+' factor { $$ = new PlusASTNode("+", $1,$3); }
 | exp '-' factor { $$ = new MinusASTNode("-", $1,$3);}
 ;

factor: efactor
 | factor '*' efactor { $$ = new MultASTNode("*", $1,$3); }
 | factor '/' efactor { $$ = new DivASTNode("/", $1,$3); }
 ;

efactor: term
 | efactor '^' term { $$ = new BinaryASTNode<Power>("**", $1,$3); }
 ;

term: NUMBER   { $$ = new NumASTNode($1); }
 | '|' term    { $$ = new AbsASTNode($2); }
 | '(' exp ')' { $$ = $2; }
 | '-' term    { $$ = new UnaryASTNode<std::negate>("-", $2); }
 ;
%%
int main()
{
    return yyparse();
}
