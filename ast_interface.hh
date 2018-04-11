#pragma once
#include <string>
#include <cstdlib>
#include <ostream>
#include <vector>
#include <memory>
#include <cassert>
#include <iostream>
#include "semantic_check.hh"
#include "helper_files/unresolved_type_info.hh"
#include "helper_files/list_helper.hh"
#include "ast_node.hh"
#include "helper_files/type_assertion.hh"
#include "symbol_table.hh"
#include "irt.hh"
#include "frame_stack.hh"


namespace tiger {

using IRTptr = ir::IRTNode::IRTptr;

class ExprNode: public ASTNode {
 public:
  ExprNode() = default;
  virtual ~ExprNode() = default;
  virtual TypeExpr eval_and_check_type(SymbolTable & env) = 0;
  //virtual int expr_type(Envirornement & env) = 0;
  virtual void print(std::ostream & os) const override = 0;
  virtual IRTptr translate(SymbolTable & env) const = 0;
};

class LvalueNode: public ASTNode {
 public:
  LvalueNode() = default;
  virtual ~LvalueNode() = default;
  virtual TypeExpr get_type(SymbolTable & env) = 0;
  virtual void print(std::ostream & os) const override = 0;
  virtual IRTptr translate(SymbolTable & env) const = 0;
};

enum class DeclType{VAR, FUNC, TYPE};
class DeclarationNode: public ASTNode {
 public:
  DeclarationNode() = default;
  virtual ~DeclarationNode() = default;
  virtual std::string name() = 0;
  virtual DeclType type() = 0;
  virtual void print(std::ostream & os) const override = 0;
  virtual IRTptr translate(SymbolTable & env) const = 0;
};

class TypeNode: public ASTNode {
 public:
  TypeNode() = default;
  virtual ~TypeNode() = default;
  virtual UnresolvedType unresolved_type() = 0;
  virtual void print(std::ostream & os) const override = 0;
  virtual IRTptr translate(SymbolTable & env) const = 0;
};

class ExprListNode: public ASTNode {
 public:
  ExprListNode(){}
  virtual ~ExprListNode(){}
  virtual void append_to(ExprNode * expr){
      list.push_back(std::unique_ptr<ExprNode>(expr));
  }
  virtual vector<TypeExpr> eval_and_check_type(SymbolTable & env){
      vector<TypeExpr> arg_types;
      for(auto & item : list){
          arg_types.push_back(item->eval_and_check_type(env));
      }
      return arg_types;
  }
  virtual void print(std::ostream & os) const override {
      print_list(os, base_list(list), ", ");
  }
  virtual IRTptr translate(SymbolTable & env) const;
protected:
  std::vector<std::unique_ptr<ExprNode>> list;
};

class ExprSequenceNode: public ASTNode {
 public:
  ExprSequenceNode(){}
  virtual ~ExprSequenceNode(){}
  virtual void append_to(ExprNode * expr){
      list.push_back(std::unique_ptr<ExprNode>(expr));
  }
  virtual bool empty() const{
      return list.size() == 0;
  }
  virtual bool singleton() const{
      return list.size() == 1;
  }
  virtual TypeExpr eval_and_check_type(SymbolTable & env){
      for(auto & item : list){
          item->eval_and_check_type(env);
      }
      return list.size() == 0 ? void_type() : list.back()->eval_and_check_type(env);
  }
  virtual void print(std::ostream & os) const override {
      print_list(os, base_list(list), ";\n");
  }
  std::vector<std::unique_ptr<ExprNode>> list;
  virtual IRTptr translate(SymbolTable & env) const;
};

class FieldNode: public ASTNode{
 public:
  FieldNode(std::string in_id, ExprNode * in_expr):
    id(in_id),
    expr(in_expr){}
  virtual ~FieldNode(){
      delete expr;
  }
  virtual pair<string,TypeExpr> eval_and_check_type(SymbolTable & env){
       return make_pair(id, expr->eval_and_check_type(env));
  }
  virtual void print(std::ostream & os) const override{
      os << id << " = " << *expr;
  }
  virtual IRTptr translate(SymbolTable & env) const;
protected:
    std::string id;
    ExprNode * expr;
};

class TypeIDNode : public ASTNode {
 public:
  TypeIDNode(std::string in_id):
     my_id(in_id){}
  virtual ~TypeIDNode(){
  }
  virtual std::string id_name(){
      return my_id;
  }
  virtual TypeExpr eval_and_check_type(SymbolTable & env){
      return env.get_checked_type(my_id);
  }
  /*virtual void in_scope(SymbolTable & table){
      cout << "reached spot" << endl;
      if(!table.has_type(my_id)){
          throw SemanticException(SematicError::TYPE_NOT_DEFINED);
      }
  }*/
  virtual void print(std::ostream & os) const override{
      os << my_id;
  }
  virtual IRTptr translate(SymbolTable & env) const;
 private:
  std::string my_id;
};

class TypeFeildNode: public ASTNode {
 public:
  TypeFeildNode(std::string in_id, TypeIDNode * in_ty):
    id(in_id),
    ty(in_ty){}
  virtual ~TypeFeildNode(){
      delete ty;
  }
  pair<string, string> field_pair(){
      return make_pair(id, ty->id_name());
  }
  virtual void print(std::ostream & os) const override{
      os << id << " : " << *ty;
  }
  virtual IRTptr translate(SymbolTable & env) const;
protected:
    std::string id;
    TypeIDNode * ty;
};

class FieldListNode: public ASTNode {
 public:
  FieldListNode(){}
  virtual ~FieldListNode(){}
  virtual void append_to(FieldNode * expr){
      list.push_back(std::unique_ptr<FieldNode>(expr));
  }
  virtual vector<pair<string,TypeExpr>> eval_and_check_type_names(SymbolTable & env){
      vector<pair<string,TypeExpr>> rec_types;
      for(auto & item : list){
          rec_types.push_back(item->eval_and_check_type(env));
      }
      return rec_types;
  }
  virtual void print(std::ostream & os) const override {
      print_list(os, base_list(list), ",");
  }
  std::vector<std::unique_ptr<FieldNode>> list;
  virtual IRTptr translate(SymbolTable & env) const;
};

class DeclarationListNode: public ASTNode {
 public:
   DeclarationListNode(){}
   virtual ~DeclarationListNode(){}
   virtual void append_to(DeclarationNode * expr);
   virtual void print(std::ostream & os) const override ;
   virtual void load_and_check_types(SymbolTable & env);
   std::vector<std::unique_ptr<DeclarationNode>> list;
   virtual IRTptr translate(SymbolTable & env) const;
};

class TypeFeildsNode: public ASTNode {
 public:
   TypeFeildsNode(){}
   virtual ~TypeFeildsNode(){}
   virtual void append_to(TypeFeildNode * expr){
       list.push_back(std::unique_ptr<TypeFeildNode>(expr));
   }
   vector<pair<string,string>> get_field_pairs(){
       vector<pair<string,string>> res;
       for(auto & val : list){
           res.push_back(val->field_pair());
       }
       return res;
   }
   virtual void print(std::ostream & os) const override {
       print_list(os, base_list(list), ", ");
   }
   std::vector<std::unique_ptr<TypeFeildNode>> list;
   virtual IRTptr translate(SymbolTable & env) const;
};

} //namespace tiger
