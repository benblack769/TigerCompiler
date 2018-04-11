#include <stdlib.h>
#include "frame.hh"

Frame newFrame(ir::label_t name, formalsList forms){
	Frame retFrame (new Frame_(name,forms));
	return retFrame;
}

Frame_::Frame_(ir::label_t name, formalsList formals){
	f_name = name;
	f_formals = formals;
}
//pg 139 for more info
Access Frame_::allocLocal(bool escape){
	Access retAccess (new InFrame(0));
	return retAccess;
};
AccessList Frame_::formals(){
	AccessList retAccessList {};
	return retAccessList;
};

std::string Frame_::getlabel(){
	std::string retStr = "";
	return retStr;
};

ir::temp_t Frame_::getFP(){
	return nullptr;
};
/*
Temp_map Frame_::F_tempMap(){
	return nullptr;
};

Temp_tempList Frame_::getRegisters(){
	return nullptr;
};
Temp_temp Frame_::getSP(){
	return nullptr;
};
Temp_temp Frame_::getZERO(){
	return nullptr;
};
Temp_temp Frame_::getRA(){
	return nullptr;
};
Temp_temp Frame_::getRV(){
	return nullptr;
};
ir::expPtr Frame_::externalCall(string s, ir::expPtrList args){
	return nullptr;
};
*/

InFrame::InFrame(int offset_){
	offset = offset_;
}

InReg::InReg(ir::Temp* t){
	temp = t;
}
/*
Frag newStringFrag(Temp_label label, std::string str){
	Frag retStrFrag (new StringFrag_(label,str));
	return retStrFrag;
};

Frag newProcFrag(ir::stm body, Frame frame){
	Frag retProcFrag (new ProFrag_(body, frame));
	return retProcFrag;
};

StringFrag_::StringFrag_(ir::label_t label_, std::string str_){
	label = label_;
	str = str_;
}

ProcFrag_::ProcFrag_(ir::stm body_, Frame frame_){
	body = body_;
	frame = frame_;
}
*/