
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
            VarDecl * var = to_sub_class<VarDecl>(list[list_idx].get());
            TypeExpr evaled_expr = var->_expr->eval_and_check_type(env);
            if(var->has_type()){
                assert_type_equality(evaled_expr,env.get_checked_type(var->type_name()),var->get_source_loc());
            }
            env.add_variable(var->name(),evaled_expr);
            list_idx += 1;
        }
        else if(start_type == DeclType::TYPE ||
                start_type == DeclType::FUNC){
           size_t list_end = list_idx+1;
            for(;list_end < list.size() &&
                 list[list_end]->type() == start_type;
                list_end++);

            if(start_type == DeclType::TYPE){
                vector<pair<string, UnresolvedType>> multu_type_info;
                for(size_t idx = list_idx; idx < list_end; idx++){
                    TypeDecl * type = to_sub_class<TypeDecl>(list[idx].get());
                    multu_type_info.push_back(make_pair(type->name(), type->typenode()));
                }
                env.add_type_set(multu_type_info);
            }
            else if(start_type == DeclType::FUNC){
                vector<pair<string, FuncHeader>> func_data;
                for(size_t idx = list_idx; idx < list_end; idx++){
                    FuncDecl * func = to_sub_class<FuncDecl>(list[idx].get());
                    FuncHeader header = {func->has_type(), func->ret_type(), second_of_pairs(func->arg_types())};
                    func_data.push_back(make_pair(func->name(), header));
                }
                env.add_function_set(func_data);
            }
            list_idx = list_end;
        }
        else{
            assert(false && "bad enum value");
        }
    }
}
