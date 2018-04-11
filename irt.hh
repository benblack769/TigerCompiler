/* irt.hh
 * api for the Intermediate Representation tree
 */
#pragma once
#include <iostream>
#include <vector>
#include <assert.h>
#include <memory>
#include "temp.hh"

namespace ir {

using const_t = int; //type that Const node will take
const std::string ws = "  "; // whitespace for toStr
using label_t = Temp_label; // may need to get this from the temp class
using temp_t = Temp_temp; // may need to get this from the temp class
using labelList = std::vector<label_t>;


// Base IRT node class, to define the hierarchy.
class IRTNode {
  public:
    using IRTptr = std::shared_ptr<IRTNode>;
    
    IRTNode() = default;
    virtual ~IRTNode() = default;
    virtual std::string toStr(std::string spacing = "") const = 0;
};

// class for things that dont return a value
class stm: public IRTNode {
  public:
    using stmPtr = std::shared_ptr<stm>;
    
    stm() = default;
    virtual ~stm() = default;
    virtual std::string toStr(std::string spacing) const = 0;
};

// class for things that do return a value
class exp: public IRTNode {
  public:
    using expPtr = std::shared_ptr<exp>;
    
    exp() = default;
    virtual ~exp() = default;
    virtual std::string toStr(std::string spacing) const = 0;
};

// like an expList but with pointers
using expPtrList = std::vector<std::shared_ptr<exp>>;
using stmPtrList = std::vector<std::shared_ptr<stm>>;

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
using IRTptr = IRTNode::IRTptr;
template<class expr_ty>
inline expPtr to_expPtr(const expr_ty & expr){
    return std::shared_ptr<exp>(new expr_ty(expr));
}
template<class stm_ty>
inline stmPtr to_stmPtr(const stm_ty & stmt){
    return std::shared_ptr<stm>(new stm_ty(stmt));
}
inline expPtr cast_to_exprPtr(IRTptr ptr){
    expPtr res = std::dynamic_pointer_cast<exp>(ptr);
    assert(bool(res));
    return res;
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

// symbolic constant n corresponding to an asembly language label
class Name: public exp {
  public:
    Name(label_t val): val_(val) {};
    virtual ~Name() = default;
    virtual std::string toStr(std::string spacing) const {
        return spacing + "Name: " + val_.toString();
    };
  private:
    const label_t val_;
};

// Temportary, like a register but we have infinite of them
class Temp: public exp {
  public:
    Temp(temp_t val): val_(val) {};
    virtual ~Temp() = default;
    virtual std::string toStr(std::string spacing) const {
        return spacing + "Temp: " + val_.toString();
    };
  private:
    const temp_t val_;
};

enum class op_k: int                {PLUS, MINUS, MUL, DIV, AND,  OR, XOR, LSHIFT, RSHIFT, ARSHIFT};
namespace {
std::vector<std::string> op_names = { "+",   "-", "*", "/", "&", "|", "^",   "<<",   ">>","ARSHIFT"};
}

class BinOp: public exp {
  public:
    BinOp(op_k o, expPtr l, expPtr r): op_(o), l_(l), r_(r) {};
    virtual ~BinOp() = default;
    virtual std::string toStr(std::string spacing) const {
        return twoChildToStr(spacing, "BinOp " + op_names.at(static_cast<int>(op_)), l_, r_);
    };
  private:
    op_k op_;
    expPtr l_;
    expPtr r_;
};

// evauates exp_ and returns the value at that loaction
class Mem: public exp {
  public:
    Mem(expPtr e): exp_(e){};
    virtual ~Mem() = default;
    virtual std::string toStr(std::string spacing) const {
        return oneChildToStr(spacing, "Mem", exp_);
    }
  private:
    expPtr exp_;
};

// evauates exp_ and returns the value at that loaction
class Call: public exp {
  public:
    Call(expPtr f, expPtrList l): func_(f), args_{l} {};
    virtual ~Call() = default;
    virtual std::string toStr(std::string spacing) const {
        std::string argsStr = spacing + ws + "args:\n";        
        for (auto arg : args_){
            argsStr += arg->toStr(spacing + ws + ws);
        }
        return oneChildToStr(spacing, "Call", func_) + argsStr;
    }
  private:
    expPtr func_;
    expPtrList args_;
};

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

// evaluate exp_ and discard the result
class Exp: public stm {
  public:
    Exp(expPtr e): exp_(e)
        {};
    virtual ~Exp() = default;
    virtual std::string toStr(std::string spacing) const {
        return oneChildToStr(spacing, "Exp: ", exp_);
    };
  private:
    expPtr exp_;
};

// jump to address exp_. labs_ contains all of the locations
// exp_ could be
class Jump: public stm {
  public:
    Jump(label_t label): lab_(label)
        {};
    virtual ~Jump() = default;
    virtual std::string toStr(std::string spacing) const {
        return spacing + "Jump: " + lab_.toString();
    }
  private:
    label_t lab_;
};
/*class Jump: public stm {
  public:
    Jump(expPtr e, labelList labs): exp_(e), labs_(labs)
        {};
    virtual ~Jump() = default;
    virtual std::string toStr(std::string spacing) const {
        std::string labsStr = spacing + ws + "labs:\n";        
        for (auto label : labs_){
            labsStr += spacing + ws + ws + label.toString();
        }
        return oneChildToStr(spacing, "Jump", exp_) + labsStr;
    }
  private:
    expPtr exp_;
    labelList labs_;
};*/

enum class rel_op_k: int                { EQ,  NE, LT, GT,  LE,  GE, ULT, ULE, UGT, UGE};
namespace {
std::vector<std::string> rel_op_names = {"=","!=","<",">","<=",">=", 
                                         "unsigned <", "unsigned <=", "unsigned >", "unsigned >="};
}
// evaluate left_ and right_ and compare with op_. If true, go to trueLab_
// if false jump to falseLab_
class CJump: public stm {
  public:
    CJump(rel_op_k o, expPtr e1, expPtr e2, label_t t, label_t f): 
        op_(o), left_(e1), right_(e2), trueLab_(t), falseLab_(f) {};
    virtual ~CJump() = default;
    virtual std::string toStr(std::string spacing) const {
        
        return spacing + "CJump op: " + rel_op_names.at(static_cast<int>(op_)) + "\n" +
            spacing + "CJump left:\n" + left_->toStr(spacing + ws) +
            spacing + "CJump right:\n" + right_->toStr(spacing + ws) +
            spacing + "CJump true label: " + trueLab_.toString() +
            spacing + "CJump false label: " + falseLab_.toString();
    };
  private:
    rel_op_k op_;
    expPtr left_;
    expPtr right_;
    label_t trueLab_;
    label_t falseLab_;
};

// evaluates stm1_ then stm2_
class Seq: public stm {
  public:
    Seq(stmPtr s1, stmPtr s2): stm1_(s1), stm2_(s2)
        {};
    virtual ~Seq() = default;
    virtual std::string toStr(std::string spacing) const {
        return twoChildToStr(spacing, "Seq", stm1_, stm2_);
    };
  private:
    stmPtr stm1_;
    stmPtr stm2_;
};

// define name_ to be the current machine code address
class Label: public stm {
  public:
    Label(label_t n): name_(n)
        {};
    virtual ~Label() = default;
    virtual std::string toStr(std::string spacing) const {
        return spacing + "Label: " + name_.toString();
    };
  private:
    label_t name_;
};


} // ir namespace

