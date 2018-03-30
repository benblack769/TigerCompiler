#include "semantic_check.hh"
#include "helper_files/type_expr.hh"

inline void assert_type_equality(TypeExpr expr1, TypeExpr expr2, SourceLoc loc){
    if(!expr1.is_convertible(expr2)){
        throw SemanticException(SematicError::BAD_TYPE_MATCH, loc);
    }
}
inline void assert_err(bool should_throw, SematicError error, SourceLoc loc){
    if(should_throw){
        throw SemanticException(error, loc);
    }
}
