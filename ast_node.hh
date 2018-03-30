#pragma once
#include "helper_files/source_loc.hh"


namespace tiger {
///////////////////////////////////////////////////////////////////////////////
// Base AST node class, to define the hierarchy.
class ASTNode {
protected:
    SourceLoc loc;
 public:
  using ASTptr = const ASTNode*; // Can't use smart ptr in union :(

  ASTNode() = default;
  virtual void set_source_loc(SourceLoc in_loc){ loc = in_loc; }
  virtual SourceLoc get_source_loc(){return loc;}
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

}
