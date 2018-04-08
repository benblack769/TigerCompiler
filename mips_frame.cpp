#include <stdlib.h>
#include <temp.hh>

namespace frame {

	struct F_frame_{
		Temp_label name;
		F_formalsList formals;
	}F_frame_;

	struct F_access_{
		enum {inFrame, inReg} kind;
		union {
			int offset;
			Temp_temp reg;
		}u;
	}F_access_;

	struct F_frag_{
		enum {F_stringFrag, F_procFrag} kind;
		union {
			struct {Temp_label label; string str;} stringg;
			struct {T_stm body; F_frame frame;} proc;
		}u;
	}F_frag_;

	F_frame F_newFrame(Temp_label name, F_formalsList formals){
		F_frame retFrame (new F_frame_);
		retFrame->name = name;
		retFrame->formals = formals;
		return retFrame;
	}
	F_access F_allocLocal(F_frame f, bool escape){
		F_access retAccess (new F_access_);
		retAccess->kind = F_access_::inFrame;
		retAccess->u.int = 0;
		return retAccess;
	};
	F_accessList F_formals(F_frame f){
		std::vector<F_access> retAccessList {};
		return retAccessList;
	};

	std::string F_getlabel(F_frame frame){
		std::string retStr = "";
		return retStr;
	};
	/*
	Temp_map F_tempMap{

	};
	Temp_tempList F_registers(void){

	};
	T_exp F_Exp(F_access acc, T_exp framePtr){

	};
	Temp_label F_name(F_frame f){

	};
	Temp_temp F_FP(void){

	};
	Temp_temp F_SP(void){

	};
	Temp_temp F_ZERO(void){

	};
	Temp_temp F_RA(void){

	};
	Temp_temp F_RV(void){

	};
	T_exp F_externalCall(string s, T_expList args){

	};
	*/
	F_frag F_StringFrag(Temp_label label, string str){
		F_frag retStrFrag (new F_frag_);
		retStrFrag->kind = F_Frag_::F_stringFrag;
		retStrFrag->u.stringg = {label,str};
		return retStrFrag;
	};
	F_frag F_ProcFrag(T_stm body, F_frame frame){
		F_frag retProcFrag (new F_frag_);
		retProcFrag->kind = F_Frag_::F_procFrag;
		retProcFrag->u.proc = {body, fame};
		return retProcFrag;
	};
}