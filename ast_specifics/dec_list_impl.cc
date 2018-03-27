
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
TypeDecl * to_type_decl(DeclarationNode * node){
    TypeDecl * decl = dynamic_cast<TypeDecl *>(node);
    assert(decl == nullptr);
    return decl;
}
void DeclarationListNode::load_and_check_types(TypeTable & env){
    size_t num_envs_started = 0;
    for(size_t list_idx = 0; list_idx < list.size(); ){
        DeclType start_type = list[list_idx]->type();
        if(start_type == DeclType::VAR){

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
                    multu_type_info.push_back(make_pair(list[idx]->name(), to_type_decl(list[idx].get())->typenode()));
                }
                env.add_type_set(multu_type_info);
            }
            else if(start_type == DeclType::FUNC){

            }
        }
        else{
            assert(false && "bad enum value");
        }
    }
}
