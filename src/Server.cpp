#include <iostream>
#include <string>

#include "regEx.h"


bool matchHere(const std::string& input_line, const std::vector<Expression>::iterator& currExp);

bool matchOOM(const std::string& input_line, const std::vector<Expression>::iterator& currExp){
	
	std::vector<Expression> subExp = {*currExp, Expression("\0")};
	std::vector<Expression>::iterator subIt = subExp.begin();
	
	std::cout << "Entered OOM." << std::endl;
	
	int index = 0;
	
	while(matchHere(input_line.substr(index), subIt)){
		std::cout << "Entered Iteration Loop." << std::endl;
		index++;
	}
	
	while(index != 0){ //mathHere returned true at least 1 time.
		index--;
		std::cout << "Found valid index." << std::endl;
		
		if (matchHere(input_line.substr(index), currExp + 2))
			return 1;
	}
	
	return 0;
}

bool matchGroup(const std::string& input_line, const std::vector<Expression>::iterator& currExp){
	
	std::vector<Expression> subExp = {*(currExp + 1), Expression("\0")};
	std::vector<Expression>::iterator subIt = subExp.begin();
	
	bool inverted = 0;
	
	if((*currExp).typeString == "^"){
		inverted = 1;
		subIt++;
	}
	
	do{
		if (matchHere(input_line, subIt) != inverted){
			while ((*subIt).type != Expression::GROUP_END && (*(subIt + 1)).type != Expression::END_OF_FILE){
				subIt++;
			}
				
			return matchHere(input_line.substr(1), subIt + 1);
		}
		
	} while ((*(++subIt)).type != Expression::GROUP_END && (*(subIt + 1)).type != Expression::END_OF_FILE);
	
	return 0;
}


bool matchHere(const std::string& input_line, const std::vector<Expression>::iterator& currExp){
	if ((*currExp).type == Expression::END_OF_FILE){
		return 1;
	}

	// Expression[1] == one or more.
	std::vector<Expression>::iterator nextExp = currExp + 1;
	switch((*nextExp).type){
		
		case Expression::MATCH_ONE_OR_MORE:
			return matchOOM(input_line, currExp);
		
		case Expression::MATCH_ZERO_OR_ONE:
			break;
		
		case Expression::MATCH_ZERO_OR_MORE:
			break;
	
		default:
			break;
	}
	
	switch((*currExp).type){
		
		case Expression::EXACT:
			if((*currExp).typeString[0] == input_line[0]){
				return matchHere(input_line.substr(1), currExp + 1);
			}
			
			break;
			
		case Expression::DIGIT:
			if(std::isdigit(input_line[0])){
				return matchHere(input_line.substr(1), currExp + 1);
			}
			
			break;
		
		case Expression::WORD:
			if(std::isalnum(input_line[0]) || input_line[0] == '_'){
				return matchHere(input_line.substr(1), currExp + 1);
			}
			
			break;
		
		case Expression::GROUP_START:
			return matchGroup(input_line, currExp);
				
			
		case Expression::GROUP_END:
			return matchHere(input_line, currExp + 1);
			
		case Expression::ANCHOR_END:
			//
		
			if((*(currExp + 1)).type == Expression::END_OF_FILE){
				return input_line == "";
			}	
			
			break;
			
		case Expression::UNDEFINED:
			throw std::runtime_error("Unhandled pattern " + (*currExp).typeString);
			break;
			
	}
		
	return 0;	
}

bool match_pattern(const std::string& input_line, const std::string& pattern) {
	std::vector<Expression> expressions = parsePattern(pattern);
	
	int index = 0;
	std::vector<Expression>::iterator currExp = expressions.begin();
	
	if (expressions[0].type == Expression::ANCHOR_START){
		return matchHere(input_line, currExp++);
	}
	do {
		
		currExp = expressions.begin() + index;
		
		if (matchHere(input_line.substr(index), currExp)) // Run once even if input_line is "". If pattern == "" and input_line == "", still returns true.
			return 1;
		
	} while (input_line[++index] != '\0');
	
	return 0;
	
}




//Default code:

int main(int argc, char* argv[]) {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cerr << "Logs from your program will appear here" << std::endl;

    if (argc != 3) {
        std::cerr << "Expected two arguments" << std::endl;
        return 1;
    }

    std::string flag = argv[1];
    std::string pattern = argv[2];

    if (flag != "-E") {
        std::cerr << "Expected first argument to be '-E'" << std::endl;
        return 1;
    }

    // Uncomment this block to pass the first stage
    std::string input_line;
    std::getline(std::cin, input_line);
    
    try {
        if (match_pattern(input_line, pattern)) {
            return 0;
        } else {
            return 1;
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
