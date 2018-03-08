/* Companion source code for "flex & bison", published by O'Reilly
 * Media, ISBN 978-0-596-15597-1
 * Copyright (c) 2009, Taughannock Networks. All rights reserved.
 * See the README file for license conditions and contact info.
 * $Header: /home/johnl/flnb/code/RCS/fb3-1.y,v 2.1 2009/11/08 02:53:18 johnl Exp $
 */

/* calculator with AST */

%{
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "ast.hh"

int yylex();
int yyparse();

%}

%union {
  tiger::ASTNode::ASTptr node;
  int64_t int_value;
  char * str;
}

/* declare tokens */
%token BAD_TOKEN
%token <int_value> INTEGER
%token <str> IDENTIFIER
%token <str> STRING

%type <node> expr

%%
start_expr:
    | expr { std::cout << *($1); }

expr:
 | IDENTIFIER { $$ = new tiger::IdASTNode($1); }
 | STRING { $$ = new tiger::StringASTNode($1); }
 | INTEGER { $$ = new tiger::IntASTNode($1); }
 | '-' expr { $$ = new tiger::NegateASTNode($2); }
 ;

%%


int
main()
{
  return yyparse();
}
