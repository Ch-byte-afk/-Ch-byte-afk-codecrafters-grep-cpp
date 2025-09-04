#include "regEx.h"

#include <string>
#include <vector>

#include <iostream>


Expression::Expression(std::string expString) : typeString(expString) {
	
	switch(expString[0]){
	
		case '\\':
			switch (expString[1]){ // Only a switch statement for the sake of expandability.
		
				case 'd':
					type = DIGIT;
					break;
					
				case 'w':
					type = WORD;
					break;
					
				default:
					type = UNDEFINED;
					break;
			}
			
			break;
		
		case '.':
			type = ANY;
			break;
		
		case '[':
			type = GROUP_START;
			break;
			
		case ']':
			type = GROUP_END;
			break;
		
		case '^':
			type = ANCHOR_START;
			break;
			
		case '$':
			type = ANCHOR_END;
			break;
		
		case '+':
			type = MATCH_ONE_OR_MORE;
			break;
			
		case '?':
			type = MATCH_ZERO_OR_ONE;
			break;
		
		case '*':
			type = MATCH_ZERO_OR_MORE;
			break;
			
		case '\0':
			type = END_OF_FILE;
			break;
		
		default:
			type = EXACT;
			break;
			
	}
}


std::vector<Expression> parsePattern(const std::string& pattern){
	std::vector<Expression> expVec;
	
	
	for(int index = 0; index <= pattern.size(); index++){
		if(pattern[index] == '\\'){
			expVec.push_back(Expression(pattern.substr(index, 2)));
			index++;
		} else {
			expVec.push_back(Expression(pattern.substr(index, 1)));
		}
	}
	
	return expVec;
}