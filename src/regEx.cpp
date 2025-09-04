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
					return;
					
				case 'w':
					type = WORD;
					return;
					
				default:
					type = UNDEFINED;
					return;
			}
			
			return;
		
		case '.':
			type = ANY;
			return;
		
		case '[':
			type = GROUP_START;
			return;
			
		case ']':
			type = GROUP_END;
			return;
			
		case '(':
			type = EITHER_OR_START;
			return;
			
		case '|':
			type = EITHER_OR_MIDDLE;
			return;
		
		case ')':
			type = EITHER_OR_END;
			return;
		
		case '^':
			type = ANCHOR_START;
			return;
			
		case '$':
			type = ANCHOR_END;
			return;
		
		case '+':
			type = MATCH_ONE_OR_MORE;
			return;
			
		case '?':
			type = MATCH_ZERO_OR_ONE;
			return;
		
		case '*':
			type = MATCH_ZERO_OR_MORE;
			return;
			
		case '\0':
			type = END_OF_FILE;
			return;
		
		default:
			type = EXACT;
			return;
			
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