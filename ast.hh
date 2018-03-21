#pragma once
#include <string>
#include <cstdlib>
#include <ostream>
#include <vector>
#include <memory>
#include <cassert>
#include <iostream>

/* interface to the lexer */
void yyerror(char *s, ...);

namespace tiger {

///////////////////////////////////////////////////////////////////////////////
// Base AST node class, to define the hierarchy.
class ASTNode {
 public:
  using ASTptr = const ASTNode*; // Can't use smart ptr in union :(

  ASTNode() = default;
  virtual ~ASTNode() = default;
  virtual void print(std::ostream & os) const = 0;
};

inline std::ostream & operator << (std::ostream & os, const ASTNode & node){
    node.print(os);
    return os;
}
inline std::ostream & operator << (std::ostream & os, const ASTNode * node){
    node->print(os);
    return os;
}

class ExprNode: public ASTNode {
 public:
  ExprNode() = default;
  virtual ~ExprNode() = default;
  virtual void print(std::ostream & os) const override = 0;
};

class LvalueNode: public ASTNode {
 public:
  LvalueNode() = default;
  virtual ~LvalueNode() = default;
  virtual void print(std::ostream & os) const override = 0;
};

class DeclarationNode: public ASTNode {
 public:
  DeclarationNode() = default;
  virtual ~DeclarationNode() = default;
  virtual void print(std::ostream & os) const override = 0;
};

class TypeNode: public ASTNode {
 public:
  TypeNode() = default;
  virtual ~TypeNode() = default;
  virtual void print(std::ostream & os) const override = 0;
};

//helpers for list nodes
template<class item_ty>
inline std::vector<ASTNode *> base_list(const std::vector<std::unique_ptr<item_ty>> & list){
    std::vector<ASTNode *> res;
    for(auto & val : list){
        res.push_back(val.get());
    }
    return res;
}

inline void print_list(std::ostream & os, const std::vector<ASTNode *> & item_list, std::string sep){
    for(size_t i = 0; i < item_list.size(); i++){
        os << *item_list[i];
        if(i != item_list.size()-1){
            os << sep;
        }
    }
}

class ExprListNode: public ASTNode {
 public:
  ExprListNode(){}
  virtual ~ExprListNode(){}
  virtual void append_to(ExprNode * expr){
      list.push_back(std::unique_ptr<ExprNode>(expr));
  }
  virtual void print(std::ostream & os) const override {
      print_list(os, base_list(list), ", ");
  }
protected:
  std::vector<std::unique_ptr<ExprNode>> list;
};

class ExprSequenceNode: public ASTNode {
 public:
  ExprSequenceNode(){}
  virtual ~ExprSequenceNode(){}
  virtual void append_to(ExprNode * expr){
      list.push_back(std::unique_ptr<ExprNode>(expr));
  }
  virtual bool empty(){
      return list.size() == 0;
  }
  virtual bool singleton(){
      return list.size() == 1;
  }
  virtual void print(std::ostream & os) const override {
      print_list(os, base_list(list), ";\n");
  }
  std::vector<std::unique_ptr<ExprNode>> list;
};

class FieldNode: public ASTNode{
 public:
  FieldNode(std::string in_id, ExprNode * in_expr):
    id(in_id),
    expr(in_expr){}
  virtual ~FieldNode(){
      delete expr;
  }
  virtual void print(std::ostream & os) const override{
      os << id << " = " << *expr;
  }
protected:
    std::string id;
    ExprNode * expr;
};

class TypeIDNode : public ASTNode {
 public:
  TypeIDNode(std::string in_id):
     my_id(in_id){}
  virtual ~TypeIDNode(){
  }
  virtual void print(std::ostream & os) const override{
      os << my_id;
  }
 private:
  std::string my_id;
};

class TypeFeildNode: public ASTNode {
 public:
  TypeFeildNode(std::string in_id, TypeIDNode * in_ty):
    id(in_id),
    ty(in_ty){}
  virtual ~TypeFeildNode(){
      delete ty;
  }
  virtual void print(std::ostream & os) const override{
      os << id << " : " << *ty;
  }
protected:
    std::string id;
    TypeIDNode * ty;
};

class FieldListNode: public ASTNode {
 public:
  FieldListNode(){}
  virtual ~FieldListNode(){}
  virtual void append_to(FieldNode * expr){
      list.push_back(std::unique_ptr<FieldNode>(expr));
  }
  virtual void print(std::ostream & os) const override {
      print_list(os, base_list(list), ",");
  }
  std::vector<std::unique_ptr<FieldNode>> list;
};


class DeclarationListNode: public ASTNode {
 public:
   DeclarationListNode(){}
   virtual ~DeclarationListNode(){}
   virtual void append_to(DeclarationNode * expr){
       list.push_back(std::unique_ptr<DeclarationNode>(expr));
   }
   virtual void print(std::ostream & os) const override {
       print_list(os, base_list(list), "\n");
   }
   std::vector<std::unique_ptr<DeclarationNode>> list;
};
class TypeFeildsNode: public ASTNode {
 public:
   TypeFeildsNode(){}
   virtual ~TypeFeildsNode(){}
   virtual void append_to(TypeFeildNode * expr){
       list.push_back(std::unique_ptr<TypeFeildNode>(expr));
   }
   virtual void print(std::ostream & os) const override {
       print_list(os, base_list(list), ", ");
   }
   std::vector<std::unique_ptr<TypeFeildNode>> list;
};


namespace exprs{

