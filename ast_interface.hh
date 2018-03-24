#pragma once
#include <string>
#include <cstdlib>
#include <ostream>
#include <vector>
#include <memory>
#include <cassert>
#include <iostream>

struct SourceLoc{
    int start_line = -1;
    int start_col = -1;
    int end_line = -1;
    int end_col = -1;
};

inline std::ostream & operator << (std::ostream & os, const SourceLoc & loc){
    if(loc.start_line == loc.end_line){
        return os << "line:" << loc.start_line;// << ", col: (" << loc.start_col << ", " << loc.end_col << ")";
    }
    else{
        return os << "from line:" << loc.start_line // ", col:" << loc.start_col
                        << " to " << loc.end_line;// << ", col:" << loc.end_col;
    }
}

namespace tiger {

///////////////////////////////////////////////////////////////////////////////
// Base AST node class, to define the hierarchy.
class ASTNode {
protected:
    SourceLoc loc;
 public:
  using ASTptr = const ASTNode*; // Can't use smart ptr in union :(

  ASTNode() = default;
  virtual void set_source_loc(SourceLoc in_loc){ loc = in_loc; std::cout << loc << std::endl;}
  virtual ~ASTNode() = default;
  virtual void print(std::ostream & os) const = 0;
};

inline std::ostream & operator << (std::ostream & os, const ASTNode & node){
    node.print(os);
    return os;
}
inline std::ostream & operator << (std::ostream & os, const ASTNode * node){
    node->print(os);
    return os;
}

class ExprNode: public ASTNode {
 public:
  ExprNode() = default;
  virtual ~ExprNode() = default;
  virtual void print(std::ostream & os) const override = 0;
};

class LvalueNode: public ASTNode {
 public:
  LvalueNode() = default;
  virtual ~LvalueNode() = default;
  virtual void print(std::ostream & os) const override = 0;
};

class DeclarationNode: public ASTNode {
 public:
  DeclarationNode() = default;
  virtual ~DeclarationNode() = default;
  virtual void print(std::ostream & os) const override = 0;
};

class TypeNode: public ASTNode {
 public:
  TypeNode() = default;
  virtual ~TypeNode() = default;
  virtual void print(std::ostream & os) const override = 0;
};

//helpers for list nodes
template<class item_ty>
inline std::vector<ASTNode *> base_list(const std::vector<std::unique_ptr<item_ty>> & list){
    std::vector<ASTNode *> res;
    for(auto & val : list){
        res.push_back(val.get());
    }
    return res;
}

inline void print_list(std::ostream & os, const std::vector<ASTNode *> & item_list, std::string sep){
    for(size_t i = 0; i < item_list.size(); i++){
        os << *item_list[i];
        if(i != item_list.size()-1){
            os << sep;
        }
    }
}

class ExprListNode: public ASTNode {
 public:
  ExprListNode(){}
  virtual ~ExprListNode(){}
  virtual void append_to(ExprNode * expr){
      list.push_back(std::unique_ptr<ExprNode>(expr));
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
  virtual void print(std::ostream & os) const override {
      print_list(os, base_list(list), ",");
  }
  std::vector<std::unique_ptr<FieldNode>> list;
};


class DeclarationListNode: public ASTNode {
 public:
   DeclarationListNode(){}
   virtual ~DeclarationListNode(){}
   virtual void append_to(DeclarationNode * expr){
       list.push_back(std::unique_ptr<DeclarationNode>(expr));
   }
   virtual void print(std::ostream & os) const override {
       print_list(os, base_list(list), "\n");
   }
   std::vector<std::unique_ptr<DeclarationNode>> list;
};
class TypeFeildsNode: public ASTNode {
 public:
   TypeFeildsNode(){}
   virtual ~TypeFeildsNode(){}
   virtual void append_to(TypeFeildNode * expr){
       list.push_back(std::unique_ptr<TypeFeildNode>(expr));
   }
   virtual void print(std::ostream & os) const override {
       print_list(os, base_list(list), ", ");
   }
   std::vector<std::unique_ptr<TypeFeildNode>> list;
};

} //namespace tiger
