#pragma once
/* Companion source code for "flex & bison", published by O'Reilly
 * Media, ISBN 978-0-596-15597-1
 * Copyright (c) 2009, Taughannock Networks. All rights reserved.
 * See the README file for license conditions and contact info.
 * $Header: /home/johnl/flnb/code/RCS/fb3-1.h,v 2.1 2009/11/08 02:53:18 johnl Exp $
 */
/*
 * Declarations for a calculator fb3-1
 */

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
  using value_t = double;  // All values will be floating-point
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
    StringASTNode(char * in_str):
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
