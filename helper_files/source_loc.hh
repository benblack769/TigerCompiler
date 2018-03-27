#pragma once
#include <ostream>

struct SourceLoc{
    int start_line = -1;
    int start_col = -1;
    int end_line = -1;
    int end_col = -1;
};

inline std::ostream & operator << (std::ostream & os, const SourceLoc & loc){
    if(loc.start_line == loc.end_line){
        return os << "line:" << loc.start_line;// << ", col: (" << loc.start_col << ", " << loc.end_col << ")";
    }
    else{
        return os << "from line:" << loc.start_line // ", col:" << loc.start_col
                        << " to " << loc.end_line;// << ", col:" << loc.end_col;
    }
}
