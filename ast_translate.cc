#include <vector>
#include "ast_interface.hh"
#include "ast_specifics/expr_ast.hh"
#include "ast_specifics/lval_ast.hh"
#include "ast_specifics/decs_ast.hh"
#include "ast_specifics/types_ast.hh"

namespace tiger{

IRTptr exprs::StringNode::translate() const{return nullptr;}
IRTptr exprs::NilNode::translate() const{return nullptr;}
IRTptr exprs::IntNode::translate() const{return nullptr;}
IRTptr exprs::LvalNode::translate() const{return nullptr;}
IRTptr exprs::NegateNode::translate() const{return nullptr;}
IRTptr exprs::BinaryNode::translate() const{return nullptr;}
IRTptr exprs::AssignNode::translate() const{return nullptr;}
IRTptr exprs::FunctionCall::translate() const{return nullptr;}
IRTptr exprs::ExprSequenceEval::translate() const{return nullptr;}
IRTptr exprs::IfThen::translate() const{return nullptr;}
IRTptr exprs::IfThenElse::translate() const{return nullptr;}
IRTptr exprs::WhileDo::translate() const{return nullptr;}
IRTptr exprs::ForToDo::translate() const{return nullptr;}
IRTptr exprs::Break::translate() const{return nullptr;}
IRTptr exprs::ArrCreate::translate() const{return nullptr;}
IRTptr exprs::RecCreate::translate() const{return nullptr;}
IRTptr exprs::LetIn::translate() const{return nullptr;}

IRTptr lvals::IdLval::translate() const{return nullptr;}
IRTptr lvals::AttrAccess::translate() const{return nullptr;}
IRTptr lvals::BracketAccess::translate() const{return nullptr;}

IRTptr decls::VarDecl::translate() const{return nullptr;}
IRTptr decls::FuncDecl::translate() const{return nullptr;}
IRTptr decls::TypeDecl::translate() const{return nullptr;}

IRTptr types::BasicType::translate() const{return nullptr;}
IRTptr types::ArrayType::translate() const{return nullptr;}
IRTptr types::TypeFeildType::translate() const{return nullptr;}

IRTptr ExprListNode::translate() const{return nullptr;}
IRTptr ExprSequenceNode::translate() const{return nullptr;}
IRTptr FieldNode::translate() const{return nullptr;}
IRTptr TypeIDNode::translate() const{return nullptr;}
IRTptr TypeFeildNode::translate() const{return nullptr;}
IRTptr FieldListNode::translate() const{return nullptr;}
IRTptr DeclarationListNode::translate() const{return nullptr;}
IRTptr TypeFeildsNode::translate() const{return nullptr;}

} //namespace