    class StringNode: public ExprNode{
    public:
        StringNode(std::string in_str):
            mystring(in_str){}

        virtual ~StringNode() = default;
        virtual void print(std::ostream & os) const override{
            os << mystring;
        }
    protected:
        std::string mystring;
    };

    class NilNode: public ExprNode{
    public:
        NilNode(){}
        virtual ~NilNode() = default;
        virtual void print(std::ostream & os) const override{
            os << "nil";
        }
    };

    class IntNode: public ExprNode{
    public:
        IntNode(int64_t in_int):
            my_int(in_int){}

        virtual ~IntNode() = default;
        virtual void print(std::ostream & os) const override{
            os << my_int;
        }
    protected:
        int64_t my_int;
    };

    class LvalNode : public ExprNode {
     public:
      LvalNode(LvalueNode * in_node):
         lval(in_node){}
      virtual ~LvalNode() = default;

      virtual void print(std::ostream & os) const override{
          os << *lval;
      }
     private:
      LvalueNode * lval;
    };

    class NegateNode : public ExprNode {
     public:
        NegateNode(ExprNode * child)
            :child_(child){}

        virtual ~NegateNode(){
            delete child_;
        }
        virtual void print(std::ostream & os) const override{
            os << '-' << *child_;
        }
    private:
        ExprNode * child_;
    };

    enum class BinaryOp { ADD, SUB, MUL, DIV, GREATEREQ, GREATER, LESS, LESSEQ, EQUAL, LESSGREATER, AND, OR };
    inline std::string str_rep(BinaryOp op){
        switch(op){
            case BinaryOp::ADD: return "+";
            case BinaryOp::SUB: return "-";
            case BinaryOp::MUL: return "*";
            case BinaryOp::DIV: return "/";
            case BinaryOp::GREATEREQ: return ">=";
            case BinaryOp::GREATER: return ">";
            case BinaryOp::LESS: return "<";
            case BinaryOp::LESSEQ: return "<=";
            case BinaryOp::EQUAL: return "=";
            case BinaryOp::LESSGREATER: return "<>";
            case BinaryOp::AND: return "&";
            case BinaryOp::OR: return "|";
        }
        assert(false && "bad binary op");
        return "";
    }
    class BinaryNode : public ExprNode {
     public:
        BinaryNode(ExprNode * in_left, ExprNode * in_right, BinaryOp in_op):
            left(in_left),
            right(in_right),
            op(in_op){}

        virtual ~BinaryNode(){
            delete left;
            delete right;
        }
        virtual void print(std::ostream & os) const override{
            os << "(" << *left << str_rep(op) << *right << ")";
        }
    private:
        ExprNode * left;
        ExprNode * right;
        BinaryOp op;
    };

    class AssignNode : public ExprNode {
     public:
        AssignNode(LvalueNode * in_target, ExprNode * in_source):
            target(in_target),
            source(in_source){}

        virtual ~AssignNode(){
            delete target;
            delete source;
        }
        virtual void print(std::ostream & os) const override{
            os << *target << ":=" << *source;
        }
    private:
        LvalueNode * target;
        ExprNode * source;
    };

