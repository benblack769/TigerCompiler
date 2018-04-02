#pragma once
#include "ast_interface.hh"

namespace tiger {

namespace lvals{
class IdLval : public LvalueNode {
 public:
  IdLval(std::string in_id):
     my_id(in_id){}
  virtual ~IdLval(){
  }
  virtual TypeExpr get_type(SymbolTable & env)override{
      assert_err(env.symbol_is_var(my_id), SematicError::VARIABLE_NOT_DEFINED, loc);
      return env.var_type(my_id);
  }
  virtual void print(std::ostream & os) const override{
      os << my_id;
  }
  virtual IR_TREE_CLASS_NAME translate() const override;
 private:
  std::string my_id;
};

class AttrAccess : public LvalueNode {
 public:
  AttrAccess(LvalueNode * lval, std::string in_id):
     lvalnode(lval),
     my_id(in_id){}

  virtual ~AttrAccess(){
      delete lvalnode;
  }
  virtual TypeExpr get_type(SymbolTable & env) override{
      TypeExpr sub_ty = lvalnode->get_type(env);
      assert_err(sub_ty.type == BaseType::RECORD, SematicError::INCOMPATABLE_RECORD_LABEL, loc);
      assert_err(env.record_subexpr_exists(sub_ty,my_id),SematicError::INCOMPATABLE_RECORD_LABEL, loc);
      return env.get_record_subexpr(sub_ty,my_id);
  }
  virtual void print(std::ostream & os) const override{
      os << *lvalnode << "." << my_id;
  }
  virtual IR_TREE_CLASS_NAME translate() const override;
 private:
  LvalueNode * lvalnode;
  std::string my_id;
};

class BracketAccess : public LvalueNode {
 public:
  BracketAccess(LvalueNode * lval, ExprNode * expr):
     _lval(lval),
     _expr(expr){}

  virtual ~BracketAccess(){
      delete _lval;
      delete _expr;
  }
  virtual TypeExpr get_type(SymbolTable & env)override{
      TypeExpr sub_ty = _lval->get_type(env);
      assert_err(sub_ty.type == BaseType::ARRAY, SematicError::INCOMPATABLE_RECORD_LABEL, loc);
      assert_type_equality(_expr->eval_and_check_type(env),int_type(),loc);
      return env.array_subtype(sub_ty);
  }
  virtual void print(std::ostream & os) const override{
      os << *_lval << "[" << *_expr << "]";
  }
  virtual IR_TREE_CLASS_NAME translate() const override;
 private:
  LvalueNode * _lval;
  ExprNode * _expr;
};
}

}
