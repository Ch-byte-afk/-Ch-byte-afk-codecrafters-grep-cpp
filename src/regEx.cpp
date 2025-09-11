#include "regEx.h"

#include <string>
#include <vector>

#include <iostream>


regEx::Expression::Expression(std::string expString) : typeString(expString) {
	
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
			type = ANY_OF;
			return;
			
		case '(':
			type = SCOPE;
			return;
			
		case '|':
			type = OR;
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
			
		default:
			type = EXACT;
			return;
			
	}
}

std::vector<regEx::Expression> regEx::parsePattern(const std::string& pattern){
	std::vector<Expression> expVec;
	
	std::string::const_iterator scopeStart = pattern.begin();
	std::string::const_iterator scopeEnd; // "Off the end" iterator for sub-strings.
	
	
	while(scopeStart != pattern.cend()){
		switch(*scopeStart){
			
			case '\\':
			
				if(scopeStart + 1 == pattern.cend()){
					throw std::runtime_error("Missing argument for expression \"\\\": " + pattern);
					return expVec;
				}
				
				scopeEnd = scopeStart + 2;
				
				expVec.push_back(Expression(std::string(scopeStart, scopeEnd)));
				scopeStart = scopeEnd;
				break;
			
			case '[':
				
				scopeEnd = scopeStart + 1;
				
				if (findScope(scopeStart, scopeEnd, pattern, '[', ']')){
						expVec.push_back(Expression(std::string(scopeStart, scopeEnd)));
				} else {
					throw std::runtime_error("Could not find matching ']' for '[': " + pattern);
					return expVec;
				}
				
				scopeStart = scopeEnd;
				break;
				
			case '(': // Not implimented.
				
				scopeEnd = scopeStart + 1;
				
				if (findScope(scopeStart, scopeEnd, pattern, '(', ')')){
						expVec.push_back(Expression(std::string(scopeStart, scopeEnd)));
				} else {
					throw std::runtime_error("Could not find matching ')' for '(': " + pattern);
					return expVec;
				}
				
				scopeStart = scopeEnd;
				break;
		
			default:
			
				scopeEnd = scopeStart + 1;
				expVec.push_back(Expression(std::string(scopeStart, scopeEnd)));
				scopeStart = scopeEnd;
				break;
		}	
	}
	
	return expVec;
}

bool regEx::findScope(std::string::const_iterator& scopeStart, std::string::const_iterator& scopeEnd, const std::string& pattern, const char& startChar, const char& endChar){
	int scopeDepth = 0;
	
	while(scopeEnd != pattern.cend()){
		if(*scopeEnd == endChar && scopeDepth == 0){
			++scopeEnd; //scopeEnd needs to point to one after the final character.
			std::cout << "Scope found: " << std::string(scopeStart, scopeEnd) << std::endl;
			return true;
		}
		
		if(*scopeEnd == startChar){
			++scopeDepth;
		} else if(*scopeEnd == endChar){
			--scopeDepth;
		}

		++scopeEnd;
		
	}
	
	return false;
}
	


bool regEx::matchExpression(std::vector<Expression>::const_iterator& exp, const std::vector<Expression>::const_iterator& end, std::string::const_iterator& match){
	if (exp + 1 != end){
		switch((*(exp + 1)).type){
			
			case Expression::MATCH_ONE_OR_MORE:
				std::cout << "One or More (+) operator found." << std::endl;
				return postfixOneOrMore(exp, end, match);
				
			case Expression::MATCH_ZERO_OR_ONE:
				std::cout << "Zero or One (?) operator found." << std::endl;
				return postfixZeroOrOne(exp, end, match);
			
			default:
				break;
		}
	} 
	
	
	switch((*exp).type){
		case Expression::EXACT:
			return (*exp).typeString[0] == *match;
			
		case Expression::DIGIT:
			return std::isdigit(*match);
			
		case Expression::WORD:
			return std::isalnum(*match) || *match == '_';
			
		case Expression::ANY:
			return (*match) != '\0';
			
		case Expression::ANY_OF:
			return matchAnyOf(*exp, match);
			
		case Expression::SCOPE:
			return matchScope(*exp, match);
			
		case Expression::ANCHOR_END:
			return exp + 1 == end;
			
		default:
			return false;
		
	}
	
}

bool regEx::matchHere(std::vector<Expression>::const_iterator& exp, const std::vector<Expression>::const_iterator& end, std::string::const_iterator& match){
	std::string::const_iterator textCurr = match;
	
	while(*textCurr != '\0' && exp != end){
		
		std::cout << "Matching expression: " << (*exp).typeString << " (" << (*exp).type << ")\n"
			<< "with starting character: " << *textCurr << std::endl;
		
		if (matchExpression(exp, end, textCurr)){
			
			std::cout << "Match. Continueing..." << std::endl;
			
			++exp;
			++textCurr;
			continue;
		}
		break;
	}
	
	if(exp == end){ //If end of scope was reached, success. All match.
	
		std::cout << "Full match. Match complete." << std::endl;
		
		match = textCurr;
		return true;
	}
	
	std::cout << "No match. Aborting..." << std::endl;
	
	return false;
}

