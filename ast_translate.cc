#include <vector>
#include "ast_interface.hh"
#include "ast_specifics/expr_ast.hh"
#include "ast_specifics/lval_ast.hh"
#include "ast_specifics/decs_ast.hh"
#include "ast_specifics/types_ast.hh"

namespace tiger{

// Will be changed
class IR_ESEQ: public IR_TREE_CLASS_NAME {
  public:
    IR_ESEQ(IR_TREE_CLASS_NAME*, IR_TREE_CLASS_NAME*){};
};
class IR_MOVE: public IR_TREE_CLASS_NAME {
  public:
    IR_MOVE(IR_TREE_CLASS_NAME*, IR_TREE_CLASS_NAME*){};
};
class IR_MEM: public IR_TREE_CLASS_NAME {
  public:
    IR_MEM(IR_TREE_CLASS_NAME*){};
};
class IR_CONST: public IR_TREE_CLASS_NAME {
  public:
    IR_CONST(int){};
};

// will probably get this from another file in the future
int wordSize = 4;
using word_t = int; //may need to change this for larger words


// Build up a tree of ESEQ and MOVE nodes to
// move all of the characters to the correct places
IRTptr exprs::StringNode::translate() const{
    // this will hold the location of the start of the string
    // probably will be set to the stack pointer in the future
    word_t strStart = 0;
    // this is the variable that will be used to count up each word in the string
    word_t strP = strStart;

    // this will store each of our memory calls before we make it into a tree
    std::vector<shared_ptr<IR_MOVE>> strExps = {};
    
    // the first word in the string will be it's length
    word_t length = mystring.length();
    auto lNode = std::make_shared<IR_MOVE>(new IR_MEM(new IR_CONST(strP)), new IR_CONST(length));
    strExps.push_back(lNode);
    strP++;
    
    // we can fit four characters in a word, so currWord will count how we're doing
    // we use shifting to fit them all 
    word_t currWord = 0;
    int charInWord = 0;
    for (int i = 0; i < length; i++) {
        currWord += static_cast<word_t>(mystring[i]);
        charInWord++;
        // once a word is filled with chars, we start a new word
        if (charInWord >= wordSize){
            auto cNode = std::make_shared<IR_MOVE>(new IR_MEM(new IR_CONST(strP)), new IR_CONST(currWord));
            strExps.push_back(cNode);
            strP++;
            currWord = 0; 
            charInWord = 0;
        }
        currWord << 8; //shift word one byte
    }
    // finish up our last word
    if (currWord != 0) {
        auto cNode = std::make_shared<IR_MOVE>(new IR_MEM(new IR_CONST(strP)), new IR_CONST(currWord));
        strExps.push_back(cNode);
    }
    // build up a tree of ESEQs where the whole tree will return the location of the beginning
    // of the string. 
    // in other words, the terminal right leaf of the tree will contain the location
    IR_TREE_CLASS_NAME* rNode = new IR_CONST(strStart);
    for (int i = strExps.size(); i >= 0; i--){
        rNode = new IR_ESEQ(strExps[i].get(), rNode);
    }
    
    return std::shared_ptr<IR_TREE_CLASS_NAME>(rNode);
}

IRTptr exprs::NilNode::translate() const{return nullptr;}
IRTptr exprs::IntNode::translate() const{return nullptr;}
IRTptr exprs::LvalNode::translate() const{return nullptr;}
IRTptr exprs::NegateNode::translate() const{return nullptr;}
IRTptr exprs::BinaryNode::translate() const{return nullptr;}
IRTptr exprs::AssignNode::translate() const{return nullptr;}
IRTptr exprs::FunctionCall::translate() const{return nullptr;}
IRTptr exprs::ExprSequenceEval::translate() const{return nullptr;}
IRTptr exprs::IfThen::translate() const{return nullptr;}
IRTptr exprs::IfThenElse::translate() const{return nullptr;}
IRTptr exprs::WhileDo::translate() const{return nullptr;}
IRTptr exprs::ForToDo::translate() const{return nullptr;}
IRTptr exprs::Break::translate() const{return nullptr;}
IRTptr exprs::ArrCreate::translate() const{return nullptr;}
IRTptr exprs::RecCreate::translate() const{return nullptr;}
IRTptr exprs::LetIn::translate() const{return nullptr;}

IRTptr lvals::IdLval::translate() const{return nullptr;}
IRTptr lvals::AttrAccess::translate() const{return nullptr;}
IRTptr lvals::BracketAccess::translate() const{return nullptr;}

IRTptr decls::VarDecl::translate() const{return nullptr;}
IRTptr decls::FuncDecl::translate() const{return nullptr;}
IRTptr decls::TypeDecl::translate() const{return nullptr;}

IRTptr types::BasicType::translate() const{return nullptr;}
IRTptr types::ArrayType::translate() const{return nullptr;}
IRTptr types::TypeFeildType::translate() const{return nullptr;}

IRTptr ExprListNode::translate() const{return nullptr;}
IRTptr ExprSequenceNode::translate() const{return nullptr;}
IRTptr FieldNode::translate() const{return nullptr;}
IRTptr TypeIDNode::translate() const{return nullptr;}
IRTptr TypeFeildNode::translate() const{return nullptr;}
IRTptr FieldListNode::translate() const{return nullptr;}
IRTptr DeclarationListNode::translate() const{return nullptr;}
IRTptr TypeFeildsNode::translate() const{return nullptr;}

} //namespace
