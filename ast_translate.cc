#include <vector>
#include "ast_interface.hh"
#include "ast_specifics/expr_ast.hh"
#include "ast_specifics/lval_ast.hh"
#include "ast_specifics/decs_ast.hh"
#include "ast_specifics/types_ast.hh"

FrameStack full_frame;

namespace tiger{

IRTptr exprs::StringNode::translate() const{return nullptr;}
IRTptr exprs::NilNode::translate() const{return nullptr;}
IRTptr exprs::IntNode::translate() const{return nullptr;}
IRTptr exprs::LvalNode::translate() const{return nullptr;}
IRTptr exprs::NegateNode::translate() const{return nullptr;}
IRTptr exprs::BinaryNode::translate() const{return nullptr;}
IRTptr exprs::AssignNode::translate() const{return nullptr;}
IRTptr exprs::FunctionCall::translate() const{return nullptr;}
IRTptr exprs::ExprSequenceEval::translate() const{return nullptr;}
IRTptr exprs::IfThen::translate() const{return nullptr;}
IRTptr exprs::IfThenElse::translate() const{return nullptr;}
IRTptr exprs::WhileDo::translate() const{return nullptr;}
IRTptr exprs::ForToDo::translate() const{return nullptr;}
IRTptr exprs::Break::translate() const{return nullptr;}
IRTptr exprs::ArrCreate::translate() const{return nullptr;}
IRTptr exprs::RecCreate::translate() const{return nullptr;}
IRTptr exprs::LetIn::translate() const{return nullptr;}

IRTptr lvals::IdLval::translate() const{return nullptr;}
IRTptr lvals::AttrAccess::translate() const{return nullptr;}
IRTptr lvals::BracketAccess::translate() const{return nullptr;}

IRTptr decls::VarDecl::translate() const{return nullptr;}
IRTptr decls::FuncDecl::translate() const{return nullptr;}
IRTptr decls::TypeDecl::translate() const{return nullptr;}

IRTptr types::BasicType::translate() const{return nullptr;}
IRTptr types::ArrayType::translate() const{return nullptr;}
IRTptr types::TypeFeildType::translate() const{return nullptr;}

IRTptr ExprListNode::translate() const{return nullptr;}
IRTptr ExprSequenceNode::translate() const{return nullptr;}
IRTptr FieldNode::translate() const{return nullptr;}
IRTptr TypeIDNode::translate() const{return nullptr;}
IRTptr TypeFeildNode::translate() const{return nullptr;}
IRTptr FieldListNode::translate() const{return nullptr;}
IRTptr DeclarationListNode::translate() const{return nullptr;}
IRTptr TypeFeildsNode::translate() const{return nullptr;}

//conditional evaluation helper function (used for boolean operations like | as well as if statements)
/*T_expr compile_conditional(RelOp, left, right, then_expr, else_expr){
	result = Temp_temp()

	then = new_label()
	else = new_label()
	end = new_label()

	stmts = {
		T_Cjump(op,left,right,then,else),
		T_Label(then),
		T_Move(T_Temp(result), then_expr),
		T_Label(else),
		T_Move(T_Temp(result), else_expr),
		T_Label(end),
	}
	T_Eseq(stmts, T_Temp(result))
}*/
//variable_access
F_access get_static_link(int level){
    return F_formals(full_frame.frame_at_level(level)).back();
}
ir::expPtr translate_variable(F_access acc, int level){
    int cur_level = full_frame.current_level();

    assert(level <= cur_level);
    ir::expPtr cur_link_expr = make_shared<ir::Temp>(F_FP());
    for(int l = cur_level; l > level; l--){
        F_access static_link_acc = get_static_link(l);
        ir::expPtr new_link_expr = F_Exp(static_link_acc, make_shared<ir::Temp>(F_FP()));
        cur_link_expr.swap(new_link_expr);
    }
    return F_Exp(acc,cur_link_expr);
}
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

} //namespace
