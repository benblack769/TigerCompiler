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

expr: IDENTIFIER LPAREN RPAREN { std::cout << "\texpr -> IDENTIFIER LPAREN RPAREN\n"; }
 | NIL_KW { $$ = new exprs::NilNode(); }
 | STRING { $$ = new exprs::StringNode($1); }
 | INTEGER { $$ = new exprs::IntNode($1); }
 | '-' expr %prec UMINUS { $$ = new exprs::NegateNode($2); }
 | expr PLUS expr { /*std::cout << "\texpr -> expr op expr\n"; $$ = new exprs::BinaryNode($1,$3,$2);*/ }
 | expr VERTICAL expr {}
 | expr AMPERSAND expr {}
 | expr LESSEQ expr {}
 | expr GREATEREQ expr {}
 | expr EQUAL expr {}
 | expr LRCOMPARISON expr {}
 | expr LESSTHAN expr {}
 | expr GREATERTHAN expr {}
 | expr MINUS expr {}
 | expr ASTERISK expr {}
 | expr FSLASH expr {}
 | LPAREN exprseq RPAREN { std::cout << "\texpr -> LPAREN exprseq RPAREN\n"; $$ = new exprs::ExprSequenceEval($2);  }
 | IDENTIFIER LPAREN exprlist RPAREN { std::cout << "\texpr -> IDENTIFIER LPAREN exprlist RPAREN\n"; $$ = new exprs::FunctionCall($1,$3); }
 | typeid LBRACE fieldlist RBRACE { std::cout << "\texpr -> typeid LBRACE fieldlist RBRACE\n"; }
 | typeid LBRACE RBRACE { /* fieldlist is optional */ std::cout << "\texpr -> typeid LBRACE RBRACE\n"; }
 | typeid LBRACK expr RBRACK OF_KW expr %prec FAKE { std::cout << "\texpr -> typeid LBRACK expr RBRACK OF_KW expr\n"; }
 | IF_KW expr THEN_KW expr { std::cout << "\texpr -> IF_KW expr THEN_KW expr \n"; $$ = new exprs::IfThen($2, $4); }
 | IF_KW expr THEN_KW expr ELSE_KW expr { std::cout << "\texpr -> IF_KW expr THEN_KW expr ELSE_KW expr\n"; $$ = new exprs::IfThenElse($2, $4, $6); }
 | WHILE_KW expr DO_KW expr %prec FAKE { std::cout << "\texpr -> WHILE_KW expr DO_KW expr\n"; $$ = new exprs::WhileDo($2, $4); }
 | FOR_KW IDENTIFIER COLONEQ expr TO_KW expr DO_KW expr %prec FAKE{ std::cout << "\texpr -> FOR_KW IDENTIFIER COLONEQ expr TO_KW expr DO_KW expr\n"; $$ = new exprs::ForToDo($2, $4, $6, $8);  }
 | BREAK_KW { std::cout << "\texpr -> BREAK_KW\n"; $$ = new exprs::Break(); }
 | LET_KW declist IN_KW exprseq END_KW { /* with optional exprseq */ std::cout << "\texpr -> LET_KW declist IN_KW exprseq END_KW\n"; $$ = new exprs::LetIn($2, $4); }
 | LET_KW declist IN_KW END_KW { /* withOUT optional exprseq */ std::cout << "\texpr -> LET_KW declist IN_KW END_KW\n"; $$ = new exprs::LetIn($2, new ExprSequenceNode()); }
 | lvalue { std::cout << "\texpr -> lvalue\n"; $$ = new exprs::LvalNode($1); }
 | lvalue COLONEQ expr %prec FAKE { std::cout << "\texpr -> lvalue COLONEQ expr\n"; $$ = new exprs::AssignNode($1, $3); }
 ;

fieldlist: IDENTIFIER '=' expr { std::cout << "\tfieldlist -> IDENTIFIER '=' expr\n"; $$ = new FieldListNode(); $$->append_to(new FieldNode($1,$3)); }
 | fieldlist ',' IDENTIFIER '=' expr { std::cout << "\tfieldlist -> fieldlist ',' IDENTIFIER\n"; $$ = $1; $$->append_to(new FieldNode($3,$5)); }
 ;

