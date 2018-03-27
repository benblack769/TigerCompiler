#pragma once
#include <vector>
#include <unordered_map>
#include <utility>

using namespace std;

template<typename key_ty, typename value_ty>
struct ManyToOne{
    using id_ty = int;
    id_ty type_num = 0;
    vector<value_ty> typevals;
    unordered_map<key_ty, id_ty> types;
    bool point_to_same(key_ty k1, key_ty k2){
        return types.at(k1) == types.at(k2);
    }
    void overwrite_with_value(key_ty key, value_ty val){
        id_ty cur_type = type_num;
        typevals.push_back(move(val));
        type_num += 1;
        types[key] = cur_type;
    }
    value_ty & get(key_ty key){
        return typevals.at(types.at(key));
    }
    bool has_key(key_ty k){
        for(auto item : types){
        //    cout << "item: " << item.first << endl;
        }
        return types.count(k);
    }
    void overwrite_with_link(key_ty new_key, key_ty old_key){
        types[new_key] = types.at(old_key);
    }
    id_ty key_id(key_ty k){
        return types.at(k);
    }
};
