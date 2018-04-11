#include <vector>
#include "frame.hh"
#include "temp.hh"
#include "ast_interface.hh"
#include "ast_specifics/expr_ast.hh"
#include "ast_specifics/lval_ast.hh"
#include "ast_specifics/decs_ast.hh"
#include "ast_specifics/types_ast.hh"

FrameStack full_frame;

using namespace tiger;
using namespace ir;

IRTptr exprs::StringNode::translate(SymbolTable & env) const{return nullptr;}
IRTptr exprs::NilNode::translate(SymbolTable & env) const{return nullptr;}
IRTptr exprs::IntNode::translate(SymbolTable & env) const{return nullptr;}
IRTptr exprs::LvalNode::translate(SymbolTable & env) const{return nullptr;}
IRTptr exprs::NegateNode::translate(SymbolTable & env) const{return nullptr;}

expPtr to_expr_seq(stmPtrList list,expPtr expr){
    expPtr res_expr = expr;
    for(stmPtr st : list){
        res_expr = to_expPtr(Eseq(st,res_expr));
    }
    return res_expr;
}
ir::expPtr compile_conditional(rel_op_k oper, ir::expPtr left, ir::expPtr right, ir::expPtr then_expr, ir::expPtr else_expr){
    Temp_temp result = newtemp();

    Temp_label then_l = newlabel();
    Temp_label else_l = newlabel();
    Temp_label end_l = newlabel();

    stmPtrList stmts = {
        to_stmPtr(CJump(oper,left,right,then_l,else_l)),
        to_stmPtr(Label(then_l)),
        to_stmPtr(Move(to_expPtr(Temp(result)), then_expr)),
        to_stmPtr(Jump(end_l)),
        to_stmPtr(Label(else_l)),
        to_stmPtr(Move(to_expPtr(Temp(result)), else_expr)),
        to_stmPtr(Label(end_l)),
    };
    return to_expr_seq(stmts, to_expPtr(Temp(result)));
}
rel_op_k to_rel_op(exprs::BinaryOp op){
    using namespace  exprs;
    switch(op){
    case BinaryOp::EQUAL: return rel_op_k::EQ;
    case BinaryOp::LESS: return rel_op_k::LT;
    case BinaryOp::LESSEQ: return rel_op_k::LE;
    case BinaryOp::GREATER: return rel_op_k::GT;
    case BinaryOp::GREATEREQ: return rel_op_k::GE;
    case BinaryOp::LESSGREATER: return rel_op_k::NE;
    default:
        assert(false && "bad binop to to_rel_op");
    }
    return rel_op_k::EQ;
}
IRTptr exprs::BinaryNode::translate(SymbolTable & env) const{
    if(op == BinaryOp::ADD
            || op == BinaryOp::SUB
            || op == BinaryOp::MUL
            || op == BinaryOp::DIV){

    }
    else if(op == BinaryOp::AND ||
            op == BinaryOp::OR){
        expPtr left_e = cast_to_exprPtr(left->translate(env));
        expPtr right_e = cast_to_exprPtr(right->translate(env));
        expPtr one_e = to_expPtr(Const(1));
        expPtr zero_e = to_expPtr(Const(0));
        if(op == BinaryOp::OR){
            return compile_conditional(rel_op_k::EQ,
                                       left_e,
                                       one_e,
                                       one_e,
                                       right_e);
        }
        else{
            return compile_conditional(rel_op_k::EQ,
                                       left_e,
                                       zero_e,
                                       zero_e,
                                       right_e);
        }
    }
    else{
        return compile_conditional(to_rel_op(op),
                                   cast_to_exprPtr(left->translate(env)),
                                   cast_to_exprPtr(right->translate(env)),
                                   to_expPtr(Const(1)),
                                   to_expPtr(Const(0)));
    }
}
IRTptr exprs::AssignNode::translate(SymbolTable & env) const{return nullptr;}
IRTptr exprs::FunctionCall::translate(SymbolTable & env) const{return nullptr;}
IRTptr exprs::ExprSequenceEval::translate(SymbolTable & env) const{return nullptr;}
IRTptr exprs::IfThen::translate(SymbolTable & env) const{return nullptr;}
IRTptr exprs::IfThenElse::translate(SymbolTable & env) const{
    return compile_conditional(rel_op_k::EQ,
                               to_expPtr(Const(0)),
                               cast_to_exprPtr(_cond->translate(env)),
                               cast_to_exprPtr(_res_1->translate(env)),
                               cast_to_exprPtr(_res_2->translate(env)));
}
IRTptr exprs::WhileDo::translate(SymbolTable & env) const{return nullptr;}
IRTptr exprs::ForToDo::translate(SymbolTable & env) const{return nullptr;}
IRTptr exprs::Break::translate(SymbolTable & env) const{return nullptr;}
IRTptr exprs::ArrCreate::translate(SymbolTable & env) const{return nullptr;}
IRTptr exprs::RecCreate::translate(SymbolTable & env) const{return nullptr;}
IRTptr exprs::LetIn::translate(SymbolTable & env) const{return nullptr;}

