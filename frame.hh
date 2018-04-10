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
//machine dependent word size
extern const int F_wordSize;

class F_frame_;
class F_access_;
class F_frag_;
//Frame struct
using F_frame = std::shared_ptr<F_frame_>;
using F_access = std::shared_ptr<F_access_>;
//accesses/formals interface
using F_formalsList = std::vector<bool>;
using F_access = std::shared_ptr<F_access_>;

using F_accessList = std::vector<F_access>;
F_frame F_newFrame(Temp_label name, F_formalsList formals);
F_access F_allocLocal(F_frame f, bool escape);
F_accessList F_formals(F_frame f);

string F_getlabel(F_frame frame);

//========Temps interface=================

extern Temp_map F_tempMap;
Temp_tempList F_registers(void);
ir::expPtr F_Exp(F_access acc, ir::expPtr framePtr);
Temp_label F_name(F_frame f);
Temp_temp F_FP(void);
Temp_temp F_SP(void);
Temp_temp F_ZERO(void);
Temp_temp F_RA(void);
Temp_temp F_RV(void);

//Misc T_exps
ir::expPtr F_externalCall(string s, ir::expPtrList args);
//T_stm F_procEntryExit1(F_frame frame, T_stm stm);

//========fragement interface for strings and function(procedures)
using  F_frag = std::shared_ptr<F_frag_>;
F_frag F_StringFrag(Temp_label label, string str);
F_frag F_ProcFrag(ir::stmPtr body, F_frame frame);

//F_frag F_string (Temp_label lab, string str);
//F_frag F_newProcFrag(T_stem body, F_frame frame);
using F_fragList = std::vector<F_frag>;
