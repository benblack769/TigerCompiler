// temp.cc
// Brandon Finley

#include "temp.hh"


 /* list initializer */
Temp_temp::Temp_temp() {

};

Temp_temp::~Temp_temp()	{

};

std::string
Temp_temp::toString() const{
	return specialName;
}

Temp_temp
newtemp() {
	return Temp_temp();
};

Temp_label::Temp_label(const std::string &name) {

};

Temp_label::Temp_label() {

};

Temp_label::~Temp_label(){

};

Temp_label 
newlabel(){
	return Temp_label("");
}

Temp_label 
newnamedlabel(std::string s){
	return Temp_label(s);
}


