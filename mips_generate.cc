#include "generate.hh"
#include "frame.hh"

using namespace ir;

extern std::vector<StringFrag> str_fragments;

// this makes the header that declares all of the constant strings
std::string generateStringHeader(std::vector<StringFrag> strTable){
    std::string header = "";
    for (auto frag : strTable){
        header += frag.str_label.toString() + ": .asciiz " + frag.str_const + "\n";
    }
    return header;
}

std::string generate(IRTNode::IRTptr root){
    return ".data\n" + generateStringHeader(str_fragments);
}

