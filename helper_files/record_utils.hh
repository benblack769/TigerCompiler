#pragma once
//Helper functions to work
#include <vector>
#include <unordered_set>

template<typename val_ty>
inline std::vector<std::string> first_of_pairs(const std::vector<std::pair<std::string, val_ty>> & vals){
    using namespace std;
    std::vector<std::string> res;
    for(auto str_pair : vals){
        res.push_back(str_pair.first);
    }
    return res;
}
template<typename val_ty>
inline std::vector<std::string> second_of_pairs(const std::vector<std::pair<val_ty,std::string>> & vals){
    using namespace std;
    std::vector<std::string> res;
    for(auto str_pair : vals){
        res.push_back(str_pair.second);
    }
    return res;
}
inline std::unordered_set<std::string> to_set(std::vector<std::string> strs){
    return std::unordered_set<std::string>(strs.begin(), strs.end());
}

template<typename val_ty>
bool first_strs_unique(const std::vector<std::pair<std::string, val_ty>> & vals){
    using namespace std;
    unordered_set<string> keys;
    for(auto str_pair : vals){
        keys.insert(str_pair.first);
    }
    return keys.size() == vals.size();
}
