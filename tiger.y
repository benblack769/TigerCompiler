%{
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "bison_header_info.hh"
#include "ast_interface.hh"
#include "tiger.tab.h"
#include "ast_specifics/decs_ast.hh"
#include "ast_specifics/expr_ast.hh"
#include "ast_specifics/lval_ast.hh"
#include "ast_specifics/types_ast.hh"

int yylex();
int yyparse();

SourceLoc t_conv(YYLTYPE loc){
    return SourceLoc{ loc.first_line, loc.first_column, loc.last_line, loc.last_column };
}

using namespace tiger;
tiger::ExprNode * rootnode;

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
  IdArray arr_acc;
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
%type <arr_acc> array_acc



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

%nonassoc OF_KW
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

expr: IDENTIFIER LPAREN RPAREN { $$ = new exprs::FunctionCall($1,new ExprListNode()); free($1); $$->set_source_loc(t_conv(@$)); }
 | NIL_KW { $$ = new exprs::NilNode(); $$->set_source_loc(t_conv(@$)); }
 | STRING { $$ = new exprs::StringNode($1); free($1); $$->set_source_loc(t_conv(@$)); }
 | INTEGER { $$ = new exprs::IntNode($1); $$->set_source_loc(t_conv(@$)); }
 | MINUS expr %prec UMINUS { $$ = new exprs::NegateNode($2); $$->set_source_loc(t_conv(@$)); }
 | expr PLUS expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::ADD); $$->set_source_loc(t_conv(@$)); }
 | expr VERTICAL expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::OR); $$->set_source_loc(t_conv(@$)); }
 | expr AMPERSAND expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::AND); $$->set_source_loc(t_conv(@$)); }
 | expr LESSEQ expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::LESSEQ); $$->set_source_loc(t_conv(@$)); }
 | expr GREATEREQ expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::GREATEREQ); $$->set_source_loc(t_conv(@$)); }
 | expr EQUAL expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::EQUAL); $$->set_source_loc(t_conv(@$)); }
 | expr LRCOMPARISON expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::LESSGREATER); $$->set_source_loc(t_conv(@$)); }
 | expr LESSTHAN expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::LESS); $$->set_source_loc(t_conv(@$)); }
 | expr GREATERTHAN expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::GREATER); $$->set_source_loc(t_conv(@$)); }
 | expr MINUS expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::SUB); $$->set_source_loc(t_conv(@$)); }
 | expr ASTERISK expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::MUL); $$->set_source_loc(t_conv(@$)); }
 | expr FSLASH expr { $$ = new exprs::BinaryNode($1,$3,exprs::BinaryOp::EQUAL); $$->set_source_loc(t_conv(@$)); }
 | LPAREN exprseq RPAREN { $$ = new exprs::ExprSequenceEval($2);  $$->set_source_loc(t_conv(@$)); }
 | IDENTIFIER LPAREN exprlist RPAREN { $$ = new exprs::FunctionCall($1,$3); free($1); $$->set_source_loc(t_conv(@$)); }
 | typeid LBRACE fieldlist RBRACE { $$ = new exprs::RecCreate($1,$3); $$->set_source_loc(t_conv(@$)); }
 | typeid LBRACE RBRACE { /* fieldlist is optional */ $$ = new exprs::RecCreate($1,new FieldListNode()); $$->set_source_loc(t_conv(@$)); }
 | array_acc OF_KW expr %prec FAKE { $$ = new exprs::ArrCreate(new TypeIDNode($1.id),$1.arrexpr,$3); free($1.id); $$->set_source_loc(t_conv(@$)); }
 | IF_KW expr THEN_KW expr { $$ = new exprs::IfThen($2, $4); $$->set_source_loc(t_conv(@$)); }
 | IF_KW expr THEN_KW expr ELSE_KW expr { $$ = new exprs::IfThenElse($2, $4, $6); $$->set_source_loc(t_conv(@$)); }
 | WHILE_KW expr DO_KW expr %prec FAKE { $$ = new exprs::WhileDo($2, $4); $$->set_source_loc(t_conv(@$)); }
 | FOR_KW IDENTIFIER COLONEQ expr TO_KW expr DO_KW expr %prec FAKE{ $$ = new exprs::ForToDo($2, $4, $6, $8); free($2); $$->set_source_loc(t_conv(@$)); }
 | BREAK_KW { $$ = new exprs::Break(); $$->set_source_loc(t_conv(@$)); }
 | LET_KW declist IN_KW exprseq END_KW { /* with optional exprseq */  $$ = new exprs::LetIn($2, $4); $$->set_source_loc(t_conv(@$)); }
 | LET_KW declist IN_KW END_KW { /* withOUT optional exprseq */ $$ = new exprs::LetIn($2, new ExprSequenceNode()); $$->set_source_loc(t_conv(@$)); }
 | lvalue { $$ = new exprs::LvalNode($1); $$->set_source_loc(t_conv(@$)); }
 | lvalue COLONEQ expr %prec FAKE { $$ = new exprs::AssignNode($1, $3); $$->set_source_loc(t_conv(@$)); }
 ;

