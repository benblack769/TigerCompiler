#pragma once
#include <unordered_map>
#include <stdexcept>
using namespace std;

enum class BaseType { INT, VOID, STRING, NIL, ARRAY, RECORD };

using type_id = int;
constexpr type_id null_id = -1;

struct TypeExpr{
    BaseType type;
    //arrays store name of type
    type_id array_type;
    //maps from name feild to type field (entry in type table)
    unordered_map<string,type_id> record_type;
    TypeExpr(BaseType b_type){
        assert(b_type == BaseType::INT ||
               b_type == BaseType::STRING ||
               b_type == BaseType::VOID ||
               b_type == BaseType::NIL);
        type = b_type;
    }
    TypeExpr(string array_ty_name){
        type = BaseType::ARRAY;
        array_type_name = array_ty_name;
    }
    TypeExpr(vector<pair<string,typeid_ty>> record_entries){
        record_type.assign(record_entries.begin(), record_entries.end());
        type = BaseType::RECORD;
    }
};
enum class UnTypes { NAME, ARRAY, RECORD };
struct UnresolvedType{
    UnTypes type;
    //arrays store name of type
    string array_type;
    string name_type;
    //maps from name feild to type field (entry in type table)
    vector<pair<string,string>> record_type;
};
UnresolvedType array_body(string arr_name){
    UnresolvedType res;
    res.type = UnTypes::ARRAY;
    res.array_type = arr_name;
    return res;
}
UnresolvedType name_body(string name_name){
    UnresolvedType res;
    res.type = UnTypes::NAME;
    res.name_type = arr_name;
    return res;
}
UnresolvedType record_body(vector<pair<string,string>> record_labels){
    UnresolvedType res;
    res.type = UnTypes::RECORD;
    res.record_type = record_labels;
    return res;
}
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
inline TypeExpr array_type(string base_type_name){
    return TypeExpr(base_type_name);
}
inline TypeExpr record_type(vector<pair<string,int>> record_entries){
    return TypeExpr(record_entries);
}
inline TypeExpr resolve_type(UnresolvedType un){
    assert(un.type == UnTypes::ARRAY || un.type == UnTypes::RECORD);
    unordered_map<string,type_id> rec;
    for(auto str_pair  : un.record_type){
        string name = str_pair.first;
        if(rec.count(name)){
            throw runtime_error("record labels must be unique");
        }
        rec[name] = null_id;
    }
    return TypeExpr{un.type,null_id,rec};
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
                types.overwrite_with_value(name, resolve_type(un_ty));
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
