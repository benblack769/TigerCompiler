#include <cassert>
#include <iostream>
#include "symbol_table.hh"
#include "semantic_check.hh"

TypeExpr partially_resolve_type(UnresolvedType un){
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
TypeTable::TypeTable(){
    types.overwrite_with_value("string", string_type());
    types.overwrite_with_value("int", int_type());
}
void TypeTable::add_type(string tyid, const TypeExpr & ty_expr){
    types.overwrite_with_value(tyid, ty_expr);
}
void TypeTable::add_type_set(vector<pair<string, UnresolvedType>> multu_rec_types){
    /*
    PARAMS: multu_rec_types are the body of type variables
    GOAL: maps mutually recursive type names corretly, making sure that they reference the right vairable, and also
    */
    //checks that names are unique in multiply recursive enviornment declearations
    unordered_set<string> added_type_names;
    for(auto ty : multu_rec_types){
        string name = ty.first;
        if(added_type_names.count(name)){
            throw SemanticException(SematicError::TWO_NAMES_IN_MUTU_RECURSIVE_ENV);
        }
        added_type_names.insert(name);
    }
    //starts resolution by putting in partially resolved records and array into global map
    unordered_map<string, string> unresolved_names;
    for(auto ty_info : multu_rec_types){
        UnresolvedType un_ty = ty_info.second;
        string name = ty_info.first;
        if(un_ty.type == UnTypes::NAME){
            unresolved_names[name] = un_ty.name_type;
        }
        else{
            types.overwrite_with_value(name, partially_resolve_type(un_ty));
        }
    }
    //resolves name    references
    //n^2 algorithm guarenttes resolution unless there is a cycle
    for(size_t i = 0; i < unresolved_names.size()+1; i++){
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
            throw SemanticException(SematicError::CYCLICLY_DEFINED_TYPES);
        }
    }
    //finishes resolution of records and arrays by putting in correct type information
    for(auto ty_info : multu_rec_types){
        UnresolvedType un_ty = ty_info.second;
        string name = ty_info.first;
        final_resolution(types.get(name), un_ty);
    }
}

void TypeTable::final_resolution(TypeExpr & ty_expr, UnresolvedType unresolved){
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
