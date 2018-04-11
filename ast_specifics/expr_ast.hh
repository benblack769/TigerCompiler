#pragma once
#include "ast_interface.hh"

namespace tiger{

namespace exprs{

class StringNode: public ExprNode{
public:
    StringNode(std::string in_str):
        mystring(in_str){}

    virtual ~StringNode() = default;
    virtual TypeExpr eval_and_check_type(SymbolTable & ){
        return string_type();
    }
    virtual void print(std::ostream & os) const override{
        os << mystring;
    }
    virtual IRTptr translate(SymbolTable & env) const override;
protected:
    std::string mystring;
};

class NilNode: public ExprNode{
public:
    NilNode(){}
    virtual ~NilNode() = default;
    virtual TypeExpr eval_and_check_type(SymbolTable & ){
        return nil_type();
    }
    virtual void print(std::ostream & os) const override{
        os << "nil";
    }
    virtual IRTptr translate(SymbolTable & env) const override;
};

class IntNode: public ExprNode{
public:
    IntNode(int64_t in_int):
        my_int(in_int){}

    virtual ~IntNode() = default;
    virtual TypeExpr eval_and_check_type(SymbolTable & ){
        return int_type();
    }
    virtual void print(std::ostream & os) const override{
        os << my_int;
    }
    virtual IRTptr translate(SymbolTable & env) const override;
protected:
    int64_t my_int;
};

class LvalNode : public ExprNode {
 public:
  LvalNode(LvalueNode * in_node):
     lval(in_node){}
  virtual ~LvalNode() = default;
  virtual TypeExpr eval_and_check_type(SymbolTable & env){
      return lval->get_type(env);
  }

  virtual void print(std::ostream & os) const override{
      os << *lval;
  }
  virtual IRTptr translate(SymbolTable & env) const override;
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
    virtual TypeExpr eval_and_check_type(SymbolTable & env){
        TypeExpr ch_ty = child_->eval_and_check_type(env);
        assert_type_equality(ch_ty, int_type(), loc);
        return ch_ty;
    }
    virtual void print(std::ostream & os) const override{
        os << '-' << *child_;
    }
    virtual IRTptr translate(SymbolTable & env) const override;
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
    virtual TypeExpr eval_and_check_type(SymbolTable & env){
        TypeExpr left_ty = left->eval_and_check_type(env);
        TypeExpr right_ty = right->eval_and_check_type(env);
        assert_type_equality(left_ty, right_ty, loc);
        if(op == BinaryOp::EQUAL || op == BinaryOp::LESSGREATER){
            assert_err(right_ty.is_convertible(right_ty), SematicError::BAD_TYPE_MATCH, loc);
            return int_type();
        }
        else{
            assert_type_equality(left_ty, int_type(), loc);
            return int_type();
        }
    }
    virtual void print(std::ostream & os) const override{
        os << "(" << *left << str_rep(op) << *right << ")";
    }
    virtual IRTptr translate(SymbolTable & env) const override;
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
    virtual TypeExpr eval_and_check_type(SymbolTable & env){
        assert_type_equality(target->get_type(env), source->eval_and_check_type(env), loc);
        return void_type();
    }
    virtual void print(std::ostream & os) const override{
        os << *target << ":=" << *source;
    }
    virtual IRTptr translate(SymbolTable & env) const override;
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
    virtual TypeExpr eval_and_check_type(SymbolTable & env){
        assert_err(env.has_var_symbol(func_name), SematicError::FUNCTION_NOT_DEFINED, loc);
        assert_err(env.symbol_is_func(func_name), SematicError::USING_VAR_AS_FUNC, loc);
        assert_err(env.verify_function_args(func_name, args->eval_and_check_type(env)), SematicError::BAD_TYPE_MATCH, loc);
        return env.function_ret_type(func_name);
    }
    virtual void print(std::ostream & os) const override{
        os << func_name << "(" << *args << ")";
    }
    virtual IRTptr translate(SymbolTable & env) const override;
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
    virtual TypeExpr eval_and_check_type(SymbolTable & env){
        return exprs->eval_and_check_type(env);
    }
    virtual void print(std::ostream & os) const override{
        if(exprs->singleton()){
            os << *exprs;
        }
        else{
            os << "(" << *exprs << ")";
        }
    }
    virtual IRTptr translate(SymbolTable & env) const override;
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
    virtual TypeExpr eval_and_check_type(SymbolTable & env){
        assert_type_equality(_cond->eval_and_check_type(env), int_type(), loc);
        _res->eval_and_check_type(env);
        return void_type();
    }
    virtual void print(std::ostream & os) const override{
        os << " if " << "(" << *_cond << ")" << " then " << "(" << *_res<< ")";
    }
    virtual IRTptr translate(SymbolTable & env) const override;
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
    virtual TypeExpr eval_and_check_type(SymbolTable & env){
        assert_type_equality(_cond->eval_and_check_type(env), int_type(), loc);
        TypeExpr ret_ty = _res_1->eval_and_check_type(env);
        assert_type_equality(ret_ty,_res_2->eval_and_check_type(env), loc);
        return ret_ty;
    }
    virtual void print(std::ostream & os) const override{
        os << " if " << "(" << *_cond << ")" << " then " << "(" << *_res_1<< ")" << " else " << "(" << *_res_2 << ")";
    }
    virtual IRTptr translate(SymbolTable & env) const override;
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
    virtual TypeExpr eval_and_check_type(SymbolTable & env){
        assert_type_equality(_cond->eval_and_check_type(env), int_type(), loc);
        _res->eval_and_check_type(env);
        return void_type();
    }
    virtual void print(std::ostream & os) const override{
        os << " while " << "(" << *_cond << ")" << " do " << "(" << *_res<< ")";
    }
    virtual IRTptr translate(SymbolTable & env) const override;
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
    virtual TypeExpr eval_and_check_type(SymbolTable & old_env){
        SymbolTable new_env = old_env;
        new_env.add_variable(_var_id,int_type(),full_frame.current_level(), F_allocLocal(full_frame.current_frame(),true));
        assert_type_equality(_initial->eval_and_check_type(old_env), int_type(), loc);
        assert_type_equality(_end->eval_and_check_type(old_env), int_type(), loc);
        return void_type();
    }
    virtual void print(std::ostream & os) const override{
        os << " for " << _var_id << ":=" << *_initial << " to " << *_end << " do " << *_eval_expr;
    }
    virtual IRTptr translate(SymbolTable & env) const override;
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

