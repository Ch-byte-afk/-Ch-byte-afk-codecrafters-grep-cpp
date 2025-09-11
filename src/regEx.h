#pragma once

#include <string>
#include <vector>

namespace regEx{
	
class Expression {
public:
	explicit Expression(std::string expString);
	
	enum expTypes {
		UNDEFINED = -1,
		EXACT,
		DIGIT,
		WORD,
		ANY,
		ANY_OF,
		SCOPE,
		OR,
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
bool findScope(std::string::const_iterator& scopeStart, std::string::const_iterator& scopeEnd, const std::string& pattern, const char& startChar, const char& endChar);

bool matchExpression(std::vector<Expression>::const_iterator& exp, const std::vector<Expression>::const_iterator& end, std::string::const_iterator& match);
bool matchPattern(const std::string& pattern, const std::string& text);
bool matchHere(std::vector<Expression>::const_iterator& exp, const std::vector<Expression>::const_iterator& end, std::string::const_iterator& match);
bool matchAnyOf(const Expression& group, std::string::const_iterator& match);
bool matchScope(const Expression& scope, std::string::const_iterator& match);

//bool conditionAND(const std::vector<Expression>& scope, const std::vector<Expression>::const_iterator& andPos);
//bool conditionOR(const std::vector<Expression>& scope, const std::vector<Expression>::const_iterator& orPos);
//bool conditionXOR(const std::vector<Expression>& scope, const std::vector<Expression>::const_iterator& xorPos);

bool postfixOneOrMore(std::vector<Expression>::const_iterator& exp, const std::vector<Expression>::const_iterator& end, std::string::const_iterator& match);
bool postfixZeroOrOne(std::vector<Expression>::const_iterator& exp, const std::vector<Expression>::const_iterator& end, std::string::const_iterator& match);
//bool postfixZeroOrMore(std::vector<Expression>::const_iterator& exp, const std::vector<Expression>::const_iterator& end, std::string::const_iterator& match);


} // regEx