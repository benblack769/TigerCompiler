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
    std::string name(){
        return _id;
    }
    bool has_type(){
        return _has_type_decl;
    }
    std::string type_name(){
        return _type_id->id_name();
    }
    DeclType type(){
        return DeclType::VAR;
    }
    virtual void print(std::ostream & os) const override{
        os << " var " << _id;
        if(_has_type_decl){
            os << " : " << *_type_id;
        }
        os <<  " := " << *_expr;
    }
     std::string _id;
     TypeIDNode * _type_id;
     ExprNode * _expr;
     bool _has_type_decl;
  virtual IRTptr translate(SymbolTable & env) const override;
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
    std::string name(){
        return _id;
    }
    DeclType type(){
        return DeclType::FUNC;
    }
    bool has_type(){
        return _has_type_decl;
    }
    int number_args(){
        return _ty_fields->get_field_pairs().size();
    }
    vector<pair<string, string>> arg_types(){
        vector<pair<string, string>> func_args = _ty_fields->get_field_pairs();
        assert_err(first_strs_unique(func_args),SematicError::NON_UNIQUE_FUNCTION_ARGS, loc);
        return func_args;
    }
    string ret_type(){
        return _type_id->id_name();
    }
    virtual void print(std::ostream & os) const override{
        os << " function " << _id << "(" << *_ty_fields << ")";
        if(_has_type_decl){
            os << " : " << *_type_id;
        }
        os <<  " = " << *_expr;
    }
    virtual IRTptr translate(SymbolTable & env) const override;
 //protected:
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
    std::string name(){
        return _id->id_name();
    }
    DeclType type(){
        return DeclType::TYPE;
    }
    UnresolvedType typenode(){ return _type->unresolved_type(); }
    virtual void print(std::ostream & os) const override{
        os << " type " << *_id << "=" << *_type;
    }
  virtual IRTptr translate(SymbolTable & env) const override;
 protected:
     TypeIDNode * _id;
     TypeNode * _type;
};
}

} // namespace