    class FunctionCall : public ExprNode {
     public:
        FunctionCall(std::string in_func_name, ExprListNode * in_args):
            func_name(in_func_name),
            args(in_args){}

        virtual ~FunctionCall(){
            delete args;
        }
        virtual void print(std::ostream & os) const override{
            os << func_name << "(" << *args << ")";
        }
    private:
        std::string func_name;
        ExprListNode * args;
    };

    class ExprSequenceEval : public ExprNode {
     public:
        ExprSequenceEval(ExprSequenceNode * in_exprs):
            exprs(in_exprs){}

        virtual ~ExprSequenceEval(){
            delete exprs;
        }
        virtual void print(std::ostream & os) const override{
            if(exprs->singleton()){
                os << *exprs;
            }
            else{
                os << "(" << *exprs << ")";
            }
        }
    private:
        ExprSequenceNode * exprs;
    };

    class IfThen : public ExprNode {
     public:
        IfThen(ExprNode * cond, ExprNode * res):
            _cond(cond),
            _res(res){}
        virtual ~IfThen(){
            delete _cond;
            delete _res;
        }
        virtual void print(std::ostream & os) const override{
            os << " if " << "(" << *_cond << ")" << " then " << "(" << *_res<< ")";
        }
    private:
        ExprNode * _cond;
        ExprNode * _res;
    };

    class IfThenElse : public ExprNode {
     public:
        IfThenElse(ExprNode * cond, ExprNode * res_1, ExprNode * res_2):
            _cond(cond),
            _res_1(res_1),
            _res_2(res_2){}
        virtual ~IfThenElse(){
            delete _cond;
            delete _res_1;
            delete _res_2;
        }
        virtual void print(std::ostream & os) const override{
            os << " if " << "(" << *_cond << ")" << " then " << "(" << *_res_1<< ")" << " else " << "(" << *_res_2 << ")";
        }
    private:
        ExprNode * _cond;
        ExprNode * _res_1;
        ExprNode * _res_2;
    };

    class WhileDo : public ExprNode {
     public:
        WhileDo(ExprNode * cond, ExprNode * res):
            _cond(cond),
            _res(res){}
        virtual ~WhileDo(){
            delete _cond;
            delete _res;
        }
        virtual void print(std::ostream & os) const override{
            os << " while " << "(" << *_cond << ")" << " do " << "(" << *_res<< ")";
        }
    private:
        ExprNode * _cond;
        ExprNode * _res;
    };
    class ForToDo : public ExprNode {
     public:
        ForToDo(std::string var_id, ExprNode * initial, ExprNode * end, ExprNode * eval_expr):
            _var_id(var_id),
            _initial(initial),
            _end(end),
            _eval_expr(eval_expr){}
        virtual ~ForToDo(){
            delete _initial;
            delete _end;
            delete _eval_expr;
        }
        virtual void print(std::ostream & os) const override{
            os << " for " << _var_id << ":=" << *_initial << " to " << *_end << " do " << *_eval_expr;
        }
    private:
        std::string _var_id;
        ExprNode * _initial;
        ExprNode * _end;
        ExprNode * _eval_expr;
    };

    class Break: public ExprNode{
    public:
        Break(){}
        virtual ~Break() = default;
        virtual void print(std::ostream & os) const override{
            os << " break ";
        }
    };

    class ArrCreate : public ExprNode {
     public:
        ArrCreate(TypeIDNode * type, ExprNode * arr_size_expr, ExprNode * arr_value_expr):
            _type(type),
            _value_expr(arr_value_expr),
            _size_expr(arr_size_expr){}
        virtual ~ArrCreate(){
            delete _type;
            delete _value_expr;
            delete _size_expr;
        }
        virtual void print(std::ostream & os) const override{
            os << *_type << "[" << *_size_expr << "]" << " of " << *_value_expr;
        }
    private:
        TypeIDNode * _type;
        ExprNode * _value_expr;
        ExprNode * _size_expr;
    };

    class RecCreate : public ExprNode {
     public:
        RecCreate(TypeIDNode * type, FieldListNode * fields):
            _type(type),
            _fields(fields)
            {}
        virtual ~RecCreate(){
            delete _type;
            delete _fields;
        }
        virtual void print(std::ostream & os) const override{
            os << *_type << "{" << *_fields << "}";
        }
    private:
        TypeIDNode * _type;
        FieldListNode * _fields;
    };

