/*frame.hh
Largerly based off of the frame.h interface
from 'modern compiler implementation in c' by
andrew w. appel. Will be implemented in mipsframe.cpp*/
#pragma once
#include <memory>
#include <vector>
#include <string>
#include "temp.hh"
#include "irt.hh"
#include <stdlib.h>
//machine dependent word size
extern const int wordSize;

using formalsList = std::vector<bool>;

class Access_{
public:
    Access_(bool esc,int offset_);
    Access_(bool esc,ir::temp_t temp_);
    ~Access_() = default;
    ir::expPtr exp(ir::temp_t fp);
    ir::expPtr expStaticLink(ir::expPtr ptr);
private:
    bool escapes;
    int offset;
    ir::temp_t temp;
};

using Access = std::shared_ptr<Access_>;
using AccessList = std::vector<Access>;

class Frame_{
public:
    Frame_(ir::label_t name_, formalsList forms);
    ~Frame_() = default;
    Access allocLocal(bool escape);
    AccessList formals();
    void allocFormals(formalsList forms);
    std::string getlabel();
    ir::expPtr externalCall(std::string s, ir::expPtrList args);
    //ir::stm procEntryExit1(ir::stm stm);
	//Temp_tempList getRegisters();
	ir::temp_t getFP();
	Temp_map F_tempMap();
	//temp_t getSP();
	//temp_t getZERO();
	//temp_t getRA();
	//temp_t getRV();
private:
	ir::label_t name;
	AccessList fList;
	//Temp_map tempNames;
	int formalsOffset = 0;
	int frameOffset = 0;
	const int wordSize = 4;
	//Temp_tempList registers;
	ir::temp_t FP;
	//temp_t SP;
	//temp_t ZERO;
	//temp_t RA;
	//temp_t RV;

};

using Frame = std::shared_ptr<Frame_>;
Frame newFrame(ir::label_t name, formalsList formals);


class Frag_{
public:
    Frag_() = default;
    ~Frag_() = default;
};

using Frag = std::shared_ptr<Frag_>;
using FragList = std::vector<Frag>;

Frag newStringFrag(ir::label_t label, std::string str);
Frag newProcFrag(ir::stmPtr body, Frame frame);

class StringFrag_: public Frag_{
public:
    StringFrag_(ir::label_t label, std::string str);
    ~StringFrag_() = default;
private:
    ir::label_t label;
    std::string str;
};

class ProcFrag_: public Frag_{
public:
    ProcFrag_(ir::stmPtr body, Frame frame);
    ~ProcFrag_() = default;
private:
    ir::stmPtr body;
    Frame frame;
};

struct FuncFrag{
    Frame frame;
    ir::IRTptr body;
};
extern std::vector<FuncFrag> func_fragments;

struct StringFrag{
    std::string str_const;
    Temp_label str_label;
};
extern std::vector<StringFrag> str_fragments;
