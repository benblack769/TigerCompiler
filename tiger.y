%{
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "ast.hh"

int yylex();
int yyparse();

tiger::ASTNode * rootnode;

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
%token ARRAY_KW
    IF_KW
    THEN_KW
    ELSE_KW
    WHILE_KW
    FOR_KW
    TO_KW
    DO_KW
    LET_KW
    IN_KW
    END_KW
    OF_KW
    BREAK_KW
    NIL_KW
    FUNCTION_KW
    VAR_KW
    TYPE_KW

%token LPAREN
    RPAREN
    COMMA
    COLON
    SEMICOLON
    LBRACK
    RBRACK
    LBRACE
    RBRACE
    PERIOD
    PLUS
    MINUS
    ASTERISK
    FSLASH
    EQUAL
    LRCOMPARISON
    LESSTHAN
    GREATERTHAN
    LESSEQ
    GREATEREQ
    AMPERSAND
    VERTICAL
    COLONEQ


%type <node> expr lvalue


%%
start_expr:
    expr { rootnode = const_cast <tiger::ASTNode*>($1);std::cout << *($1); }

expr: IDENTIFIER { $$ = new tiger::IdASTNode($1); }
 | STRING { $$ = new tiger::StringASTNode($1); }
 | INTEGER { $$ = new tiger::IntASTNode($1); }
 | '-' expr { $$ = new tiger::NegateASTNode($2); }
 ;

lvalue: IDENTIFIER { $$ = new tiger::IdASTNode($1); }
    | lvalue PERIOD IDENTIFIER { $$ = new tiger::IdASTNode($1); }

%%
