// temp.cc
// Brandon Finley

#include "temp.hh"
#include <string>
using namespace std;

int temp_var_count = 0;
int temp_label_count = 0;

constexpr int uninitialized_temp_num = -1;

 /* list initializer */
<<<<<<< HEAD
Temp_temp::Temp_temp() {
	// int num = ?
	// const std::string specialName
	// static int count =?

};

Temp_temp::~Temp_temp()	{
	
=======
Temp_temp::Temp_temp(int temp) {
	num = temp;
};
Temp_temp::Temp_temp() {
   num = uninitialized_temp_num;
};

Temp_temp::~Temp_temp()	{

};

std::string
Temp_temp::toString() const{
	return to_string(num);
}

Temp_temp
newtemp() {
	int old_count = temp_var_count;
	temp_var_count++;
	return Temp_temp(old_count);
>>>>>>> 496b2e6a6eee7a88af26b3e540b9937f4c6a0129
};

Temp_label::Temp_label(const std::string &in_name) {
	name = in_name;
};

Temp_label::Temp_label(int in_num) {
	name = "L"+to_string(in_num);
};
Temp_label::Temp_label(){
	name = "uninitialized label";
}

Temp_label::~Temp_label(){

};
std::string Temp_label::toString() const{
	return name;
}

Temp_label newlabel(){
	int old_count = temp_label_count;
	temp_label_count++;
	return Temp_label(old_count);
}

Temp_label
newnamedlabel(std::string s){
	return Temp_label(s);
}
