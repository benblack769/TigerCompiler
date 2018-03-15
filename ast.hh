#pragma once
#include <string>
#include <cstdlib>
#include <ostream>

/* interface to the lexer */
void yyerror(char *s, ...);

namespace tiger {

///////////////////////////////////////////////////////////////////////////////
// Base AST node class, to define the hierarchy.
class ASTNode {
 public:
  using ASTptr = const ASTNode*; // Can't use smart ptr in union :(

  ASTNode() = default;
  virtual ~ASTNode() = default;
  virtual void print(std::ostream & os) const = 0; // For void printing purposes
};

//} //namespace tiger

inline std::ostream & operator << (std::ostream & os, const ASTNode & node){
    node.print(os);
    return os;
}

//namespace tiger {

class StringASTNode: public ASTNode{
public:
    StringASTNode(const char * in_str):
        mystring(in_str){}

    virtual ~StringASTNode() = default;
    virtual void print(std::ostream & os) const override{
        os << '"' << mystring << '"';
    }
protected:
    std::string mystring;
};

class IntASTNode: public ASTNode{
public:
    IntASTNode(int64_t in_int):
        my_int(in_int){}

    virtual ~IntASTNode() = default;
    virtual void print(std::ostream & os) const override{
        os << my_int;
    }
protected:
    int64_t my_int;
};

///////////////////////////////////////////////////////////////////////////////
// A node type that evaluates to a numeric constant:
class IdASTNode : public ASTNode {
 public:
  IdASTNode(char * in_id):
     my_id(in_id){}
  virtual ~IdASTNode() = default;

  virtual void print(std::ostream & os) const override{
      os << my_id;
  }
 private:
  std::string my_id;
};

class NegateASTNode : public ASTNode {
 public:
    NegateASTNode(ASTptr child)
        :child_(child){}

    virtual ~NegateASTNode()
    {
        delete child_;
    }
    virtual void print(std::ostream & os) const override{
        os << '-' << *child_;
    }
private:
    ASTptr child_;
};

} // namespace
