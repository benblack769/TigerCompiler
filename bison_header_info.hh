#pragma once
void yyerror(char *s, ...);
struct ErrorInfo{

};
namespace tiger{
class ASTNode;
class ExprNode;
class LvalueNode;
class DeclarationNode;
class TypeNode;
class ExprListNode;
class ExprSequenceNode;
class FieldNode;
class TypeIDNode;
class TypeFeildNode;
class FieldListNode;
class DeclarationListNode;
class TypeFeildsNode;
}
struct IdArray{
    char * id;
    tiger::ExprNode* arrexpr;
};
