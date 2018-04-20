#include "irt.hh"
#include <string>

using namespace std;

using namespace ir;

const string gp_register_1 = "$t0";
const string gp_register_2 = "$t1";
const string stack_pointer = "$sp";
const int word_size = 4;
const int word_size_str = to_string(word_size);

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

std::string Const::munch(){return "";}
std::string Name::munch(){return "";}
std::string Temp::munch(){return "";}
std::string BinOp::munch(){return "";}
std::string Mem::munch(){return "";}
std::string Call::munch(){return "";}
std::string Eseq::munch(){return "";}
std::string Move::munch(){return "";}
std::string Exp::munch(){return "";}
std::string Jump::munch(){return "";}
string pop_into(string reg){
    string line_1 = format_instruction("sw", reg, indirect_acc(stack_pointer));
    string line_2 = format_instruction("addiu",stack_pointer,stack_pointer,"-"+word_size_str);
    return line_1 + line_2;
}
string push_onto(string reg){
    string line_1 = format_instruction("addiu",stack_pointer,stack_pointer,word_size_str);
    string line_2 = format_instruction("sw", reg, indirect_acc(stack_pointer));
    return line_1 + line_2;
}
string instr_name(rel_op_k op){
    switch(op){
        case rel_op_k::EQ: "beq";
        case rel_op_k::LT: "blt";
        case rel_op_k::LE: "ble";
        case rel_op_k::GT: "bgt";
        case rel_op_k::GE: "bge";
        case rel_op_k::NE: "bne";
    }
}
std::string CJump::munch(){
    return left_->munch() +
        right_->munch() +
        pop_into(gp_register_2) +
        pop_into(gp_register_1) +
        format_instruction(instr_name(this->op_),gp_register_1,gp_register_2,trueLab_) +
        format_instruction("b",falseLab_);
}
std::string Seq::munch(){
    return stm1_.munch() +
           stm2.munch();
}
std::string Label::munch(){
    return name_+":\n";
}
