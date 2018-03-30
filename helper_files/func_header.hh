#pragma once
#include <vector>
#include <string>

//used to pass infromation about the function header to the symbol table
struct FuncHeader{
    bool has_return_type;
    std::string return_type_name;
    std::vector<std::string> arg_type_names;
};