bool regEx::matchPattern(const std::string& pattern, const std::string& text){
	const std::vector<Expression> expressions = parsePattern(pattern);
	std::vector<Expression>::const_iterator exp = expressions.cbegin();
	std::vector<Expression>::const_iterator end = expressions.cend();
	
	std::string::const_iterator textBegin = text.begin();
	std::string::const_iterator match = text.begin();
	
	
	if((*exp).type == Expression::ANCHOR_START){
		++exp;
		return matchHere(exp, end, match);
	}
	
	
	while(*textBegin!= '\0'){
	if (matchHere(exp, end, match))
		return true;
	
	if(*textBegin == '\0')
		return false;
	
	++textBegin;
	match = textBegin;
	exp = expressions.cbegin();
	}
	
	return false;
}


// Specialty match functions:

bool regEx::matchAnyOf(const Expression& group, std::string::const_iterator& match){
	// Starting index 1: directly after opening bracket. Length of size() - 2: length of full string excluding brackets.
	std::vector<Expression> possibleMatches = parsePattern(group.typeString.substr(1, group.typeString.size() - 2));
	
	for(std::vector<Expression>::const_iterator exp = possibleMatches.cbegin();
	exp != possibleMatches.cend(); ++exp){
		if(matchExpression(exp, possibleMatches.cend(), match)){
			return true;
		}
	}
	return false;
}

bool regEx::matchScope(const Expression& scope, std::string::const_iterator& match){
	// Starting index 1: directly after opening bracket. Length of size() - 2: length of full string excluding parenthesis.
	std::vector<Expression> fullScope = parsePattern(scope.typeString.substr(1, scope.typeString.size() - 2));
	std::vector<std::vector<Expression>::const_iterator> subScopes = {fullScope.begin()};
	
	for(std::vector<Expression>::const_iterator exp = fullScope.cbegin();
	exp != fullScope.cend(); ++exp){
		if((*exp).type == Expression::OR){ // Could be refactored to more easily tell if a type is a conditional.
			subScopes.push_back(exp);
		}
	}
	
	subScopes.push_back(fullScope.cend());
	
	// Does not function with future possible '&' conditional operator. If implimented, refactor.
	
	int index = 0;
	
	std::vector<Expression>::const_iterator currStart = subScopes[index];
	std::vector<Expression>::const_iterator currEnd;
	
	while(currEnd != fullScope.cend()){
		currEnd = subScopes[index + 1];
		
		std::cout << "Entered 308 While" << std::endl;
		
		if (matchHere(currStart, currEnd, match)){
			std::cout << "Scope match complete." << std::endl;
			return true;
		}
		
		++index;
		currStart = currEnd + 1;
		
	}
	return false;
}


// Postfix match functions:

bool regEx::postfixOneOrMore(std::vector<Expression>::const_iterator& exp, const std::vector<Expression>::const_iterator& end, std::string::const_iterator& match){
	
	std::vector<Expression>::const_iterator nextExp = exp + 2;
	std::string::const_iterator currMatch = match;
	
	while(*currMatch != '\0' && matchExpression(exp, exp + 1, currMatch)){
		++currMatch;
	}
	
	while(currMatch != match){ // At least one functional match found.
		--currMatch;
		
		
		
		if (matchHere(nextExp, end, currMatch)){
			match = currMatch;
			exp = end - 1;
			return true;
		}
	}
	
	return false;
	
	// God, this is so funky.I didn't want to use heavy recursion, but I just couldn't find an effective way around it with this one.
}

bool regEx::postfixZeroOrOne(std::vector<Expression>::const_iterator& exp, const std::vector<Expression>::const_iterator& end, std::string::const_iterator& match){
	
	std::vector<Expression>::const_iterator nextExp = exp + 2;
	std::string::const_iterator currMatch = match;
	
		std::cout << "End of scope given to ZeroOrOne: " << (*(end - 1)).typeString << std::endl;
	
	if (matchHere(nextExp, end, currMatch)){
		
		std::cout << "Found to be a zero-match. (ZeroOrMore)" << std::endl; 
		
		match = currMatch;
		exp = nextExp;
		return true;
	}
	
	
	if(nextExp != end && matchExpression(exp, exp + 1, currMatch))
		++currMatch;

		if (matchHere(nextExp, end, currMatch)){
			
			std::cout << "Found to be a one-match. (ZeroOrMore)" << std::endl;
			
			match = currMatch;
			exp = nextExp;
			return true;
		}
	}
	
	return false;
	
}