#pragma once
#include <ostream>
#include <vector>
#include <memory>
#include <string>
#include "ast_node.hh"

using namespace tiger;

//helpers for list nodes
template<class item_ty>
inline std::vector<ASTNode *> base_list(const std::vector<std::unique_ptr<item_ty>> & list){
    std::vector<ASTNode *> res;
    for(auto & val : list){
        res.push_back(val.get());
    }
    return res;
}

inline void print_list(std::ostream & os, const std::vector<ASTNode *> & item_list, std::string sep){
    for(size_t i = 0; i < item_list.size(); i++){
        os << *item_list[i];
        if(i != item_list.size()-1){
            os << sep;
        }
    }
}
