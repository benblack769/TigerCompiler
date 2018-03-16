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
start_expr: expr {  }
 ;

expr: IDENTIFIER LPAREN RPAREN { std::cout << "\texpr -> IDENTIFIER LPAREN RPAREN\n"; }
 | NIL_KW { }
 | STRING { }
 | INTEGER {  }
 | '-' expr {  }
 | expr op expr { std::cout << "\texpr -> expr op expr\n"; }
 | LPAREN exprseq RPAREN { std::cout << "\texpr -> LPAREN exprseq RPAREN\n"; }
 | IDENTIFIER LPAREN exprlist RPAREN { std::cout << "\texpr -> IDENTIFIER LPAREN exprlist RPAREN\n"; }
 | typeid LBRACE fieldlist RBRACE { std::cout << "\texpr -> typeid LBRACE fieldlist RBRACE\n"; }
 | typeid LBRACE RBRACE { /* fieldlist is optional */ std::cout << "\texpr -> typeid LBRACE RBRACE\n"; }
 | typeid LBRACK expr RBRACK OF_KW expr { std::cout << "\texpr -> typeid LBRACK expr RBRACK OF_KW expr\n"; }
 | IF_KW expr THEN_KW expr { std::cout << "\texpr -> IF_KW expr THEN_KW expr \n"; }
 | IF_KW expr THEN_KW expr ELSE_KW expr { std::cout << "\texpr -> IF_KW expr THEN_KW expr ELSE_KW expr\n"; }
 | WHILE_KW expr DO_KW expr { std::cout << "\texpr -> WHILE_KW expr DO_KW expr\n"; }
 | FOR_KW IDENTIFIER COLONEQ expr TO_KW expr DO_KW expr { std::cout << "\texpr -> FOR_KW IDENTIFIER COLONEQ expr TO_KW expr DO_KW expr\n"; }
 | BREAK_KW { std::cout << "\texpr -> BREAK_KW\n";}
 | LET_KW declist IN_KW exprseq END_KW { /* with optional exprseq */ std::cout << "\texpr -> LET_KW declist IN_KW exprseq END_KW\n"; } 
 | LET_KW declist IN_KW END_KW { /* withOUT optional exprseq */ std::cout << "\texpr -> LET_KW declist IN_KW END_KW\n"; }
 | lvalue { std::cout << "\texpr -> lvalue\n"; }
 | lvalue COLONEQ expr { std::cout << "\texpr -> lvalue COLONEQ expr\n"; }
 ;

fieldlist: IDENTIFIER '=' expr { std::cout << "\tfieldlist -> IDENTIFIER '=' expr\n"; }
 | fieldlist ',' IDENTIFIER '=' expr { std::cout << "\tfieldlist -> fieldlist ',' IDENTIFIER\n"; }
 ;

declist: declaration { std::cout << "\tdeclist -> declaration\n"; }
 | declist declaration { std::cout << "\tdeclist -> declist declaration\n"; }
 ;

declaration: typedec { std::cout << "\tdeclaration -> typedec\n"; }
 | vardec { std::cout << "\tdeclaration -> vardec\n"; }
 | fundec { std::cout << "\tdeclaration -> fundec\n"; }
 ;

vardec: VAR_KW IDENTIFIER COLONEQ expr { std::cout << "\tvardec -> VAR_KW IDENTIFIER COLONEQ expr\n"; }
 | VAR_KW IDENTIFIER COLON typeid COLONEQ expr { std::cout << "\tvardec -> VAR_KW IDENTIFIER COLON typeid COLONEQ expr\n"; }
 ;

fundec: FUNCTION_KW IDENTIFIER LPAREN typefields RPAREN EQUAL expr { std::cout << "\tfundec -> FUNCTION_KW IDENTIFIER LPAREN typefields RPAREN EQUAL expr\n"; }
 | FUNCTION_KW IDENTIFIER LPAREN RPAREN EQUAL expr { std::cout << "\tfindec -> FUNCTION_KW IDENTIFIER LPAREN RPAREN EQUAL expr\n"; } 
 | FUNCTION_KW IDENTIFIER LPAREN typefields RPAREN COLON typeid EQUAL expr { std::cout << "\tfundec -> FUNCTION_KW IDENTIFIER LPAREN typefields RPAREN COLON typeid EQUAL expr\n"; }
 | FUNCTION_KW IDENTIFIER LPAREN RPAREN COLON typeid EQUAL expr { std::cout << "\tfundec -> FUNCTION_KW IDENTIFIER LPAREN RPAREN COLON typeid EQUAL expr\n"; }
 ;

typedec: TYPE_KW typeid '=' type { std::cout << "\typedec -> TYPE_KW typeid '=' type\n"; }
 ;

type: typeid { std::cout << "\ttype -> typeid\n"; }
 | LBRACE typefields RBRACE { std::cout << "\ttype -> LBRACE typefields RBRACE\n"; }
 | LBRACE RBRACE { /* typefields is optional */ std::cout << "\ttype -> LBRACE RBRACE \n"; }
 | ARRAY_KW OF_KW typeid { std::cout << "\ttype -> ARRAY_KW OF_KW typeid\n"; }
 ;

typefields: typefield { std::cout << "\ttypefields -> typefield\n"; }
 | typefields ',' typefield { std::cout << "\ttypefields -> typefields ',' typefield\n"; }
 ;
typefield: IDENTIFIER ':' typeid { std::cout << "\ttypefield -> IDENTIFIER\n"; }
 ;

exprseq: expr { std::cout << "\texprseq -> expr\n"; }
 | exprseq ';' expr { std::cout << "\texprseq ';' expr\n"; }
 ;
    
exprlist: expr { std::cout << "\texprlist -> expr\n"; }
 | exprlist ',' expr { std::cout << "exprlist -> exprlist ',' expr\n"; }
 ;

typeid: IDENTIFIER { std::cout << "\ttypeid -> IDENTIFIER\n"; }

op: '+' { std::cout << "\top -> '+'\n"; }
 | '-' { std::cout << "\top -> '-'\n"; }
 | '*' { std::cout << "\top -> '*'\n"; }
 | '/' { std::cout << "\top -> '/'\n"; }
 | '=' { std::cout << "\top -> '='\n"; }
 | LRCOMPARISON { std::cout << "\top -> LRCOMPARISON\n"; }
 | '>' { std::cout << "\top -> '>'\n";}
 | '<' { std::cout << "\top -> '<'\n"; }
 | GREATEREQ { std::cout << "\top -> GREATEREQ\n"; } 
 | LESSEQ { std::cout << "\top -> LESSEQ\n"; }
 | '&' { std::cout << "\top -> '&'\n"; }
 | '|' { std::cout << "\top -> '|'\n"; }
 ;

lvalue: IDENTIFIER { std::cout << "\tlvalue -> IDENTIFIER\n"; }
    | lvalue PERIOD IDENTIFIER { std::cout << "\tlvalue -> PERIOD IDENTIFIER\n"; }
    ;
%%
