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

expr: IDENTIFIER {  }
 | NIL_KW { }
 | STRING { }
 | INTEGER {  }
 | '-' expr {  }
 | expr op expr { std::cout << "\texpr -> expr op expr\n"; }
 | LPAREN exprseq RPAREN { std::cout << "\texpr -> LPAREN exprseq RPAREN\n"; }
 | IDENTIFIER LPAREN exprlist RPAREN { std::cout << "\texpr -> IDENTIFIER LPAREN exprlist RPAREN\n"; }
 | typeid LBRACE fieldlist RBRACE { std::cout << "\ttypeid lbrace fieldlist rbrace found\n"; }
 | typeid LBRACE RBRACE { /* fieldlist is optional */ std::cout << "\ttypeid lbrace rbrace found\n"; }
 | typeid LBRACK expr RBRACK OF_KW expr { std::cout << "\ttypeid lbrack expr rbrack of expr found\n"; }
 | IF_KW expr THEN_KW expr { std::cout << "\tif expr then expr found\n"; }
 | IF_KW expr THEN_KW expr ELSE_KW expr { std::cout << "\tif expr then expr else expr found\n"; }
 | WHILE_KW expr DO_KW expr { std::cout << "\twhile expr do expr\n"; }
 | FOR_KW IDENTIFIER COLONEQ expr TO_KW expr DO_KW expr { std::cout << "\tfor identifier coloneq expr to expr do expr\n"; }
 | BREAK_KW { std::cout << "\tFound 'break' \n";}
 | LET_KW declist IN_KW exprseq END_KW { /* with optional exprseq */ std::cout << "\tlet declist in exprseq end found\n"; } 
 | LET_KW declist IN_KW END_KW { /* withOUT optional exprseq */ std::cout << "\tlet declist in end found\n"; }
 | lvalue { std::cout << "\tlvalue found\n"; }
 | lvalue COLONEQ expr { std::cout << "\tlvalue := expr found\n"; }
 ;

fieldlist: IDENTIFIER '=' expr { std::cout << "\tidentifier = expr found\n"; }
 | fieldlist ',' IDENTIFIER '=' expr { std::cout << "\tfieldlist , identifier = expr\n"; }
 ;

declist: declaration { std::cout << "\tdeclaration found\n"; }
 | declist declaration { std::cout << "\tdeclist declaration found\n"; }
 ;

declaration: typedec { std::cout << "\ttypedec found\n"; }
 | vardec { std::cout << "\tvardec found\n"; }
 | fundec { std::cout << "\tfundec found\n"; }
 ;

vardec: VAR_KW IDENTIFIER COLONEQ expr { std::cout << "\tvar identifier := expr found\n"; }
 | VAR_KW IDENTIFIER COLON typeid COLONEQ expr { std::cout << "\tvar identifier : typeid := expr found\n"; }
 ;

fundec: FUNCTION_KW IDENTIFIER LPAREN typefields RPAREN EQUAL expr { std::cout << "\tfunction identifier ( typefields ) = expr found\n"; }
 | FUNCTION_KW IDENTIFIER LPAREN RPAREN EQUAL expr { std::cout << "\tfunction identifier () = expr found\n"; } 
 | FUNCTION_KW IDENTIFIER LPAREN typefields RPAREN COLON typeid EQUAL expr { std::cout << "\tfunction identifier ( typefields ) : typeid = expr found\n"; }
 | FUNCTION_KW IDENTIFIER LPAREN RPAREN COLON typeid EQUAL expr { std::cout << "\tfunction identifier () : typeid = expr found\n"; }
 ;

typedec: TYPE_KW typeid '=' type { std::cout << "\ttype typeid = type found \n"; }
 ;

type: typeid { std::cout << "\ttypeid found\n"; }
 | LBRACE typefields RBRACE { std::cout << "\tlbrace typefields rbrace found\n"; }
 | LBRACE RBRACE { /* typefields is optional */ std::cout << "\tlbrace rbrace found\n"; }
 | ARRAY_KW OF_KW typeid { std::cout << "\tarray of typeid found\n"; }
 ;

typefields: typefield { std::cout << "typefield found\n"; }
 | typefields ',' typefield { std::cout << "typefields , typefield found\n"; }
 ;
typefield: IDENTIFIER ':' typeid { std::cout << "\tidentifier found\n"; }
 ;

exprseq: expr {}
 | exprseq ';' expr {}
 ;
    
exprlist: expr {}
 | exprlist ',' expr {}
 ;

typeid: IDENTIFIER { std::cout << "\ttypeid found\n"; }

op: '+' {}
 | '-' {}
 | '*' {}
 | '/' {}
 | '=' {}
 | LRCOMPARISON {}
 | '>' {}
 | '<' {}
 | GREATEREQ {}
 | LESSEQ {}
 | '&' {}
 | '|' {}
 ;

lvalue: IDENTIFIER {  }
    | lvalue PERIOD IDENTIFIER {  }
    ;
%%
