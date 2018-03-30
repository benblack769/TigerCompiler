#include <vector>
#include <string>

struct FuncHeader{
    bool has_return_type;
    std::string return_type_name;
    std::vector<std::string> arg_type_names;
};
