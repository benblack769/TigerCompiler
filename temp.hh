// temp.h
// Brandon Finley
// Noodles around with Temps and Labels.
// data representations inspired by ktok07b6 @ github

#include <string>
#include <vector>

class Temp_temp {
	public:
		Temp_temp();
		Temp_temp(/**/);
		~Temp_temp();
	private:
		int num;
		const std::string &specialName;
		static int count;

};

// methods on Temp_temp:
Temp_temp newtemp();
	// call constructor

/*	//////////////	*/

class Temp_label {
	public:
		Temp_label(const std::string &name);
		~Temp_label();
		const std::string toString() const;
	private:
		std::string name;
		int count;
};

// methods on Temp_label:
Temp_label newlabel();
	// call constructor
Temp_label namedlabel(string);
	// call constructor

class Temp_tempList{
	public:
		// Todo
	private:
		std::vector<Temp_temp*> tempList;
};