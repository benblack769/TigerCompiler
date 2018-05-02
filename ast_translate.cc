#include <vector>
#include "frame.hh"
#include "temp.hh"
#include "ast_interface.hh"
#include "ast_specifics/expr_ast.hh"
#include "ast_specifics/lval_ast.hh"
#include "ast_specifics/decs_ast.hh"
#include "ast_specifics/types_ast.hh"

FrameStack full_frame;
std::vector<FuncFrag> func_fragments;
std::vector<StringFrag> str_fragments;


using namespace tiger;
using namespace ir;

IRTptr exprs::StringNode::translate(const SymbolTable & env) const{
    StringFrag str_frag{this->mystring,newlabel()};
    str_fragments.push_back(str_frag);
    return to_expPtr(Name(str_frag.str_label));
}
// nil will just be a constant 0
IRTptr exprs::NilNode::translate(const SymbolTable & env) const{
    return std::make_shared<Const>(0);
}
IRTptr exprs::IntNode::translate(const SymbolTable & env) const{
    return std::make_shared<ir::Const>(my_int);
}
IRTptr exprs::LvalNode::translate(const SymbolTable & env) const{
    return lval->translate(env);
}
// Make a (-1) * exp ast node then translate that
IRTptr exprs::NegateNode::translate(const SymbolTable & env) const{
    auto neg1Node = new exprs::IntNode(-1);
    auto mulNode = new exprs::BinaryNode(neg1Node,child_,exprs::BinaryOp::MUL);
    return mulNode->translate(env);
}

