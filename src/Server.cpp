#include <iostream>
#include <string>

#include "regEx.h"


bool matchHere(const std::string& input_line, const std::vector<Expression>::iterator& currExp);


bool matchOOM(const std::string& input_line, const std::vector<Expression>::iterator& currExp){
	
	std::vector<Expression> subExp = {*currExp, Expression("\0")};
	std::vector<Expression>::iterator subIt = subExp.begin();
	
	int index = 0;
	
	while(matchHere(input_line.substr(index), subIt)){
		index++;
	}
	
	while(index != 0){ //mathHere returned true at least 1 time.
		index--;
		
		if (matchHere(input_line.substr(index + 1), currExp + 2))
			return 1;
	}
	
	return 0;
}

bool matchZOO(const std::string& input_line, const std::vector<Expression>::iterator& currExp){
	
	std::vector<Expression> subExp = {*currExp, Expression("\0")};
	std::vector<Expression>::iterator subIt = subExp.begin();
	
	int index = 0;
	
	if (matchHere(input_line, subIt)){
		return matchHere(input_line.substr(1), currExp + 2);
	}
	return matchHere(input_line, currExp + 2);
}


bool matchGroup(const std::string& input_line, const std::vector<Expression>::iterator& currExp){
	
	int index = 1;
	
	bool inverted = 0;
	
	if((*(currExp + 1)).typeString == "^"){
		inverted = 1;
		index++;
	}
	
	if(!inverted){
		while(((*(currExp + index)).type != Expression::GROUP_END) && ((*(currExp + index)).type != Expression::END_OF_FILE)){
			std::vector<Expression> subVec = {*(currExp + index), Expression("\0")};
			if(matchHere(input_line, subVec.begin())){
				return 1;
			}
			index++;
		}
		
	} else {
		while(((*(currExp + index)).type != Expression::GROUP_END) && ((*(currExp + index)).type != Expression::END_OF_FILE)){
			std::vector<Expression> subVec = {*(currExp + index), Expression("\0")};
			if(matchHere(input_line, subVec.begin())){
				return 0;
			}
			index++;
		}
		if(input_line != ""){
			return 1;
		}
	}
	
	return 0;
}

bool matchEitherOr(const std::string& input_line, const std::vector<Expression>::iterator& currExp){
	std::vector<Expression>::iterator start = currExp;
	std::vector<Expression>::iterator end = currExp;
	
	std::vector<Expression> scope = {*start};
	
	for(unsigned int depth = 0; ((*(end)).type != Expression::EITHER_OR_END) || depth != 0; end++){
		if(((*end).type == Expression::EITHER_OR_MIDDLE) && (depth == 0)){
			scope.push_back(*end);
		}
		
		if(((*end).type == Expression::END_OF_FILE) || ((*end).type == Expression::UNDEFINED)){
			return 0;
		}
	}
	
	scope.push_back(*end);
	
	for(unsigned int index = 0; index != scope.size(); index++){
	std::vector<Expression> subScope = std::vector<Expression>(scope[index] + 1, scope[index + 1] - 1);
	subScope.push_back(Expression("\0"));
	
		if (matchHere(input_line, subLeft.begin()) || matchHere(input_line, subRight.begin())){
			return 1;
		}
	}
	
	return 0;
	/* 
	Needs massive refactor. Current idea:
	int scopeDepth keeps track of the depth of the current scope. If we run into another (, it increases by 1.
	If we run into a ) and scopeDepth != 0, decrease scopeDepth by 1. Finally, if we reach ) and scopeDepth == 0,
	set end to equal the found ). Do the same for middle (|) operators, appending them to a vector if they are found at scopeDepth of 0.
	
	Alternatively, we could reverse-iterate over the vector, finding the outermost remaining scope, and only passing in the restricted scope
	as done here. This would require more overall iterations at maximum, and would not function for patterns such as "(dog | cat)(cat | mouse)" with 2 unique scopes.
	*/
}

bool matchHere(const std::string& input_line, const std::vector<Expression>::iterator& currExp){
	std::cout << "Entered function: matchHere with an input line of: " << input_line << "\n And a current expression of: " << (*currExp).typeString << "(" << (*currExp).type << ")" << std::endl;
	
	if ((*currExp).type == Expression::END_OF_FILE){
		return 1;
	}

	// Expression[1] == one or more.
	std::vector<Expression>::iterator nextExp = currExp + 1;
	switch((*nextExp).type){
		
		case Expression::MATCH_ONE_OR_MORE:
			return matchOOM(input_line, currExp);
		
		case Expression::MATCH_ZERO_OR_ONE:
			return matchZOO(input_line, currExp);
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
		
		case Expression::ANY:
		std::cout << "Expression::Any" << std::endl;
			if((input_line[0] != ' ') && (input_line != "")){
				std::cout << "If returned true." << std::endl;
				return matchHere(input_line.substr(1), currExp + 1);
			}
			
			break;
		
		case Expression::GROUP_START:
			return matchGroup(input_line, currExp);
				
			
		case Expression::EITHER_OR_START:
			return matchEitherOr(input_line, currExp);
			
		case Expression::ANCHOR_END:
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
		return matchHere(input_line, currExp + 1);
	}
	do {
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
