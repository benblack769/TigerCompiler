#include <cassert>
#include <iostream>
#include "symbol_table.hh"
#include "semantic_check.hh"

SymbolTable::SymbolTable(){
    types.add_base_type("string", BaseType::STRING);
    types.add_base_type("int", BaseType::INT);
}
void SymbolTable::add_type_set(vector<pair<string, UnresolvedType>> multu_rec_types){
    /*
    PARAMS: multu_rec_types are the body of type variables
    GOAL: maps mutually recursive type names corretly, making sure that they reference the right vairable, and also
    */
    //checks that names are unique in multiply recursive enviornment declearations
    if(!first_strs_unique(multu_rec_types)){
        throw SemanticException(SematicError::TWO_NAMES_IN_MUTU_RECURSIVE_ENV);
    }
    //removes types of same name from table
    for(string name : first_of_pairs(multu_rec_types)){
        if(types.has_type(name)){
            types.remove_type(name);
        }
    }
    //starts resolution by putting in partially resolved records and array into global map
    unordered_map<string, string> unresolved_names;
    unordered_map<string, typeid_ty> part_resolved_ty;

    for(auto ty_info : multu_rec_types){
        UnresolvedType un_ty = ty_info.second;
        string name = ty_info.first;
        if(un_ty.type == UnTypes::NAME){
            unresolved_names[name] = un_ty.name_type;
        }
        else{
            if(un_ty.type == UnTypes::ARRAY){
                part_resolved_ty[name] = types.make_new_array(name);
            }
            else if(un_ty.type == UnTypes::RECORD){
                part_resolved_ty[name] = types.make_new_record(name);
            }
        }
    }
    //resolves name    references
    //n^2 algorithm guarenttes resolution unless there is a cycle
    for(size_t i = 0; i < unresolved_names.size()+1; i++){
        bool no_key = false;
        for(auto name_pair : unresolved_names){
            if(types.has_type(name_pair.second)){
                types.link_type(name_pair.first,name_pair.second);
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
        string name = ty_info.first;
        UnresolvedType unresolved = ty_info.second;
        if(unresolved.type == UnTypes::ARRAY){
            typeid_ty arr_table_loc = part_resolved_ty.at(name);
            types.set_array(arr_table_loc,get_checked_type(unresolved.array_type));
        }
        else if(unresolved.type == UnTypes::RECORD){
            typeid_ty rec_table_loc = part_resolved_ty.at(name);
            vector<pair<string, TypeExpr>> resolved_items;
            for(auto str_pair : unresolved.record_type){
                resolved_items.push_back(make_pair(str_pair.first, get_checked_type(str_pair.second)));
            }
            types.set_record(rec_table_loc,resolved_items);
        }
    }
}

void SymbolTable::add_function_set(vector<pair<string, FuncHeader>> multu_rec_funcs){
    if(!first_strs_unique(multu_rec_funcs)){
        throw SemanticException(SematicError::TWO_NAMES_IN_MUTU_RECURSIVE_ENV);
    }
    //resolves function argument/return types, and adds them to the variable table
    for(auto func_header_pair : multu_rec_funcs){
        string fname = func_header_pair.first;
        FuncHeader typeinfo = func_header_pair.second;
        //check function type info
        FuncEntry func_res;
        func_res.ret_type =  typeinfo.has_return_type ? get_checked_type(typeinfo.return_type_name) : void_type();
        for(string ty_names : typeinfo.arg_type_names){
            func_res.arg_types.push_back(get_checked_type(ty_names));
        }
        vars[fname] = VarFuncItem(func_res);
    }
}
void SymbolTable::add_variable(string name, TypeExpr type){
    vars[name] = VarFuncItem(VarEntry{type});
}
