#pragma once
#include <vector>
#include "helper_files/type_expr.hh"
#include "helper_files/many_to_one.hh"
#include "helper_files/unresolved_type_info.hh"
using namespace std;

struct TypeTable{
    ManyToOne<string, TypeExpr> types;
    TypeTable();
    void add_type(string tyid, const TypeExpr & ty_expr);
    bool has_type(string tyid){
        return types.has_key(tyid);
    }
    void add_type_set(vector<pair<string, UnresolvedType>> multu_rec_types);
    void final_resolution(TypeExpr & ty_expr, UnresolvedType unresolved);
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