expPtr to_expr_seq(stmPtrList list,expPtr expr){
    expPtr res_expr = expr;
    for(int64_t i = int64_t(list.size())-1; i >= 0; i--){
        res_expr = to_expPtr(Eseq(list[i],res_expr));
    }
    return res_expr;
}
stmPtr to_stm_seq(stmPtrList list){
    stmPtr res_stm = NoOp();
    for(size_t i = 0; i < list.size(); i++){
        res_stm = to_stmPtr(Seq(res_stm,list[i]));
    }
    return res_stm;
}
ir::expPtr compile_conditional(rel_op_k oper, ir::expPtr left, ir::expPtr right, ir::expPtr then_expr, ir::expPtr else_expr){
    Temp_temp result = newtemp();

    Temp_label then_l = newlabel();
    Temp_label end_l = newlabel();

    Temp_label else_l = newlabel();
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
op_k to_op_k(exprs::BinaryOp op){
    using namespace  exprs;
    switch(op){
    case BinaryOp::ADD: return op_k::PLUS;
    case BinaryOp::SUB: return op_k::MINUS;
    case BinaryOp::MUL: return op_k::MUL;
    case BinaryOp::DIV: return op_k::DIV;
    default:
        assert(false && "bad binop to op_k");
    }
    return op_k::PLUS;
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
IRTptr exprs::BinaryNode::translate(const SymbolTable & env) const{
    if(op == BinaryOp::ADD
            || op == BinaryOp::SUB
            || op == BinaryOp::MUL
            || op == BinaryOp::DIV){
        if (auto irL = dynamic_pointer_cast<exp>(left->translate(env))){
            if (auto irR = dynamic_pointer_cast<exp>(right->translate(env))){
                return std::make_shared<BinOp>(to_op_k(op), irL, irR);
            }
        }
        assert(false && "left or right of Binop is not an integer");
        return nullptr;
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
IRTptr exprs::AssignNode::translate(const SymbolTable & env) const{
    return to_stmPtr(Move(cast_to_exprPtr(target->translate(env)),
                          cast_to_exprPtr(source->translate(env))));
}
IRTptr exprs::FunctionCall::translate(const SymbolTable & env) const{
    FuncEntry func_data = env.func_data(this->func_name);
    expPtrList arglist;
    for(auto & arg : args->list){
        arglist.push_back(cast_to_exprPtr(arg->translate(env)));
    }
    return to_expPtr(Call(func_data.func_label,arglist));
}
// since this class wraps ExprSequenceNode, so will this function
IRTptr exprs::ExprSequenceEval::translate(const SymbolTable & env) const{
    return exprs->translate(env);
}
IRTptr exprs::IfThen::translate(const SymbolTable & env) const{
    Temp_label then_l = newlabel();
    Temp_label end_l = newlabel();

    stmPtrList stmts = {
        to_stmPtr(CJump(rel_op_k::EQ,
                        to_expPtr(Const(0)),
                        cast_to_exprPtr(_cond->translate(env)),
                        then_l,end_l)),
        to_stmPtr(Label(then_l)),
        make_stmPtr(_res->translate(env)),
        to_stmPtr(Label(end_l)),
    };
    return to_stm_seq(stmts);
}
IRTptr exprs::IfThenElse::translate(const SymbolTable & env) const{
    return compile_conditional(rel_op_k::NE,
                               to_expPtr(Const(0)),
                               cast_to_exprPtr(_cond->translate(env)),
                               make_expPtr(_res_1->translate(env)),
                               make_expPtr(_res_2->translate(env)));
}
stmPtr while_stmt(expPtr irCond, stmPtr irBody){
    auto startLbl = newlabel(); // right before the Cjump
    auto bodyLbl = newlabel(); // right before we start the body
    auto endLbl = newlabel(); // after the body

    // add jump and label to the end of body
    auto jumpEnd = std::make_shared<Seq>(std::make_shared<Jump>(startLbl), std::make_shared<Label>(endLbl));
    irBody = std::make_shared<Seq>(irBody, jumpEnd);
    // add label at the start of body
    irBody = std::make_shared<Seq>(std::make_shared<Label>(bodyLbl), irBody);
    // compare the cond to 0
    auto cjmp = std::make_shared<CJump>(rel_op_k::NE, std::make_shared<Const>(0), irCond, bodyLbl, endLbl);
    auto jmpNlbl = std::make_shared<Seq>(std::make_shared<Label>(startLbl), cjmp);
    return std::make_shared<Seq>(jmpNlbl, irBody);
}
IRTptr exprs::WhileDo::translate(const SymbolTable & env) const{
    // the condition must produce a value, but the body must not
    auto irCond = cast_to_exprPtr(_cond->translate(env));
    stmPtr irBody = cast_to_stmPtr(_res->translate(env));
    return while_stmt(irCond, irBody);
}
IRTptr exprs::ForToDo::translate(const SymbolTable & old_env) const{
    expPtr end_t = to_expPtr(Temp(newtemp()));
    expPtr var_access = new_env.var_data(_var_id).access->exp(full_frame.current_frame()->getFP());
    expPtr conditional= compile_conditional(rel_op_k::LT,
                                            var_access,
                                            end_t,
                                            to_expPtr(Const(1)),
                                            to_expPtr(Const(0)));
    stmPtrList stmts = {
        to_stmPtr(Move(end_t,cast_to_exprPtr(_end->translate(old_env)))),
        to_stmPtr(Move(var_access,cast_to_exprPtr(_initial->translate(old_env)))),
        while_stmt(conditional,make_stmPtr(_eval_expr->translate(new_env)))
    };

    return to_stm_seq(stmts);
}
IRTptr exprs::Break::translate(const SymbolTable & env) const{
    assert(false && "break not implemented");
    return nullptr;
}
IRTptr exprs::ArrCreate::translate(const SymbolTable & env) const{
    ir::expPtrList args = {
        cast_to_exprPtr(this->_size_expr->translate(env)),
        cast_to_exprPtr(this->_value_expr->translate(env)),
    };
    return full_frame.current_frame()->externalCall("createArray",args);
}
IRTptr exprs::RecCreate::translate(const SymbolTable & env) const{
    assert(false && "Records not implemented");
    return nullptr;
}
IRTptr exprs::LetIn::translate(const SymbolTable & ) const{
    auto irtDecs = _decl_list->translate(new_env);
    auto irtExps = _expr_sequ->translate(new_env);

    // irtDecs must be an stm
    auto stmNode = cast_to_stmPtr(irtDecs);
    // irtExps may be an exp or a stm
    if(auto expNode = dynamic_pointer_cast<ir::exp>(irtExps)){
        return std::make_shared<ir::Eseq>(stmNode, cast_to_exprPtr(expNode));
    }
    else if(auto stmNode2 = dynamic_pointer_cast<ir::stm>(irtExps)){
        return std::make_shared<ir::Seq>(stmNode, stmNode2);
    } else {
        std::cerr << "In LetIn::translate:" << std::endl;
        std::cerr << "Got a pointer to neither an exp or stm type node. Probably a nullptr" << std::endl;
        throw 1;
    }
}

Access get_static_link(int level){
    return full_frame.frame_at_level(level)->formals().back();
}
ir::expPtr translate_variable(Access acc, int level){
    int cur_level = full_frame.current_level();
    Frame cur_frame = full_frame.current_frame();
    assert(level <= cur_level);
    ir::expPtr cur_link_expr = to_expPtr(ir::Temp(cur_frame->getFP()));
    for(int l = cur_level; l > level; l--){
        Access static_link_acc = get_static_link(l);
        ir::expPtr new_link_expr = static_link_acc->expStaticLink(cur_link_expr);
        cur_link_expr = new_link_expr;
    }
    return acc->expStaticLink(cur_link_expr);
}
ir::expPtr get_and_translate_var(const SymbolTable & env, string id){
    VarEntry var_data = env.var_data(id);
    return translate_variable(var_data.access, var_data.level);
}
IRTptr lvals::IdLval::translate(const SymbolTable & env) const{
    return get_and_translate_var(env, my_id);
}
IRTptr lvals::AttrAccess::translate(const SymbolTable & env) const{return nullptr;}
IRTptr lvals::BracketAccess::translate(const SymbolTable & env) const{
    return to_expPtr(BinOp(op_k::PLUS, cast_to_exprPtr(_lval->translate(env)),
                                       cast_to_exprPtr(_expr->translate(env))));
}

IRTptr decls::VarDecl::translate(const SymbolTable & old_env) const{
    return to_stmPtr(Move(get_and_translate_var(new_env,_id), cast_to_exprPtr(this->_expr->translate(old_env))));
}
IRTptr decls::FuncDecl::translate(const SymbolTable &) const{
    full_frame.new_frame(my_frame);
    func_fragments.push_back(FuncFrag{my_frame,this->_expr->translate(new_env)});
    full_frame.pop_frame();
    return nullptr;
}
IRTptr decls::TypeDecl::translate(const SymbolTable & env) const{
    assert(false && "types should not be translated");
    return nullptr;
}

IRTptr ExprListNode::translate(const SymbolTable & env) const{return nullptr;}
/*
 If the last value in this sequence returns a value, this function will return
 An Eseq, otherwise it will return a Seq
 */
IRTptr ExprSequenceNode::translate(const SymbolTable & env) const{
    if(empty()){
        return NoOp();
    }
    if(singleton()){
        return list.front()->translate(env);
    }
    auto rNode = list.at(list.size()-1)->translate(env);
    for(int i = list.size()-2; i >= 0; i--){
        auto lNode = list.at(i)->translate(env);
        if(auto expNode = dynamic_pointer_cast<ir::exp>(lNode)){
            lNode = std::make_shared<ir::Exp>(expNode); //Exp is a stm, unlike exp
        }
        // at this point, we know lNode is a stm
        auto stmNode = dynamic_pointer_cast<ir::stm>(lNode);
        // the left of an Eseq or Seq is always an stm, but the right of
        // a Eseq must be an exp, and for Seq an stm
        if(auto expNode = dynamic_pointer_cast<ir::exp>(rNode)){
            rNode = std::make_shared<ir::Eseq>(stmNode, expNode);
        }
        else if(auto stmNode2 = dynamic_pointer_cast<ir::stm>(rNode)){
            rNode = std::make_shared<ir::Seq>(stmNode, stmNode2);
        } else {
            std::cerr << "In ExprSequenceNode::translate:" << std::endl;
            std::cerr << "Got a pointer to neither an exp or stm type node. Probably a nullptr" << std::endl;
            throw 1;
        }
    }
    return rNode;
}
IRTptr FieldNode::translate(const SymbolTable & env) const{return nullptr;}
IRTptr TypeFeildNode::translate(const SymbolTable & env) const{return nullptr;}
IRTptr FieldListNode::translate(const SymbolTable & env) const{return nullptr;}
IRTptr DeclarationListNode::translate(const SymbolTable & env) const{
    stmPtrList stmts;
    for(auto & decl_node : list){
        switch(decl_node->type()){
        case DeclType::VAR: stmts.push_back(cast_to_stmPtr(decl_node->translate(env))); break;
        case DeclType::FUNC: decl_node->translate(env); break;
        case DeclType::TYPE: break;
        }
    }
    return to_stm_seq(stmts);
}

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
