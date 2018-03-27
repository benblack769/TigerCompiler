#pragma once
#include <string>
#include <vector>
#include <utility>

using namespace std;

enum class UnTypes { NAME, ARRAY, RECORD };
struct UnresolvedType{
    UnTypes type;
    //arrays store name of type
    string array_type;
    string name_type;
    //maps from name feild to type field (entry in type table)
    vector<pair<string,string>> record_type;
};
inline UnresolvedType array_body(string arr_name){
    UnresolvedType res;
    res.type = UnTypes::ARRAY;
    res.array_type = arr_name;
    return res;
}
inline UnresolvedType name_body(string name_name){
    UnresolvedType res;
    res.type = UnTypes::NAME;
    res.name_type = name_name;
    return res;
}
inline UnresolvedType record_body(vector<pair<string,string>> record_labels){
    UnresolvedType res;
    res.type = UnTypes::RECORD;
    res.record_type = record_labels;
    return res;
}
