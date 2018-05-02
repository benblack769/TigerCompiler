#include "irt.hh"
#include <string>

using namespace std;

using namespace ir;

const string gp_register_1 = "$t0";
const string gp_register_2 = "$t1";
const string stack_pointer = "$sp";
const string frame_pointer = "$fp";
const int word_size = 4;
const string word_size_str = to_string(word_size);

string format_instruction(string instr, string arg1="", string arg2="", string arg3=""){
    string res = instr + "\t\t";
    if(arg1.size()){
        res += arg1;
    }
    if(arg2.size()){
        res += ","+arg2;
    }
    if(arg3.size()){
        res += ","+arg3;
    }
    res += "\n";
    return res;
}
string indirect_acc(string ptr_reg, int offset=0){
    return to_string(offset) + "(" + ptr_reg + ")";
}
string pop_into(string reg){
    string line_1 = format_instruction("lw", reg, indirect_acc(stack_pointer));
    string line_2 = format_instruction("addiu",stack_pointer,stack_pointer,"-"+word_size_str);
    return line_1 + line_2;
}
string push_onto(string reg){
    string line_1 = format_instruction("addiu",stack_pointer,stack_pointer,word_size_str);
    string line_2 = format_instruction("sw", reg, indirect_acc(stack_pointer));
    return line_1 + line_2;
}

// push the value of the const on the stack
std::string Const::munch(){
    string line_1 = format_instruction("li", gp_register_1, to_string(val_));
    string line_2 = push_onto(gp_register_1);
    return line_1 + line_2;
}
// in ast_translate, Name is only used for strings.
// This function pushes the memory address of the string the Name is 
// associated with onto the stack
std::string Name::munch(){
    string line_1 = format_instruction("la", gp_register_1, val_.toString());
    string line_2 = push_onto(gp_register_1);
    return line_1 + line_2;
}
std::string Temp::munch(){return "";}
// push fp onto the stack
std::string Fp::munch(){
    return push_onto(gp_register_1);
}
string binop_instr_name(op_k op){
    switch(op){
    case op_k::PLUS:    return "add";
    case op_k::MINUS:   return "sub";
    case op_k::MUL:     return "mult";
    case op_k::DIV:     return "div";
    case op_k::AND:     return "and";
    case op_k::OR:      return "or";
    case op_k::XOR:     return "xor";
    case op_k::LSHIFT:  return "sllv";
    case op_k::RSHIFT:  return "srlv";
    case op_k::ARSHIFT: return "srav";
    default:
        assert(false && "bad case");
        return "";
    }
}
// put the values of left and right on the stack
// then pop them off and apply the operation
// then push the result to the stack
std::string BinOp::munch(){
    return  l_->munch() + r_->munch() +
    pop_into(gp_register_1) + pop_into(gp_register_2) +
    format_instruction(binop_instr_name(op_), gp_register_1, gp_register_1, gp_register_2) +
    push_onto(gp_register_1);
}
// evaluate exp_, get the value at exp_'s address, push that value on the stack
std::string Mem::munch(){
    return exp_->munch() + 
    pop_into(gp_register_1) + 
    // load the value at the memory address stored in gp_register_1 into gp_register_1
    format_instruction("lw", gp_register_1, indirect_acc(gp_register_1)) +
    push_onto(gp_register_1);
}
std::string Call::munch(){return "";}

// evaluates stm_ then exp_
// since the last value on the stack will be from exp_,
// then this node will "return" the value of exp_
// without manipulating the stack
std::string Eseq::munch(){
   return stm_->munch() + exp_->munch(); 
}

// put value from src_ into dest_
// dest_ must be a Temp or a Mem
std::string Move::munch(){
    auto srcStr = src_->munch();
    if(auto memP = dynamic_pointer_cast<Mem>(dest_)){
        // store the value of src_ in the memory location refered to
        // by memP
        return srcStr + memP->getExp()->munch() +
        pop_into(gp_register_1) + pop_into(gp_register_2) +
        format_instruction("sw", gp_register_2, indirect_acc(gp_register_1));
    } else if(auto tempP = dynamic_pointer_cast<Temp>(dest_)){
        //TODO
        return "";
    } else {
        assert(false && "dest of Move ir node is not Temp or Mem");
    }
}
std::string Exp::munch(){
    return exp_->munch() +
           pop_into(gp_register_1);
    // it doesn't matter that the result gets store in gp_register_1,
    // we just want to get rid of the result of exp_, so we pop it off the stack
}
std::string Jump::munch(){
    return lab_.toString();
}
string instr_name(rel_op_k op){
    switch(op){
    case rel_op_k::EQ: return "beq";
    case rel_op_k::LT: return "blt";
    case rel_op_k::LE: return "ble";
    case rel_op_k::GT: return "bgt";
    case rel_op_k::GE: return "bge";
    case rel_op_k::NE: return "bne";
    default:
        assert(false && "bad case");
        return "";
    }
}
std::string CJump::munch(){
    return left_->munch() +
        right_->munch() +
        pop_into(gp_register_2) +
        pop_into(gp_register_1) +
        format_instruction(instr_name(this->op_),gp_register_1,gp_register_2,trueLab_.toString()) +
        format_instruction("b",falseLab_.toString());
}
std::string Seq::munch(){
    return stm1_->munch() +
           stm2_->munch();
}
std::string Label::munch(){
    return name_.toString()+":\n";
}
