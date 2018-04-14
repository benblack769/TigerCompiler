#pragma once
#include <vector>
#include <iostream>
#include "helper_files/type_expr.hh"
#include "helper_files/unresolved_type_info.hh"
#include "helper_files/func_header.hh"
#include "semantic_check.hh"
#include "frame.hh"
using namespace std;

class SymbolTable;

class TypeTable{
protected:
    vector<TypeExpr> array_values;
    vector<unordered_map<string,TypeExpr>> record_values;
    unordered_map<string, TypeExpr> types;
public:
    friend class SymbolTable;
    void add_base_type(string name, BaseType type){
        types[name] = TypeExpr(type);
    }
    void link_type(string new_name, string old_name){
        types[new_name] = types.at(old_name);
    }
    void remove_type(string name){
        types.erase(name);
    }
    TypeExpr get_type(string name){
        return types.at(name);
    }
    bool has_type(string name){
        return types.count(name);
    }
    TypeExpr get_array_base_type(string array_name){
        TypeExpr arr_val = get_type(array_name);
        assert(arr_val.type == BaseType::ARRAY);
        return array_values.at(arr_val.value_id);
    }
    const unordered_map<string,TypeExpr> & get_record_entries(string record_name){
        TypeExpr rec_val = get_type(record_name);
        assert(rec_val.type == BaseType::RECORD);
        return record_values.at(rec_val.value_id);
    }
protected:
    typeid_ty make_new_record(string name){
        typeid_ty my_id = record_values.size();
        record_values.emplace_back();
        types[name] = record_type(my_id);
        return my_id;
    }
    void set_record(typeid_ty label, vector<pair<string, TypeExpr>> val){
        assert(first_strs_unique(val));
        record_values.at(label) = unordered_map<string,TypeExpr>(val.begin(), val.end());
    }
    typeid_ty make_new_array(string name){
        typeid_ty my_id = array_values.size();
        array_values.emplace_back();
        types[name] = array_type(my_id);
        return my_id;
    }
    void set_array(typeid_ty label, TypeExpr val){
        array_values.at(label) = val;
    }
};

struct VarEntry{
    TypeExpr type;
    Access access;
    int level;
};
struct FuncEntry{
    TypeExpr ret_type;
    vector<TypeExpr> arg_types;
    int level;
};
enum class VarOrFunc{BAD,VAR,FUNC};
class VarFuncItem{
public:
    VarFuncItem(){
        _type = VarOrFunc::BAD;
    }
    VarFuncItem(VarEntry in_var){
        _type = VarOrFunc::VAR;
        var_data = in_var;
    }
    VarFuncItem(FuncEntry in_func){
        _type = VarOrFunc::FUNC;
        func_data = in_func;
    }
    VarOrFunc type()const{
        return _type;
    }
    const FuncEntry & func()const{
        assert(_type == VarOrFunc::FUNC);
        return func_data;
    }
    const  VarEntry & var()const{
        assert(_type == VarOrFunc::VAR);
        return var_data;
    }
protected:
    VarOrFunc _type;
    FuncEntry func_data;
    VarEntry var_data;
};
class SymbolTable{
protected:
public:
    SymbolTable();
    bool has_type(string tyid){
        return types.has_type(tyid);
    }
    bool has_var_symbol(string var_id)const{
        return vars.count(var_id);
    }
    bool symbol_is_var(string var_id)const{
        return has_var_symbol(var_id) && vars.at(var_id).type() == VarOrFunc::VAR;
    }
    bool symbol_is_func(string var_id)const{
        return has_var_symbol(var_id) && vars.at(var_id).type() == VarOrFunc::FUNC;
    }
    void add_type_set(vector<pair<string, UnresolvedType>> multu_rec_types);
    void add_function_set(vector<pair<string, FuncHeader>> multu_rec_funcs, int func_depth_level);
    void add_variable(string name, TypeExpr type, int func_depth_level, Access frame_access);
    bool verify_function_args(string func_name, vector<TypeExpr> arg_types){
        assert(symbol_is_func(func_name));
        const vector<TypeExpr> & expected_types = vars[func_name].func().arg_types;
        return expected_types.size() == arg_types.size() &&
                std::equal(arg_types.begin(), arg_types.end(), expected_types.begin(), [](TypeExpr o, TypeExpr t){return o.is_convertible(t);});
    }
    TypeExpr array_subtype(TypeExpr arr_expr){
        assert(arr_expr.type == BaseType::ARRAY);
        return types.array_values.at(arr_expr.value_id);
    }
    bool record_subexpr_exists(TypeExpr record_expr, string rec_name){
        auto & act_types = types.record_values.at(record_expr.value_id);
        return act_types.count(rec_name);
    }
    TypeExpr get_record_subexpr(TypeExpr record_expr, string rec_name){
        auto & act_types = types.record_values.at(record_expr.value_id);
        return act_types.at(rec_name);
    }
    /*bool verify_record_args(TypeExpr record_expr, vector<pair<string,TypeExpr>> record_args){
        assert(record_expr.type != BaseType::RECORD);
        assert(first_strs_unique(record_args));
        auto & act_types = types.record_values.at(record_expr.value_id);
        for(auto str_pair : record_args){
            assert(act_types.count(str_pair.first));
            if(act_types.at(str_pair.first).is_convertible(str_pair.second)){
                return false;
            }
        }
        return true;
    }*/
    TypeExpr function_ret_type(string func_name){
        assert(symbol_is_func(func_name));
        return vars.at(func_name).func().ret_type;
    }
    TypeExpr var_type(string varid){
        assert(symbol_is_var(varid));
        return vars.at(varid).var().type;
    }
    VarEntry var_data(string varid)const{
        assert(symbol_is_var(varid));
        return vars.at(varid).var();
    }
    TypeExpr get_checked_type(string tyid){
        if(!has_type(tyid)){
            throw SemanticException(SematicError::TYPE_NOT_DEFINED);
        }
        return types.get_type(tyid);
    }
protected:
    TypeTable types;
    unordered_map<string, VarFuncItem> vars;
};
