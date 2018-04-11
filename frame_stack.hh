#pragma once
#include <vector>
#include "frame.hh"

class FrameStack{
public:
    void new_frame(Frame frame){
        frames.push_back(frame);
    }
    Frame current_frame(){
        return frames.back();
    }
    void pop_frame(){
        frames.pop_back();
    }
    int current_level(){
        return frames.size();
    }
protected:
    std::vector<Frame> frames;
};
extern FrameStack full_frame;