    virtual TypeExpr eval_and_check_type(SymbolTable & ){
        return void_type();
    }
    virtual void print(std::ostream & os) const override{
        os << " break ";
    }
    virtual IRTptr translate(SymbolTable & env) const override;
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
    virtual TypeExpr eval_and_check_type(SymbolTable & env){
        TypeExpr arr_type = _type->eval_and_check_type(env);
        assert_err(arr_type.type == BaseType::ARRAY, SematicError::BAD_TYPE_MATCH, loc);
        assert_type_equality(_size_expr->eval_and_check_type(env), int_type(), loc);
        assert_type_equality(_value_expr->eval_and_check_type(env), env.array_subtype(arr_type), loc);
        return arr_type;
    }
    virtual void print(std::ostream & os) const override{
        os << *_type << "[" << *_size_expr << "]" << " of " << *_value_expr;
    }
    virtual IRTptr translate(SymbolTable & env) const override;
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
    virtual TypeExpr eval_and_check_type(SymbolTable & env){
        TypeExpr rec_ty = _type->eval_and_check_type(env);
        assert_err(rec_ty.type == BaseType::RECORD, SematicError::BAD_TYPE_MATCH, loc);
        vector<pair<string,TypeExpr>> rec_arg_tys = _fields->eval_and_check_type_names(env);
        assert_err(first_strs_unique(rec_arg_tys),SematicError::NON_UNIQUE_RECORD_LABELS, loc);
        for(auto str_pair : rec_arg_tys){
            assert_err(env.record_subexpr_exists(rec_ty, str_pair.first), SematicError::INCOMPATABLE_RECORD_LABEL, loc);
            assert_type_equality(str_pair.second, env.get_record_subexpr(rec_ty, str_pair.first), loc);
        }
        return rec_ty;
    }
    virtual void print(std::ostream & os) const override{
        os << *_type << "{" << *_fields << "}";
    }
    virtual IRTptr translate(SymbolTable & env) const override;
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
    virtual TypeExpr eval_and_check_type(SymbolTable & env){
        SymbolTable new_env = env;
        _decl_list->load_and_check_types(new_env);
        return _expr_sequ->eval_and_check_type(new_env);
    }
    virtual void print(std::ostream & os) const override{
        os << " let " << *_decl_list << " in " << *_expr_sequ << " end ";
    }
    virtual IRTptr translate(SymbolTable & env) const override;
private:
    DeclarationListNode * _decl_list;
    ExprSequenceNode * _expr_sequ;
};

}

}
