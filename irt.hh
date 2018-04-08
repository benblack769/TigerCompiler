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

// class for things that dont return a value
class stm: public IRTNode {
  public:
    using stmPtr = const shared_ptr<stm>;
    
    stm() = default;
    virtual ~stm() = default;
    virtual std::string toStr(std::string spacing) const = 0;
};

// class for things that do return a value
class exp: public IRTNode {
  public:
    using expPtr = const shared_ptr<exp>;
    
    exp() = default;
    virtual ~exp() = default;
    virtual std::string toStr(std::string spacing) const = 0;
};

namespace {
    // helper functions so that our toStrings look almost the same
    std::string oneChildToStr(std::string spacing, std::string name, IRTNode::IRTptr child) {
        return spacing + name + ":\n" + child->toStr(spacing+ws);
    };
    std::string twoChildToStr(std::string spacing, std::string name, IRTNode::IRTptr child1, IRTNode::IRTptr child2) {
        return spacing + name + ":\n" + child1->toStr(spacing+ws) + "\n" + child2->toStr(spacing+ws);
    };

    // ptr names 
    // so that you can get expPtr in things that inherit from stm and vice versa
    using expPtr = exp::expPtr;
    using stmPtr = stm::stmPtr;
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

////////////////////////////////////////////////////////////////////////////////
// Here starts the real classes
// They're all from 153-154 in Appel's book
////////////////////////////////////////////////////////////////////////////////

// holds a constant word
class Const: public exp {
  public:
    Const(const_t val): val_(val) {};
    virtual ~Const() = default;
    virtual std::string toStr(std::string spacing) const {
        return spacing + "Const: " + std::to_string(val_);
    };
  private:
    const const_t val_;
};

// symbolic constant n
/*
class Name: public exp {
  public:
    Name(const_t val): val_(val) {};
    virtual ~Const() = default;
    virtual std::string toStr(std::string spacing) const {
        return spacing + "Const: " + std::to_string(val_);
    };
  private:
    const const_t val_;
};
*/

// Sequence of two nodes
// stm_ gets evaluated for side effects
// exp_ gets evaluated for value
class Eseq: public exp {
  public:
    Eseq(stmPtr l, expPtr r): stm_(l), exp_(r)
        {};
    virtual ~Eseq() = default;
    virtual std::string toStr(std::string spacing) const {
        return twoChildToStr(spacing, "Eseq", stm_, exp_);
    };
  private:
    stmPtr stm_;
    expPtr exp_;
};
// The result of the move command will put the value from src_ into the
// memory location or temporary variable at dest_
class Move: public stm {
  public:
    Move(expPtr dest, expPtr src): dest_(dest), src_(src)
        {};
    virtual ~Move() = default;
    virtual std::string toStr(std::string spacing) const {
        return twoChildToStr(spacing, "Eseq", dest_, src_);
    };
  private:
    expPtr dest_;
    expPtr src_;

};

// evauates exp_ and returns the value at that loaction
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



} // ir namespace
} // tiger

