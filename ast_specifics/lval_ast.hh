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

  virtual void print(std::ostream & os) const override{
      os << my_id;
  }
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
  virtual void print(std::ostream & os) const override{
      os << *lvalnode << "." << my_id;
  }
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
  virtual void print(std::ostream & os) const override{
      os << *_lval << "[" << *_expr << "]";
  }
 private:
  LvalueNode * _lval;
  ExprNode * _expr;
};
}

}
