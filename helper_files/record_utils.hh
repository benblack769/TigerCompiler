#include <vector>
#include <unordered_set>

template<typename val_ty>
bool first_strs_unique(const std::vector<std::pair<std::string, val_ty>> & vals){
    using namespace std;
    unordered_set<string> keys;
    for(auto str_pair : vals){
        keys.insert(str_pair.first);
    }
    return keys.size() == vals.size();
}
