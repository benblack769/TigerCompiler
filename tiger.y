%{
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "ast.hh"

int yylex();
int yyparse();

using namespace tiger;
tiger::ASTNode * rootnode;

%}

%union {
  tiger::ExprNode* expr;
  tiger::ExprListNode* expr_list;
  tiger::ExprSequenceNode* expr_seq;
  tiger::FieldListNode* field_list;
  tiger::LvalueNode* lvalue;
  tiger::DeclarationListNode* decl_list;
  tiger::DeclarationNode* decl_node;
  tiger::TypeDeclarationNode* type_decl;
  tiger::TypeFeildsNode* type_field;
  tiger::TypeIDNode* type_id;
  int64_t int_value;
  char * str;
  void * _unimplemented;
  tiger::exprs::BinaryOp bin_op;
}
/* declare types */
%type <expr> expr
%type <lvalue> lvalue
%type <field_list> fieldlist
%type <decl_list> declist
%type <decl_node> declaration vardec fundec typedec
%type <_unimplemented> type typefields typefield exprseq exprlist typeid

%type <bin_op> op


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

%left VERTICAL
%left AMPERSAND
%nonassoc LESSEQ GREATEREQ EQUAL LRCOMPARISON LESSTHAN GREATERTHAN
%left PLUS MINUS
%left ASTERISK FSLASH


%%
start_expr: expr { rootnode = $1; }
 ;

expr: IDENTIFIER LPAREN RPAREN { std::cout << "\texpr -> IDENTIFIER LPAREN RPAREN\n"; }
 | NIL_KW { $$ = new exprs::NilNode(); }
 | STRING { $$ = new exprs::StringNode($1); }
 | INTEGER { $$ = new exprs::IntNode($1); }
 | '-' expr { $$ = new exprs::NegateNode($2); }
 | expr op expr { std::cout << "\texpr -> expr op expr\n"; $$ = new exprs::BinaryNode($1,$3,$2); }
 | LPAREN exprseq RPAREN { std::cout << "\texpr -> LPAREN exprseq RPAREN\n"; }
 | IDENTIFIER LPAREN exprlist RPAREN { std::cout << "\texpr -> IDENTIFIER LPAREN exprlist RPAREN\n"; }
 | typeid LBRACE fieldlist RBRACE { std::cout << "\texpr -> typeid LBRACE fieldlist RBRACE\n"; }
 | typeid LBRACE RBRACE { /* fieldlist is optional */ std::cout << "\texpr -> typeid LBRACE RBRACE\n"; }
 | typeid LBRACK expr RBRACK OF_KW expr { std::cout << "\texpr -> typeid LBRACK expr RBRACK OF_KW expr\n"; }
 | IF_KW expr THEN_KW expr { std::cout << "\texpr -> IF_KW expr THEN_KW expr \n"; $$ = new exprs::IfThen($2, $4); }
 | IF_KW expr THEN_KW expr ELSE_KW expr { std::cout << "\texpr -> IF_KW expr THEN_KW expr ELSE_KW expr\n"; $$ = new exprs::IfThenElse($2, $4, $6); }
 | WHILE_KW expr DO_KW expr { std::cout << "\texpr -> WHILE_KW expr DO_KW expr\n"; }
 | FOR_KW IDENTIFIER COLONEQ expr TO_KW expr DO_KW expr { std::cout << "\texpr -> FOR_KW IDENTIFIER COLONEQ expr TO_KW expr DO_KW expr\n"; }
 | BREAK_KW { std::cout << "\texpr -> BREAK_KW\n";}
 | LET_KW declist IN_KW exprseq END_KW { /* with optional exprseq */ std::cout << "\texpr -> LET_KW declist IN_KW exprseq END_KW\n"; }
 | LET_KW declist IN_KW END_KW { /* withOUT optional exprseq */ std::cout << "\texpr -> LET_KW declist IN_KW END_KW\n"; }
 | lvalue { std::cout << "\texpr -> lvalue\n"; $$ = new exprs::LvalNode($1); }
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

typedec: TYPE_KW typeid EQUAL type { std::cout << "\typedec -> TYPE_KW typeid '=' type\n"; }
 ;

type: typeid { std::cout << "\ttype -> typeid\n"; }
 | LBRACE typefields RBRACE { std::cout << "\ttype -> LBRACE typefields RBRACE\n"; }
 | LBRACE RBRACE { /* typefields is optional */ std::cout << "\ttype -> LBRACE RBRACE \n"; }
 | ARRAY_KW OF_KW typeid { std::cout << "\ttype -> ARRAY_KW OF_KW typeid\n"; }
 ;

typefields: typefield { std::cout << "\ttypefields -> typefield\n"; }
 | typefields ',' typefield { std::cout << "\ttypefields -> typefields ',' typefield\n"; }
 ;
typefield: IDENTIFIER COLON typeid { std::cout << "\ttypefield -> IDENTIFIER\n"; }
 ;

exprseq: expr { std::cout << "\texprseq -> expr\n"; }
 | exprseq SEMICOLON expr { std::cout << "\texprseq ';' expr\n"; }
 ;

exprlist: expr { std::cout << "\texprlist -> expr\n"; }
 | exprlist COMMA expr { std::cout << "exprlist -> exprlist ',' expr\n"; }
 ;

typeid: IDENTIFIER { std::cout << "\ttypeid -> IDENTIFIER\n"; }

op: PLUS { std::cout << "\top -> '+'\n"; $$ = exprs::BinaryOp::ADD; }
 | MINUS { std::cout << "\top -> '-'\n"; $$ = exprs::BinaryOp::SUB; }
 | ASTERISK { std::cout << "\top -> '*'\n"; $$ = exprs::BinaryOp::MUL; }
 | FSLASH { std::cout << "\top -> '/'\n"; $$ = exprs::BinaryOp::DIV; }
 | EQUAL { std::cout << "\top -> '='\n"; $$ = exprs::BinaryOp::EQUAL; }
 | LRCOMPARISON { std::cout << "\top -> LRCOMPARISON\n"; $$ = exprs::BinaryOp::LESSGREATER; }
 | GREATERTHAN { std::cout << "\top -> '>'\n"; $$ = exprs::BinaryOp::GREATER; }
 | GREATEREQ { std::cout << "\top -> GREATEREQ\n"; $$ = exprs::BinaryOp::GREATEREQ; }
 | LESSTHAN { std::cout << "\top -> '<'\n"; $$ = exprs::BinaryOp::LESS; }
 | LESSEQ { std::cout << "\top -> LESSEQ\n"; $$ = exprs::BinaryOp::LESSEQ; }
 | AMPERSAND { std::cout << "\top -> '&'\n"; $$ = exprs::BinaryOp::AND; }
 | VERTICAL { std::cout << "\top -> '|'\n"; $$ = exprs::BinaryOp::OR; }
 ;

lvalue: IDENTIFIER { std::cout << "\tlvalue -> IDENTIFIER\n"; $$ = new lvals::IdLval($1); }
    | lvalue PERIOD IDENTIFIER { std::cout << "\tlvalue -> PERIOD IDENTIFIER\n"; }
    ;
%%
