#pragma once
#include "ast_interface.hh"

namespace tiger{

namespace types{

class BasicType : public TypeNode {
 public:
    BasicType(TypeIDNode * in_type)
        :type(in_type){}

    virtual ~BasicType(){
        delete type;
    }
    virtual void print(std::ostream & os) const override{
        os << *type;
    }
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
    virtual void print(std::ostream & os) const override{
        os << " array of " << *type;
    }
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
    virtual void print(std::ostream & os) const override{
        os << "{" << *type_fields << "}";
    }
private:
    TypeFeildsNode * type_fields;
};
}
}
