#include "semantic_check.hh"
#include "ast_interface.hh"
#include "symbol_table.hh"

SemanticException semantic_checks(tiger::ExprNode * node){
    try{
        SymbolTable types;
        node->eval_and_check_type(types);
    }
    catch(SemanticException & err){
        return err;
    }
    return SemanticException(SematicError::NO_ERRORS);
}
