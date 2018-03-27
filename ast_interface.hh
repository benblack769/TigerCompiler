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
#include "symbol_table.hh"


namespace tiger {


class ExprNode: public ASTNode {
 public:
  ExprNode() = default;
  virtual ~ExprNode() = default;
  virtual void check_type_scopes(TypeTable & env){}
  //virtual int expr_type(Envirornement & env) = 0;
  virtual void print(std::ostream & os) const override = 0;
};

class LvalueNode: public ASTNode {
 public:
  LvalueNode() = default;
  virtual ~LvalueNode() = default;
  virtual void print(std::ostream & os) const override = 0;
};

enum class DeclType{VAR, FUNC, TYPE};
class DeclarationNode: public ASTNode {
 public:
  DeclarationNode() = default;
  virtual ~DeclarationNode() = default;
  virtual std::string name() = 0;
  virtual DeclType type() = 0;
  virtual void print(std::ostream & os) const override = 0;
};

class TypeNode: public ASTNode {
 public:
  TypeNode() = default;
  virtual ~TypeNode() = default;
  virtual UnresolvedType unresolved_type() = 0;
  virtual void print(std::ostream & os) const override = 0;
};

class ExprListNode: public ASTNode {
 public:
  ExprListNode(){}
  virtual ~ExprListNode(){}
  virtual void append_to(ExprNode * expr){
      list.push_back(std::unique_ptr<ExprNode>(expr));
  }
  virtual void check_type_scopes(TypeTable & env){
      for(auto & item : list){
          item->check_type_scopes(env);
      }
  }
  virtual void print(std::ostream & os) const override {
      print_list(os, base_list(list), ", ");
  }
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
  virtual bool empty(){
      return list.size() == 0;
  }
  virtual bool singleton(){
      return list.size() == 1;
  }
  virtual void check_type_scopes(TypeTable & env){
      for(auto & item : list){
          item->check_type_scopes(env);
      }
  }
  virtual void print(std::ostream & os) const override {
      print_list(os, base_list(list), ";\n");
  }
  std::vector<std::unique_ptr<ExprNode>> list;
};

class FieldNode: public ASTNode{
 public:
  FieldNode(std::string in_id, ExprNode * in_expr):
    id(in_id),
    expr(in_expr){}
  virtual ~FieldNode(){
      delete expr;
  }
  virtual void check_type_scopes(TypeTable & env){
      expr->check_type_scopes(env);
  }
  virtual void print(std::ostream & os) const override{
      os << id << " = " << *expr;
  }
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
  virtual void in_scope(TypeTable & table){
      if(!table.has_type(my_id)){
          throw runtime_error("type out of scope!");
      }
  }
  virtual void print(std::ostream & os) const override{
      os << my_id;
  }
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
  virtual void check_type_scopes(TypeTable & env){
      for(auto & item : list){
          item->check_type_scopes(env);
      }
  }
  virtual void print(std::ostream & os) const override {
      print_list(os, base_list(list), ",");
  }
  std::vector<std::unique_ptr<FieldNode>> list;
};

class DeclarationListNode: public ASTNode {
 public:
   DeclarationListNode(){}
   virtual ~DeclarationListNode(){}
   virtual void append_to(DeclarationNode * expr);
   virtual void print(std::ostream & os) const override ;
   virtual void load_and_check_types(TypeTable & env);
   std::vector<std::unique_ptr<DeclarationNode>> list;
};

class TypeFeildsNode: public ASTNode {
 public:
   TypeFeildsNode(){}
   virtual ~TypeFeildsNode(){}
   virtual void append_to(TypeFeildNode * expr){
       list.push_back(std::unique_ptr<TypeFeildNode>(expr));
   }
   vector<pair<string,string>> get_record_fields(){
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
};

} //namespace tiger
