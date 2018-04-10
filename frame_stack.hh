#pragma once
#include <vector>
#include "frame.hh"

class FrameStack{
public:
    void new_frame(F_frame frame){
        frames.push_back(frame);
    }
    F_frame current_frame(){
        return frames.back();
    }
    void pop_frame(){
        frames.pop_back();
    }
    F_frame frame_at_level(int level){
        return frames.at(level-1);
    }
    int current_level(){
        return frames.size();
    }
protected:
    std::vector<F_frame> frames;
};
extern FrameStack full_frame;
