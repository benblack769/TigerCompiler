#pragma once
#include <exception>

enum class SematicError{ NO_ERRORS, VAR_NOT_DEFINED, TYPE_NOT_DEFINED };

namespace tiger{
class ExprNode;
}
SematicError semantic_checks(tiger::ExprNode * node);

class SemanticException : public std::exception{
public:
    SemanticException(SematicError err){
        err_type = err;
    }
    SematicError error(){ return err_type; }
protected:
    SematicError err_type;
};
