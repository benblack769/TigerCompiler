/* irt.hh
 * api for the Intermediate Representation tree
 */
#pragma once
#include <iostream>

namespace tiger {
namespace ir {

using const_t = int; //type that Const node will take
const std::string ws = "  "; // whitespace for toStr


// Base IRT node class, to define the hierarchy.
class IRTNode {
  public:
    using IRTptr = const shared_ptr<IRTNode>;
    
    IRTNode() = default;
    virtual ~IRTNode() = default;
    virtual std::string toStr(std::string spacing = "") const = 0;
};

namespace {
    // helper functions so that our toStrings look almost the same
    std::string oneChildToStr(std::string spacing, std::string name, IRTNode::IRTptr child) {
        return spacing + name + ":\n" + child->toStr(spacing+ws);
    };
    std::string twoChildToStr(std::string spacing, std::string name, IRTNode::IRTptr child1, IRTNode::IRTptr child2) {
        return spacing + name + ":\n" + child1->toStr(spacing+ws) + "\n" + child2->toStr(spacing+ws);
    };
}

// printing stuff
inline std::ostream & operator << (std::ostream & os, const IRTNode & node){
    os << node.toStr() << std::endl;
    return os;
}
inline std::ostream & operator << (std::ostream & os, const IRTNode * node){
    os << node->toStr() << std::endl;
    return os;
}

class Eseq: public IRTNode {
  public:
    Eseq(IRTptr stm, IRTptr exp): stm_(stm), exp_(exp)
        {};
    virtual ~Eseq() = default;
    virtual std::string toStr(std::string spacing) const {
        return twoChildToStr(spacing, "Eseq", stm_, exp_);
    };
  private:
    IRTptr stm_;
    IRTptr exp_;
};
// The result of the move command will put the value from src_ into the
// memory location or temporary variable at dest_
class Move: public IRTNode {
  public:
    Move(IRTptr dest, IRTptr src): dest_(dest), src_(src)
        {};
    virtual ~Move() = default;
    virtual std::string toStr(std::string spacing) const {
        return twoChildToStr(spacing, "Eseq", dest_, src_);
    };
  private:
    IRTptr dest_;
    IRTptr src_;

};
class Mem: public IRTNode {
  public:
    Mem(IRTptr exp): exp_(exp){};
    virtual ~Mem() = default;
    virtual std::string toStr(std::string spacing) const {
        return oneChildToStr(spacing, "Mem", exp_);
    }
  private:
    IRTptr exp_;
};
class Const: public IRTNode {
  public:
    Const(const_t val): val_(val) {};
    virtual ~Const() = default;
    virtual std::string toStr(std::string spacing) const {
        return spacing + "Const: " + std::to_string(val_);
    };
  private:
    const const_t val_;
};


} // ir namespace
} // tiger

