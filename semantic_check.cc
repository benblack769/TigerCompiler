#include "semantic_check.hh"
#include "ast_interface.hh"
#include "symbol_table.hh"

SematicError semantic_checks(tiger::ExprNode * node){
    try{
        TypeTable types;
        node->check_type_scopes(types);
    }
    catch(SemanticException & err){
        return err.error();
    }
    return SematicError::NO_ERRORS;
}
