#pragma once
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include "helper_files/many_to_one.hh"
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
        if(record_entries.size() != record_type.size()){
            throw runtime_error("record labels must be unique");
        }
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
inline TypeExpr partially_resolve_type(UnresolvedType un){
    if(un.type == UnTypes::ARRAY){
        return array_type(null_id);
    }
    else if(un.type == UnTypes::RECORD){
        vector<pair<string,typeid_ty>> res;
        for(auto str_pair  : un.record_type){
            string name = str_pair.first;
            res.push_back(make_pair(name,null_id));
        }
        return record_type(res);
    }
    else{
        assert(false && "should only partially resolve records and arrays");
    }
}
struct TypeTable{
    ManyToOne<string, TypeExpr> types;
    TypeTable(){
        types.overwrite_with_value("string", string_type());
        types.overwrite_with_value("int", int_type());
    }
    void add_type(string tyid, const TypeExpr & ty_expr){
        types.overwrite_with_value(tyid, ty_expr);
    }
    bool has_type(string tyid){
        return types.has_key(tyid);
    }
    void add_type_set(vector<pair<string, UnresolvedType>> multu_rec_types){
        /*
        PARAMS: multu_rec_types are the body of type variables
        GOAL: maps mutually recursive type names corretly, making sure that they reference the right vairable, and also
        */
        //checks that names are unique in multiply recursive enviornment declearations
        unordered_set<string> added_type_names;
        for(auto ty : multu_rec_types){
            string name = ty.first;
            if(added_type_names.count(name)){
                throw runtime_error("two type names cannot be declared in the same mutual recurive block");
            }
            added_type_names.insert(name);
        }
        //starts resolution by putting in partially resolved records and array into global map
        unordered_map<string, string> unresolved_names;
        for(auto ty_info : multu_rec_types){
            UnresolvedType un_ty = ty_info.second;
            string name = ty_info.first;
            if(un_ty.type == UnTypes::NAME){
                unresolved_names[un_ty.name_type] = name;
            }
            else{
                types.overwrite_with_value(name, partially_resolve_type(un_ty));
            }
        }
        //resolves name references
        //n^2 algorithm guarenttes resolution unless there is a cycle
        for(int i = 0; i < unresolved_names.size()+1; i++){
            bool no_key = false;
            for(auto name_pair : unresolved_names){
                if(types.has_key(name_pair.second)){
                    types.overwrite_with_link(name_pair.first,name_pair.second);
                }
                else{
                    no_key = true;
                }
            }
            if(no_key && i == unresolved_names.size()){
                throw runtime_error("cycle detected in type declaration");
            }
        }
        //finishes resolution of records and arrays by putting in correct type information
        for(auto ty_info : multu_rec_types){
            UnresolvedType un_ty = ty_info.second;
            string name = ty_info.first;
            final_resolution(types.get(name), un_ty);
        }
    }
    void final_resolution(TypeExpr & ty_expr, UnresolvedType unresolved){
        //puts in correct type ID for the string identifiers in unresolved
        if(ty_expr.type == BaseType::ARRAY){
            ty_expr.array_type = types.key_id(unresolved.array_type);
        }
        else if(ty_expr.type == BaseType::RECORD){
            for(auto str_pair : unresolved.record_type){
                ty_expr.record_type[str_pair.first] = types.key_id(str_pair.second);
            }
        }
    }
};
/*
struct TypeEnvStack{
    vector<TypeTable> type_stack;
    void begin_env(){
        type_stack.push_back(type_stack.back());
    }
    void end_env(){
        type_stack.pop_back();
    }
    void add_type_header(vector<pair<string, UnresolvedType>> multu_rec_types){
        type_stack.back().types.add_type(multu_rec_types);
    }
    bool has_type(string tyid){
        return type_stack.back()..has_type(tyid);
    }
};*/
