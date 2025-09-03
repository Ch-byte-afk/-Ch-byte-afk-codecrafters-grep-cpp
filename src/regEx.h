#pragma once

#include <string>
#include <vector>


class Expression {
public:
	explicit Expression(std::string expString);
	
	enum expTypes {
		UNDEFINED = -1,
		EXACT,
		DIGIT,
		WORD,
		GROUP_START,
		GROUP_END,
		ANCHOR_START,
		ANCHOR_END,
		MATCH_ONE_OR_MORE,
		MATCH_ZERO_OR_ONE,
		MATCH_ZERO_OR_MORE,
		END_OF_FILE
	};
	
	int type;
	std::string typeString;
	
	/*
	Room for expansion:
	Allow inverted types. (/D, /W, etc.)
	Implicit equivalency to Char, *Char, and String.
	*/
	
};


std::vector<Expression> parsePattern(const std::string& pattern);