declist: declaration { std::cout << "\tdeclist -> declaration\n"; $$ = new DeclarationListNode(); $$->append_to($1); }
 | declist declaration { std::cout << "\tdeclist -> declist declaration\n";  $$ = $1; $$->append_to($2); }
 ;

declaration: typedec { std::cout << "\tdeclaration -> typedec\n"; $$ = $1; }
 | vardec { std::cout << "\tdeclaration -> vardec\n";  $$ = $1; }
 | fundec { std::cout << "\tdeclaration -> fundec\n";  $$ = $1; }
 ;

vardec: VAR_KW IDENTIFIER COLONEQ expr { std::cout << "\tvardec -> VAR_KW IDENTIFIER COLONEQ expr\n"; $$ = new decls::VarDecl($2,nullptr,$4, false);}
 | VAR_KW IDENTIFIER COLON typeid COLONEQ expr { std::cout << "\tvardec -> VAR_KW IDENTIFIER COLON typeid COLONEQ expr\n";  $$ = new decls::VarDecl($2, $4, $6, true); }
 ;

fundec: FUNCTION_KW IDENTIFIER LPAREN typefields RPAREN EQUAL expr { $$ = new decls::FuncDecl($2,$4,nullptr,$7,false); }
 | FUNCTION_KW IDENTIFIER LPAREN RPAREN EQUAL expr {  $$ = new decls::FuncDecl($2,new TypeFeildsNode(),nullptr,$6,false); }
 | FUNCTION_KW IDENTIFIER LPAREN typefields RPAREN COLON typeid EQUAL expr { $$ = new decls::FuncDecl($2,$4,$7,$9,true); }
 | FUNCTION_KW IDENTIFIER LPAREN RPAREN COLON typeid EQUAL expr { $$ = new decls::FuncDecl($2,new TypeFeildsNode(),$6,$8,true); }
 ;

typedec: TYPE_KW typeid EQUAL type { std::cout << "\typedec -> TYPE_KW typeid '=' type\n"; $$ = new decls::TypeDecl($2,$4); }
 ;

type: typeid { std::cout << "\ttype -> typeid\n"; $$ = new types::BasicType($1); }
 | LBRACE typefields RBRACE { std::cout << "\ttype -> LBRACE typefields RBRACE\n";  $$ = new types::TypeFeildType($2); }
 | LBRACE RBRACE { /* typefields is optional */ std::cout << "\ttype -> LBRACE RBRACE \n"; $$ = new types::TypeFeildType(new TypeFeildsNode()); }
 | ARRAY_KW OF_KW typeid { std::cout << "\ttype -> ARRAY_KW OF_KW typeid\n"; $$ = new types::ArrayType($3); }
 ;

typefields: typefield { std::cout << "\ttypefields -> typefield\n"; $$ = new TypeFeildsNode(); $$->append_to($1); }
 | typefields ',' typefield { std::cout << "\ttypefields -> typefields ',' typefield\n"; $$ = $1; $$->append_to($3); }
 ;
typefield: IDENTIFIER COLON typeid { std::cout << "\ttypefield -> IDENTIFIER\n"; $$ = new TypeFeildNode($1,$3); }
 ;

exprseq: expr { std::cout << "\texprseq -> expr\n"; $$ = new ExprSequenceNode(); $$->append_to($1); }
 | exprseq SEMICOLON expr { std::cout << "\texprseq ';' expr\n"; $$ = $1; $$->append_to($3); }
 ;

exprlist: expr { std::cout << "\texprlist -> expr\n";  $$ = new ExprListNode(); $$->append_to($1); }
 | exprlist COMMA expr { std::cout << "exprlist -> exprlist ',' expr\n";  $$ = $1; $$->append_to($3); }
 ;

typeid: IDENTIFIER { std::cout << "\ttypeid -> IDENTIFIER\n"; $$ = new TypeIDNode($1); }


lvalue: IDENTIFIER { std::cout << "\tlvalue -> IDENTIFIER\n"; $$ = new lvals::IdLval($1); }
    | lvalue PERIOD IDENTIFIER { std::cout << "\tlvalue -> PERIOD IDENTIFIER\n"; $$ = new lvals::AttrAccess($1,$3);  }
    ;
%%
