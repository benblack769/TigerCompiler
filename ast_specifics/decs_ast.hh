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
    void get_and_check_var_type(SymbolTable & old_env){
        TypeExpr evaled_expr = this->_expr->eval_and_check_type(old_env);
        TypeExpr res_expr;
        if(this->has_type()){
            res_expr = old_env.get_checked_type(this->type_name());
            assert_type_equality(evaled_expr,res_expr,this->get_source_loc());
        }
        else{
            res_expr = evaled_expr;
        }
        old_env.add_variable(this->name(),res_expr,full_frame.current_level(), full_frame.current_frame()->allocLocal(true));
        new_env = old_env;
    }
    virtual void print(std::ostream & os) const override{
        os << " var " << _id;
        if(_has_type_decl){
            os << " : " << *_type_id;
        }
        os <<  " := " << *_expr;
    }
     SymbolTable new_env;
     std::string _id;
     TypeIDNode * _type_id;
     ExprNode * _expr;
     bool _has_type_decl;
  virtual IRTptr translate(const SymbolTable & env) const override;
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
    void check_function_body(SymbolTable & old_env){
        new_env = old_env;
        full_frame.new_frame(newFrame(newlabel().toString(),formalsList(this->number_args())));
        for(auto var_pair : this->arg_types()){
            new_env.add_variable(var_pair.first,new_env.get_checked_type(var_pair.second),full_frame.current_level(),full_frame.current_frame()->allocLocal(true));
        }
        TypeExpr func_ret_ty = this->_expr->eval_and_check_type(new_env);
        if(this->has_type()){
            assert_type_equality(func_ret_ty,new_env.get_checked_type(this->ret_type()),this->get_source_loc());
        }
        full_frame.pop_frame();
    }
    virtual IRTptr translate(const SymbolTable & env) const override;
 //protected:
     SymbolTable new_env;
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
  virtual IRTptr translate(const SymbolTable & env) const override;
 protected:
     TypeIDNode * _id;
     TypeNode * _type;
};
}

} // namespace
