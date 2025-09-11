#include <iostream>
#include <string>

#include "regEx.h"



int main(){
	
	
	std::string pattern = "x?[\\d\\w]+(2|(3|1))+2\\w";
	
	std::string text = "a123";
	
	//std::vector<regEx::Expression> scope = regEx::parsePattern(pattern);
	
	//std::cout << scope[0].typeString << " " << scope[0].type << std::endl;
	
	
	if (regEx::matchPattern(pattern, text)){
		std::cout << "Match found." << std::endl;
	} else {
		std::cout << "No match found." << std::endl;
	}
	
	
	
	return 0;
}