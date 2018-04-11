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
    Access_() = default;
    ~Access_() = default;
    virtual std::string toString();
    virtual ir::exp* F_Exp(ir::exp* fp);
};

using Access = std::shared_ptr<Access_>;
using AccessList = std::vector<Access>;

class InFrame: public Access_{
public:
    InFrame(int offset);
    ~InFrame() = default;
private:
    int offset;
};

class InReg: public Access_{
public:
    InReg(ir::Temp* t);
    ~InReg() = default;
private:
    ir::Temp* temp;
};

class Frame_{
public:
    Frame_(ir::label_t name, formalsList formals);
    ~Frame_() = default;
    Access allocLocal(bool escape);
    AccessList formals();
    std::string getlabel();
    //ir::expPtr externalCall(string s, ir::expPtrList args);
    //ir::stm procEntryExit1(ir::stm stm);
	//Temp_tempList getRegisters();
	ir::temp_t getFP();
	//temp_t getSP();
	//temp_t getZERO();
	//temp_t getRA();
	//temp_t getRV();
private:
	ir::label_t f_name;
	formalsList f_formals;
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
/*
Frag newStringFrag(ir::label_t label, std::string str);
Frag newProcFrag(ir::stm body, Frame frame);

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
    ProcFrag_(ir::stm body, Frame frame);
    ~ProcFrag_() = default;
private:
    ir::stm body;
    Frame frame;
};
*/