array_acc: IDENTIFIER LBRACK expr RBRACK { $$ = IdArray {$1,$3}; }
 ;

fieldlist: IDENTIFIER EQUAL expr {  $$ = new FieldListNode(); $$->append_to(new FieldNode($1,$3));  free($1); $$->set_source_loc(t_conv(@$)); }
 | fieldlist COMMA IDENTIFIER EQUAL expr { $$ = $1; $$->append_to(new FieldNode($3,$5));  free($3); $$->set_source_loc(t_conv(@$)); }
 ;

declist: declaration {  $$ = new DeclarationListNode(); $$->append_to($1); $$->set_source_loc(t_conv(@$)); }
 | declist declaration {   $$ = $1; $$->append_to($2); $$->set_source_loc(t_conv(@$)); }
 ;

declaration: typedec {  $$ = $1; }
 | vardec { $$ = $1; }
 | fundec { $$ = $1; }
 ;

vardec: VAR_KW IDENTIFIER COLONEQ expr { $$ = new decls::VarDecl($2,nullptr,$4, false);  free($2); $$->set_source_loc(t_conv(@$)); }
 | VAR_KW IDENTIFIER COLON typeid COLONEQ expr { $$ = new decls::VarDecl($2, $4, $6, true); free($2); $$->set_source_loc(t_conv(@$)); }
 ;

fundec: FUNCTION_KW IDENTIFIER LPAREN typefields RPAREN EQUAL expr { $$ = new decls::FuncDecl($2,$4,nullptr,$7,false); free($2); $$->set_source_loc(t_conv(@$)); }
 | FUNCTION_KW IDENTIFIER LPAREN RPAREN EQUAL expr {  $$ = new decls::FuncDecl($2,new TypeFeildsNode(),nullptr,$6,false); free($2); $$->set_source_loc(t_conv(@$)); }
 | FUNCTION_KW IDENTIFIER LPAREN typefields RPAREN COLON typeid EQUAL expr { $$ = new decls::FuncDecl($2,$4,$7,$9,true); free($2); $$->set_source_loc(t_conv(@$)); }
 | FUNCTION_KW IDENTIFIER LPAREN RPAREN COLON typeid EQUAL expr { $$ = new decls::FuncDecl($2,new TypeFeildsNode(),$6,$8,true); free($2); $$->set_source_loc(t_conv(@$)); }
 ;

typedec: TYPE_KW typeid EQUAL type { $$ = new decls::TypeDecl($2,$4); $$->set_source_loc(t_conv(@$)); }
 ;

type: typeid { $$ = new types::BasicType($1); $$->set_source_loc(t_conv(@$)); }
 | LBRACE typefields RBRACE { $$ = new types::TypeFeildType($2); $$->set_source_loc(t_conv(@$)); }
 | LBRACE RBRACE { /* typefields is optional */ $$ = new types::TypeFeildType(new TypeFeildsNode()); $$->set_source_loc(t_conv(@$)); }
 | ARRAY_KW OF_KW typeid { $$ = new types::ArrayType($3); $$->set_source_loc(t_conv(@$)); }
 ;

typefields: typefield { $$ = new TypeFeildsNode(); $$->append_to($1); $$->set_source_loc(t_conv(@$)); }
 | typefields COMMA typefield { $$ = $1; $$->append_to($3); $$->set_source_loc(t_conv(@$)); }
 ;
typefield: IDENTIFIER COLON typeid { $$ = new TypeFeildNode($1,$3); free($1); $$->set_source_loc(t_conv(@$)); }
 ;

exprseq: expr {  $$ = new ExprSequenceNode(); $$->append_to($1); $$->set_source_loc(t_conv(@$)); }
 | exprseq SEMICOLON expr { $$ = $1; $$->append_to($3); $$->set_source_loc(t_conv(@$)); }
 ;

exprlist: expr {  $$ = new ExprListNode(); $$->append_to($1); $$->set_source_loc(t_conv(@$)); }
 | exprlist COMMA expr { $$ = $1; $$->append_to($3); $$->set_source_loc(t_conv(@$)); }
 ;

typeid: IDENTIFIER { $$ = new TypeIDNode($1); free($1); $$->set_source_loc(t_conv(@$)); }
 ;

lvalue: IDENTIFIER { $$ = new lvals::IdLval($1); free($1); $$->set_source_loc(t_conv(@$)); }
 | array_acc { $$ = new lvals::BracketAccess(new lvals::IdLval($1.id),$1.arrexpr); free($1.id); $$->set_source_loc(t_conv(@$)); }
 | lvalue LBRACK expr RBRACK { $$ = new lvals::BracketAccess($1,$3); $$->set_source_loc(t_conv(@$)); }
 | lvalue PERIOD IDENTIFIER { $$ = new lvals::AttrAccess($1,$3); free($3); $$->set_source_loc(t_conv(@$)); }
 ;
%%
