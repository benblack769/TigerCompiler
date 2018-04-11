
#include "ast_interface.hh"
#include "ast_specifics/decs_ast.hh"

using namespace tiger;
using namespace decls;

void DeclarationListNode::append_to(DeclarationNode * expr){
   list.push_back(std::unique_ptr<DeclarationNode>(expr));
}
void DeclarationListNode::print(std::ostream & os) const {
   print_list(os, base_list(list), "\n");
}
template<typename ty_ty>
ty_ty * to_sub_class(DeclarationNode * node){
    ty_ty * decl = dynamic_cast<ty_ty *>(node);
    assert(decl != nullptr);
    return decl;
}
void DeclarationListNode::load_and_check_types(SymbolTable & env){
    for(size_t list_idx = 0; list_idx < list.size(); ){
        DeclType start_type = list[list_idx]->type();
        if(start_type == DeclType::VAR){
            //processes variable decleration using old environment to evaluate variable descrition
            VarDecl * var = to_sub_class<VarDecl>(list[list_idx].get());
            TypeExpr evaled_expr = var->_expr->eval_and_check_type(env);
            TypeExpr res_expr;
            if(var->has_type()){
                res_expr = env.get_checked_type(var->type_name());
                assert_type_equality(evaled_expr,res_expr,var->get_source_loc());
            }
            else{
                res_expr = evaled_expr;
            }
            env.add_variable(var->name(),res_expr,full_frame.current_level(), F_allocLocal(full_frame.current_frame(),true));
            list_idx += 1;
        }
        else if(start_type == DeclType::TYPE ||
                start_type == DeclType::FUNC){
            //mutually recursive scopes for types and functions are defined to be:
            //Sequential declarations of types or functions.
            //so in declaration layed out like: (type, type, func, var, func, func)
            //the first two types will be possibly mutually recursive, and so will the last two functions.
            size_t list_end = list_idx+1;
            for(;list_end < list.size() &&
                 list[list_end]->type() == start_type;
                list_end++);

            if(start_type == DeclType::TYPE){
                //All type info is simply passed to the symbol_table for evaluation
                vector<pair<string, UnresolvedType>> multu_type_info;
                for(size_t idx = list_idx; idx < list_end; idx++){
                    TypeDecl * type = to_sub_class<TypeDecl>(list[idx].get());
                    multu_type_info.push_back(make_pair(type->name(), type->typenode()));
                }
                env.add_type_set(multu_type_info);
            }
            else if(start_type == DeclType::FUNC){
                //processes function headers, and then their contents
                vector<pair<string, FuncHeader>> func_data;
                for(size_t idx = list_idx; idx < list_end; idx++){
                    FuncDecl * func = to_sub_class<FuncDecl>(list[idx].get());
                    FuncHeader header = {func->has_type(), func->has_type() ? func->ret_type() : "??", second_of_pairs(func->arg_types())};
                    func_data.push_back(make_pair(func->name(), header));
                }
                env.add_function_set(func_data,full_frame.current_level());

                //checks types inside function definitions, verifies that the expression is correct, and also checks the consistency of the return type
                for(size_t idx = list_idx; idx < list_end; idx++){
                    FuncDecl * func = to_sub_class<FuncDecl>(list[idx].get());
                    //create new table for function arguments
                    SymbolTable new_env = env;
                    full_frame.new_frame(F_newFrame(newlabel(),F_formals_all_true(func->number_args())));
                    for(auto var_pair : func->arg_types()){
                        new_env.add_variable(var_pair.first,new_env.get_checked_type(var_pair.second),full_frame.current_level(),F_allocLocal(full_frame.current_frame(),true));
                    }
                    TypeExpr func_ret_ty = func->_expr->eval_and_check_type(new_env);
                    if(func->has_type()){
                        assert_type_equality(func_ret_ty,env.get_checked_type(func->ret_type()),func->get_source_loc());
                    }
                    full_frame.pop_frame();
                }
            }
            list_idx = list_end;
        }
        else{
            assert(false && "bad enum value");
        }
    }
}
