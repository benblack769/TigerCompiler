#include "semantic_check.hh"
#include "ast_interface.hh"
#include "symbol_table.hh"

SematicError semantic_checks(tiger::ExprNode * node){
    try{
        SymbolTable types;
        node->eval_and_check_type(types);
    }
    catch(SemanticException & err){
        return err.error_code();
    }
    return SematicError::NO_ERRORS;
}