    class LetIn : public ExprNode {
     public:
        LetIn(DeclarationListNode * decl_list, ExprSequenceNode * expr_sequ):
            _decl_list(decl_list),
            _expr_sequ(expr_sequ){}
        virtual ~LetIn(){
            delete _decl_list;
            delete _expr_sequ;
        }
        virtual void print(std::ostream & os) const override{
            os << " let " << *_decl_list << " in " << *_expr_sequ << " end ";
        }
    private:
        DeclarationListNode * _decl_list;
        ExprSequenceNode * _expr_sequ;
    };

    class TypeCreation : public ExprNode {
     public:
        TypeCreation(TypeIDNode * type_id, FieldListNode * feild_list):
            _type_id(type_id),
            _feild_list(feild_list){}
        virtual ~TypeCreation(){
            delete _type_id;
            delete _feild_list;
        }
        virtual void print(std::ostream & os) const override{
            os << *_type_id << "{" << *_feild_list << "}";
        }
    private:
        TypeIDNode * _type_id;
        FieldListNode * _feild_list;
    };
}

namespace lvals{
    class IdLval : public LvalueNode {
     public:
      IdLval(std::string in_id):
         my_id(in_id){}
      virtual ~IdLval(){
      }

      virtual void print(std::ostream & os) const override{
          os << my_id;
      }
     private:
      std::string my_id;
    };

    class AttrAccess : public LvalueNode {
     public:
      AttrAccess(LvalueNode * lval, std::string in_id):
         lvalnode(lval),
         my_id(in_id){}

      virtual ~AttrAccess(){
          delete lvalnode;
      }
      virtual void print(std::ostream & os) const override{
          os << *lvalnode << "." << my_id;
      }
     private:
      LvalueNode * lvalnode;
      std::string my_id;
    };

    class BracketAccess : public LvalueNode {
     public:
      BracketAccess(LvalueNode * lval, ExprNode * expr):
         _lval(lval),
         _expr(expr){}

      virtual ~BracketAccess(){
          delete _lval;
          delete _expr;
      }
      virtual void print(std::ostream & os) const override{
          os << *_lval << "[" << *_expr << "]";
      }
     private:
      LvalueNode * _lval;
      ExprNode * _expr;
    };
}
namespace decls{
    class VarDecl: public DeclarationNode {
     public:
      VarDecl(std::string id, TypeIDNode * type_id, ExprNode * expr, bool has_type_decl):
        _id(id),
        _type_id(type_id),
        _expr(expr),
        _has_type_decl(has_type_decl){}

        virtual ~VarDecl(){
            delete _type_id;
            delete _expr;
        }
        virtual void print(std::ostream & os) const override{
            os << " var " << _id;
            if(_has_type_decl){
                os << " : " << *_type_id;
            }
            os <<  " := " << *_expr;
        }
     protected:
         std::string _id;
         TypeIDNode * _type_id;
         ExprNode * _expr;
         bool _has_type_decl;
    };
    class FuncDecl: public DeclarationNode {
     public:
      FuncDecl(std::string id, TypeFeildsNode * ty_fields, TypeIDNode * type_id, ExprNode * expr, bool has_type_decl):
        _id(id),
        _ty_fields(ty_fields),
        _type_id(type_id),
        _expr(expr),
        _has_type_decl(has_type_decl){}

        virtual ~FuncDecl(){
            delete _ty_fields;
            delete _type_id;
            delete _expr;
        }
        virtual void print(std::ostream & os) const override{
            os << " function " << _id << "(" << *_ty_fields << ")";
            if(_has_type_decl){
                os << " : " << *_type_id;
            }
            os <<  " = " << *_expr;
        }
     protected:
         std::string _id;
         TypeFeildsNode * _ty_fields;
         TypeIDNode * _type_id;
         ExprNode * _expr;
         bool _has_type_decl;
    };
    class TypeDecl: public DeclarationNode {
     public:
      TypeDecl(TypeIDNode * id, TypeNode * type):
        _id(id),
        _type(type){}

        virtual ~TypeDecl(){
            delete _id;
            delete _type;
        }
        virtual void print(std::ostream & os) const override{
            os << " type " << *_id << "=" << *_type;
        }
     protected:
         TypeIDNode * _id;
         TypeNode * _type;
    };
}

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

} // namespace
