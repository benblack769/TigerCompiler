#pragma once
#include <exception>
#include <cassert>
#include <sstream>
#include "helper_files/source_loc.hh"

enum class SematicError{
    NO_ERRORS,
    TYPE_NOT_DEFINED,
    CYCLICLY_DEFINED_TYPES,
    BAD_TYPE_MATCH,
    TWO_NAMES_IN_MUTU_RECURSIVE_ENV,
    NON_UNIQUE_RECORD_LABELS,
    INCOMPATABLE_RECORD_LABEL,
    FUNCTION_NOT_DEFINED,
    VARIABLE_NOT_DEFINED,
    USING_VAR_AS_FUNC,
    USING_FUNC_AS_VAR,
};

inline std::string error_str(SematicError err){
    switch(err){
    case SematicError::NO_ERRORS: return "valid semantics";
    case SematicError::TYPE_NOT_DEFINED: return "variable not defined";
    case SematicError::CYCLICLY_DEFINED_TYPES: return "variable not defined";
    case SematicError::BAD_TYPE_MATCH: return "variable not defined";
    case SematicError::TWO_NAMES_IN_MUTU_RECURSIVE_ENV: return "variable not defined";
    case SematicError::NON_UNIQUE_RECORD_LABELS: return "variable not defined";
    case SematicError::FUNCTION_NOT_DEFINED: return "variable not defined";
    case SematicError::VARIABLE_NOT_DEFINED: return "variable not defined";
    case SematicError::USING_VAR_AS_FUNC: return "variable not defined";
    case SematicError::USING_FUNC_AS_VAR: return "variable not defined";
    case SematicError::INCOMPATABLE_RECORD_LABEL: return "variable not defined";
    }
    assert("error not defined");
    return "error not defined";
}

namespace tiger{
class ExprNode;
}
SematicError semantic_checks(tiger::ExprNode * node);

class SemanticException : public std::exception{
public:
    SemanticException(SematicError err, SourceLoc loc=SourceLoc()){
        err_type = err;
        err_location = loc;
        std::stringstream sstr;
        sstr << "Error found in " <<  err_location << ":\n" << error_str(err);
        err_str = sstr.str();
    }
    SematicError error_code(){ return err_type; }
    SourceLoc error_loc(){ return err_location; }
    const char * what() const throw()
    {
        return err_str.c_str();
    }
protected:
    SematicError err_type;
    SourceLoc err_location;
    std::string err_str;
};
