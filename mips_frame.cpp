#include <stdlib.h>
#include "frame.hh"

Frame newFrame(ir::label_t name, formalsList forms){
	Frame retFrame (new Frame_(name,forms));
	return retFrame;
}

Frame_::Frame_(ir::label_t name_, formalsList forms){
	name = name_;
	fList = forms;
	FP = newtemp();
}
//pg 139 for more info
Access Frame_::allocLocal(bool escape){
	if(escape){
		Access retAccess (new Access_(escape,frameOffset));
		frameOffset += wordSize;
		return retAccess;
	}else{
		Access retAccess (new Access_(escape,newtemp()));
		return retAccess;
	}
};

Access_::Access_(bool esc,int offset_){
	escapes = esc;
	offset = offset_;
}

Access_::Access_(bool esc,ir::temp_t temp_){
	escapes = esc;
	temp = temp_;
}

AccessList Frame_::formals(){
	AccessList retAccessList {};
	return retAccessList;
};

std::string Frame_::getlabel(){
	std::string retStr = "";
	return retStr;
};

ir::temp_t Frame_::getFP(){
	return FP;
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
ir::exp Frame_::externalCall(string s, ir::expPtrList args){
	return nullptr;
};
*/

ir::expPtr Access_::exp(ir::temp_t fp){
	if(escapes){
		auto tempFP = to_expPtr(ir::Temp(fp));
		auto tempOffset = to_expPtr(ir::Const(offset));
		auto tempBinOp = to_expPtr(ir::BinOp(ir::op_k::PLUS,tempFP,tempOffset));
		return to_expPtr(ir::Mem(tempBinOp));
	}else{
		return to_expPtr(ir::Temp(temp));
	}
}

ir::expPtr Access_::expStaticLink(ir::expPtr ptr){
	auto tempOffset = to_expPtr(ir::Const(offset));
	auto tempBinOp = to_expPtr(ir::BinOp(ir::op_k::PLUS,ptr,tempOffset));
	return to_expPtr(ir::Mem(tempBinOp));

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