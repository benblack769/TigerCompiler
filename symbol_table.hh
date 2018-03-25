#pragma once
#include <unordered_map>
using namespace std;

enum class BaseType { INT, VOID, STRING, ARRAY, RECORD };

using typeid_ty = int;

struct TypeExpr{
    BaseType type;
    //arrays store name of type
    typeid_ty array_type;
    //maps from name feild to type field (entry in type table)
    unordered_map<string,typeid_ty> record_type;
    TypeExpr(BaseType b_type){
        assert(b_type == BaseType::INT ||
               b_type == BaseType::STRING ||
               b_type == BaseType::VOID);
        type = b_type;
    }
    TypeExpr(string array_ty_name){
        type = BaseType::ARRAY;
        array_type_name = array_ty_name;
    }
    TypeExpr(vector<pair<string,int>> record_entries){
        record_type.assign(record_entries.begin(), record_entries.end());
        type = BaseType::RECORD;
    }
    bool operator == (const TypeExpr & other){
        if(type != other.type){
            return false;
        }
        //TODO: nees to implement fully
    }
};
struct TypeTable{
    vector<typeid_ty, TypeExpr> typevals;
    unordered_map<string, typeid_ty> types;
    TypeTable(){
        add_type("string", TypeExpr(BaseType::STRING));
        add_type("int", TypeExpr(BaseType::INT));
    }
    typeid_ty add_type(string tyid, const TypeExpr & ty_expr){
        typeid_ty cur_type = typevals.size();
        typevals.push_back(ty_expr);
        types[tyid] = cur_type;
        return cur_type;
    }
    typeid_ty lookup(string tyid){
        return types[tyid];
    }
    const TypeExpr & get(typeid_ty tyid){
        return typevals[tyid];
    }
};
