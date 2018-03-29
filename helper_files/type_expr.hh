#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include "helper_files/record_utils.hh"
using namespace std;
enum class BaseType { INT, VOID, STRING, NIL, ARRAY, RECORD };

using typeid_ty = int;
constexpr typeid_ty null_id = -1;

struct TypeExpr{
    BaseType type;
    //arrays store name of type
    typeid_ty array_type;
    //maps from name feild to type field (entry in type table)
    unordered_map<string,typeid_ty> record_type;
    TypeExpr(BaseType b_type){
        assert(b_type == BaseType::INT ||
               b_type == BaseType::STRING ||
               b_type == BaseType::VOID ||
               b_type == BaseType::NIL);
        type = b_type;
    }
    TypeExpr(typeid_ty array_ty_id){
        type = BaseType::ARRAY;
        array_type = array_ty_id;
    }
    TypeExpr(vector<pair<string,typeid_ty>> record_entries):
            record_type(record_entries.begin(), record_entries.end()){
        assert(first_strs_unique(record_entries));
        type = BaseType::RECORD;
    }
};
inline TypeExpr string_type(){
    return TypeExpr(BaseType::STRING);
}
inline TypeExpr int_type(){
    return TypeExpr(BaseType::INT);
}
inline TypeExpr nil_type(){
    return TypeExpr(BaseType::NIL);
}
inline TypeExpr void_type(){
    return TypeExpr(BaseType::VOID);
}
inline TypeExpr array_type(typeid_ty base_type_name){
    return TypeExpr(base_type_name);
}
inline TypeExpr record_type(vector<pair<string,typeid_ty>> record_entries){
    return TypeExpr(record_entries);
}
