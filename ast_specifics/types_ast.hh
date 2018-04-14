#pragma once
#include "ast_interface.hh"
#include <unordered_set>

namespace tiger{

namespace types{

class BasicType : public TypeNode {
 public:
    BasicType(TypeIDNode * in_type)
        :type(in_type){}

    virtual ~BasicType(){
        delete type;
    }
    virtual UnresolvedType unresolved_type(){
        return name_body(type->id_name());
    }
    virtual void print(std::ostream & os) const override{
        os << *type;
    }
    virtual IRTptr translate(const SymbolTable & env) const override;
private:
    TypeIDNode * type;
};
class ArrayType : public TypeNode {
 public:
    ArrayType(TypeIDNode * in_type)
        :type(in_type){}

    virtual ~ArrayType(){
        delete type;
    }
    virtual UnresolvedType unresolved_type(){
        return array_body(type->id_name());
    }
    virtual void print(std::ostream & os) const override{
        os << " array of " << *type;
    }
    virtual IRTptr translate(const SymbolTable & env) const override;
private:
    TypeIDNode * type;
};

class TypeFeildType : public TypeNode {
 public:
    TypeFeildType(TypeFeildsNode * _type_fields)
        :type_fields(_type_fields){}

    virtual ~TypeFeildType(){
        delete type_fields;
    }
    void check_double_keyed_names(){
        if(!first_strs_unique(type_fields->get_field_pairs())){
            throw SemanticException(SematicError::NON_UNIQUE_RECORD_LABELS);
        }
    }
    UnresolvedType unresolved_type(){
        check_double_keyed_names();
        return record_body(type_fields->get_field_pairs());
    }
    virtual void print(std::ostream & os) const override{
        os << "{" << *type_fields << "}";
    }
    virtual IRTptr translate(const SymbolTable & env) const override;
private:
    TypeFeildsNode * type_fields;
};
}
}
