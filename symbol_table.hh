#pragma once
#include <unordered_map>
using namespace std;

enum class BaseType { INT, VOID, STRING, ARRAY, RECORD };

struct TypeExpr{
    BaseType type;
    //arrays store name of type
    int array_type;
    //maps from name feild to type field (entry in type table)
    unordered_map<string,int> record_type;
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
    int typenum = 0;
    unordered_map<int, TypeExpr> typevals;
    unordered_map<string, int> types;
    TypeTable(){
        add_type("string", TypeExpr(BaseType::STRING));
        add_type("int", TypeExpr(BaseType::INT));
    }
    void add_type(string tyid, const TypeExpr & ty_expr){
        types[tyid] = typenum;
        typevals[typenum] = ty_expr;
        typenum += 1;
    }
    const int lookup(string tyid){
        return types[tyid];
    }
};