F_access get_static_link(int level){
    return F_formals(full_frame.frame_at_level(level)).back();
}
ir::expPtr translate_variable(F_access acc, int level){
    int cur_level = full_frame.current_level();

    assert(level <= cur_level);
    ir::expPtr cur_link_expr = to_expPtr(ir::Temp(F_FP()));
    for(int l = cur_level; l > level; l--){
        F_access static_link_acc = get_static_link(l);
        ir::expPtr new_link_expr = F_Exp(static_link_acc, cur_link_expr);
        cur_link_expr = new_link_expr;
    }
    return F_Exp(acc,cur_link_expr);
}
IRTptr lvals::IdLval::translate(SymbolTable & env) const{
    VarEntry var_data = env.var_data(my_id);
    return translate_variable(var_data.access, var_data.level);
}
IRTptr lvals::AttrAccess::translate(SymbolTable & env) const{return nullptr;}
IRTptr lvals::BracketAccess::translate(SymbolTable & env) const{return nullptr;}

IRTptr decls::VarDecl::translate(SymbolTable & env) const{return nullptr;}
IRTptr decls::FuncDecl::translate(SymbolTable & env) const{return nullptr;}
IRTptr decls::TypeDecl::translate(SymbolTable & env) const{return nullptr;}

IRTptr types::BasicType::translate(SymbolTable & env) const{return nullptr;}
IRTptr types::ArrayType::translate(SymbolTable & env) const{return nullptr;}
IRTptr types::TypeFeildType::translate(SymbolTable & env) const{return nullptr;}

IRTptr ExprListNode::translate(SymbolTable & env) const{return nullptr;}
IRTptr ExprSequenceNode::translate(SymbolTable & env) const{return nullptr;}
IRTptr FieldNode::translate(SymbolTable & env) const{return nullptr;}
IRTptr TypeIDNode::translate(SymbolTable & env) const{return nullptr;}
IRTptr TypeFeildNode::translate(SymbolTable & env) const{return nullptr;}
IRTptr FieldListNode::translate(SymbolTable & env) const{return nullptr;}
IRTptr DeclarationListNode::translate(SymbolTable & env) const{return nullptr;}
IRTptr TypeFeildsNode::translate(SymbolTable & env) const{return nullptr;}

//conditional evaluation helper function (used for boolean operations like | as well as if statements)

//function access
/*T_expr get_function_static_link(int func_level){
    int cur_level = get_current_level();

    assert(func_level + 1 <= cur_level);
    if(func_level + 1 == cur_level){
        return FP();
    }
    else if(func_level == cur_level){
        return F_Exp(get_static_link(func_level),FP());
    }
    else if(true){
        //
    }
}*/
