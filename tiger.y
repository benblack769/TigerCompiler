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
  tiger::TypeNode* type_node;
  tiger::FieldNode* field_node;
  tiger::DeclarationListNode* decl_list;
  tiger::DeclarationNode* decl_node;
  tiger::TypeFeildsNode* type_fields;
  tiger::TypeFeildNode* type_field;
  tiger::TypeIDNode* type_id;
  int64_t int_value;
  char * str;
  tiger::exprs::BinaryOp bin_op;
}
/* declare types */
%type <expr> expr
%type <lvalue> lvalue
%type <field_list> fieldlist
%type <decl_list> declist
%type <decl_node> declaration vardec fundec typedec
%type <expr_seq> exprseq
%type <expr_list> exprlist
%type <type_id> typeid
%type <type_node> type
%type <type_field> typefield
%type <type_fields> typefields



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

%nonassoc THEN_KW
%nonassoc ELSE_KW
%nonassoc FAKE
%left VERTICAL
%left AMPERSAND
%nonassoc LESSEQ GREATEREQ EQUAL LRCOMPARISON LESSTHAN GREATERTHAN
%left PLUS MINUS
%left ASTERISK FSLASH
%nonassoc UMINUS


%%
start_expr: expr { rootnode = $1; }
 ;

expr: IDENTIFIER LPAREN RPAREN { $$ = new exprs::FunctionCall($1,new ExprListNode()); free($1); }
 | NIL_KW { $$ = new exprs::NilNode(); }
 | STRING { $$ = new exprs::StringNode($1); free($1); }
 | INTEGER { $$ = new exprs::IntNode($1); }
 | MINUS expr %prec UMINUS { $$ = new exprs::NegateNode($2); }
 | expr PLUS expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::ADD); }
 | expr VERTICAL expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::OR); }
 | expr AMPERSAND expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::AND); }
 | expr LESSEQ expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::LESSEQ); }
 | expr GREATEREQ expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::GREATEREQ); }
 | expr EQUAL expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::EQUAL); }
 | expr LRCOMPARISON expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::LESSGREATER); }
 | expr LESSTHAN expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::LESS); }
 | expr GREATERTHAN expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::GREATER); }
 | expr MINUS expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::SUB); }
 | expr ASTERISK expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::MUL); }
 | expr FSLASH expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::EQUAL); }
 | LPAREN exprseq RPAREN { $$ = new exprs::ExprSequenceEval($2);  }
 | IDENTIFIER LPAREN exprlist RPAREN { $$ = new exprs::FunctionCall($1,$3); free($1); }
 | typeid LBRACE fieldlist RBRACE { $$ = new exprs::RecCreate($1,$3); }
 | typeid LBRACE RBRACE { /* fieldlist is optional */ $$ = new exprs::RecCreate($1,new FieldListNode()); }
 | typeid LBRACK expr RBRACK OF_KW expr %prec FAKE { $$ = new exprs::ArrCreate($1,$3,$6); }
 | IF_KW expr THEN_KW expr { $$ = new exprs::IfThen($2, $4); }
 | IF_KW expr THEN_KW expr ELSE_KW expr { $$ = new exprs::IfThenElse($2, $4, $6); }
 | WHILE_KW expr DO_KW expr %prec FAKE { $$ = new exprs::WhileDo($2, $4); }
 | FOR_KW IDENTIFIER COLONEQ expr TO_KW expr DO_KW expr %prec FAKE{ $$ = new exprs::ForToDo($2, $4, $6, $8); free($2); }
 | BREAK_KW { $$ = new exprs::Break(); }
 | LET_KW declist IN_KW exprseq END_KW { /* with optional exprseq */  $$ = new exprs::LetIn($2, $4); }
 | LET_KW declist IN_KW END_KW { /* withOUT optional exprseq */ $$ = new exprs::LetIn($2, new ExprSequenceNode()); }
 | lvalue { $$ = new exprs::LvalNode($1); }
 | lvalue COLONEQ expr %prec FAKE { $$ = new exprs::AssignNode($1, $3); }
 ;

fieldlist: IDENTIFIER EQUAL expr {  $$ = new FieldListNode(); $$->append_to(new FieldNode($1,$3));  free($1); }
 | fieldlist COMMA IDENTIFIER EQUAL expr { $$ = $1; $$->append_to(new FieldNode($3,$5));  free($3); }
 ;

declist: declaration {  $$ = new DeclarationListNode(); $$->append_to($1); }
 | declist declaration {   $$ = $1; $$->append_to($2); }
 ;

declaration: typedec {  $$ = $1; }
 | vardec { $$ = $1; }
 | fundec { $$ = $1; }
 ;

vardec: VAR_KW IDENTIFIER COLONEQ expr { $$ = new decls::VarDecl($2,nullptr,$4, false);  free($2); }
 | VAR_KW IDENTIFIER COLON typeid COLONEQ expr { $$ = new decls::VarDecl($2, $4, $6, true); free($2); }
 ;

fundec: FUNCTION_KW IDENTIFIER LPAREN typefields RPAREN EQUAL expr { $$ = new decls::FuncDecl($2,$4,nullptr,$7,false); free($2); }
 | FUNCTION_KW IDENTIFIER LPAREN RPAREN EQUAL expr {  $$ = new decls::FuncDecl($2,new TypeFeildsNode(),nullptr,$6,false); free($2); }
 | FUNCTION_KW IDENTIFIER LPAREN typefields RPAREN COLON typeid EQUAL expr { $$ = new decls::FuncDecl($2,$4,$7,$9,true); free($2); }
 | FUNCTION_KW IDENTIFIER LPAREN RPAREN COLON typeid EQUAL expr { $$ = new decls::FuncDecl($2,new TypeFeildsNode(),$6,$8,true); free($2); }
 ;

typedec: TYPE_KW typeid EQUAL type { $$ = new decls::TypeDecl($2,$4); }
 ;

type: typeid { $$ = new types::BasicType($1); }
 | LBRACE typefields RBRACE { $$ = new types::TypeFeildType($2); }
 | LBRACE RBRACE { /* typefields is optional */ $$ = new types::TypeFeildType(new TypeFeildsNode()); }
 | ARRAY_KW OF_KW typeid { $$ = new types::ArrayType($3); }
 ;

typefields: typefield { $$ = new TypeFeildsNode(); $$->append_to($1); }
 | typefields COMMA typefield { $$ = $1; $$->append_to($3); }
 ;
typefield: IDENTIFIER COLON typeid { $$ = new TypeFeildNode($1,$3); free($1); }
 ;

exprseq: expr {  $$ = new ExprSequenceNode(); $$->append_to($1); }
 | exprseq SEMICOLON expr { $$ = $1; $$->append_to($3); }
 ;

exprlist: expr {  $$ = new ExprListNode(); $$->append_to($1); }
 | exprlist COMMA expr { $$ = $1; $$->append_to($3); }
 ;

typeid: IDENTIFIER { $$ = new TypeIDNode($1); free($1); }
 ;

lvalue: IDENTIFIER { $$ = new lvals::IdLval($1); free($1); }
  | lvalue LBRACK expr RBRACK { $$ = new lvals::BracketAccess($1,$3); }
  | lvalue PERIOD IDENTIFIER { $$ = new lvals::AttrAccess($1,$3); free($3); }
 ;
%%
