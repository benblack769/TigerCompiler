#pragma once
#include "ast_interface.hh"

namespace tiger {


namespace decls{
class VarDecl: public DeclarationNode {
 public:
  VarDecl(std::string id, TypeIDNode * type_id, ExprNode * expr, bool has_type_decl):
    _id(id),
    _type_id(type_id),
    _expr(expr),
    _has_type_decl(has_type_decl){}

    virtual ~VarDecl(){
        delete _type_id;
        delete _expr;
    }
    virtual void print(std::ostream & os) const override{
        os << " var " << _id;
        if(_has_type_decl){
            os << " : " << *_type_id;
        }
        os <<  " := " << *_expr;
    }
 protected:
     std::string _id;
     TypeIDNode * _type_id;
     ExprNode * _expr;
     bool _has_type_decl;
};
class FuncDecl: public DeclarationNode {
 public:
  FuncDecl(std::string id, TypeFeildsNode * ty_fields, TypeIDNode * type_id, ExprNode * expr, bool has_type_decl):
    _id(id),
    _ty_fields(ty_fields),
    _type_id(type_id),
    _expr(expr),
    _has_type_decl(has_type_decl){}

    virtual ~FuncDecl(){
        delete _ty_fields;
        delete _type_id;
        delete _expr;
    }
    virtual void print(std::ostream & os) const override{
        os << " function " << _id << "(" << *_ty_fields << ")";
        if(_has_type_decl){
            os << " : " << *_type_id;
        }
        os <<  " = " << *_expr;
    }
 protected:
     std::string _id;
     TypeFeildsNode * _ty_fields;
     TypeIDNode * _type_id;
     ExprNode * _expr;
     bool _has_type_decl;
};
class TypeDecl: public DeclarationNode {
 public:
  TypeDecl(TypeIDNode * id, TypeNode * type):
    _id(id),
    _type(type){}

    virtual ~TypeDecl(){
        delete _id;
        delete _type;
    }
    virtual void print(std::ostream & os) const override{
        os << " type " << *_id << "=" << *_type;
    }
 protected:
     TypeIDNode * _id;
     TypeNode * _type;
};
}

} // namespace
