#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <cassert>
#include "helper_files/record_utils.hh"
using namespace std;
enum class BaseType { UNRESOLVED, INT, VOID, STRING, NIL, ARRAY, RECORD };

using typeid_ty = size_t;
constexpr typeid_ty null_id = -1;

struct TypeExpr{
    BaseType type;
    //arrays store name of type
    typeid_ty value_id;
    TypeExpr(){
        type = BaseType::UNRESOLVED;
        value_id = null_id;
    }
    TypeExpr(BaseType b_type){
        assert(b_type == BaseType::INT ||
               b_type == BaseType::STRING ||
               b_type == BaseType::VOID ||
               b_type == BaseType::NIL);
        type = b_type;
        value_id = null_id;
    }
    TypeExpr(BaseType b_type, typeid_ty ty_id){
        assert(b_type == BaseType::ARRAY ||
               b_type == BaseType::RECORD);
        type = b_type;
        value_id = ty_id;
    }
    bool not_void(){
        assert_unresolved();
        return type != BaseType::VOID;
    }
    bool is_convertible(TypeExpr other){
        assert_unresolved();
        other.assert_unresolved();
        return (type == other.type && value_id == other.value_id) ||
                (type == BaseType::NIL && other.type == BaseType::RECORD) ||
                (type == BaseType::RECORD && other.type == BaseType::NIL);
    }
    void assert_unresolved(){
        assert(type != BaseType::UNRESOLVED);
    }
    /*bool operator ==(TypeExpr other)const{
        return (type == other.type
                    && value_id == other.value_id)
                || (type != BaseType::VOID
                    && other.type != BaseType::VOID
                    && (type == BaseType::NIL
                        || other.type == BaseType::NIL);
    }*/
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
inline TypeExpr array_type(typeid_ty ty_id){
    return TypeExpr(BaseType::ARRAY,ty_id);
}
inline TypeExpr record_type(typeid_ty ty_id){
    return TypeExpr(BaseType::RECORD,ty_id);
}
