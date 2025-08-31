#include <iostream>
#include <string>


/* 
	Parse into individual pattern objects.
	Recursively run through handlePattern_MATCH untill end of pattern is reached indicating success, or end of input is reached indicating failure.


*/


bool handlePattern_MATCH(const std::string& input_line, const std::string& pattern){
	
	switch (pattern[1]){
		
		case 'd':
			return std::isdigit(input_line[0]);       // input_line.find_first_of("0123456789") != std::string::npos; // Return true if digit is present.
			
		case 'w':
			return std::isalnum(input_line[0]); // Return true if alpha-numeric character present. Inefficient.
			
		default:
			throw std::runtime_error("Unhandled pattern " + pattern);
	}
}

bool handlePattern_GROUP(const std::string& input_line, const std::string& pattern){
			
	unsigned int start = pattern.find_first_of('[') + 1;
	unsigned int end = pattern.find_last_of(']');
		
	if (end != std::string::npos) // If closing square bracket is found.
		--end;
	
	std::string group = pattern.substr(start, end);
	std::cout << group << std::endl;
		
	if(group[0] == '^'){ 
		return input_line.find_first_not_of(group) != std::string::npos;// Return true if any non-given characters are found.
	} else {
		return input_line.find_first_of(group) != std::string::npos; // Return true if any of the given characters are found.
	} 
}


bool matchHere(const std::string& input_line, const std::string& pattern){
		if (pattern.length() == 0){
		return 1;
	}
	 
	if (pattern[0] == '\\' && handlePattern_MATCH(input_line, pattern)){ // Step pattern by 2 to account for backslash (\) character.
		return matchHere(input_line.substr(1), pattern.substr(2));
	} 
	
	if (pattern[0] == '[' && handlePattern_GROUP(input_line, pattern)){
		return matchHere(input_line.substr(1), pattern.substr(pattern.find_last_of(']') + 1));
		
	}
	
	if (pattern[0] != '\\' && pattern[0] != '[' && pattern[0] == input_line[0]) { // Switch statement may help reduce complexity in the future.
		return matchHere(input_line.substr(1), pattern.substr(1));
	}
	
	return 0;
}

bool match_pattern(const std::string& input_line, const std::string& pattern) {
	
	int curIndex = 0;
	
	do{
		if (matchHere(input_line.substr(curIndex), pattern)) // Run once even if input_line is "". If pattern == "" and input_line == "", still returns true.
			return 1;
		
	} while (input_line[++curIndex] != '\0');
	
	return 0;
	
}

/*
bool handlePattern_MATCH(const std::string& input_line, const std::string& pattern){
	
	switch (pattern[1]){
		
		case 'd':
			return match_pattern(input_line.substr(1), pattern.substr(2));       // input_line.find_first_of("0123456789") != std::string::npos; // Return true if digit is present.
			
		case 'w':
			return input_line.find_first_of("_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") != std::string::npos; // Return true if alpha-numeric character present. Inefficient.
			
		default:
			throw std::runtime_error("Unhandled pattern " + pattern);
	}
}

bool handlePattern_GROUP(const std::string& input_line, const std::string& pattern){
			
	unsigned int start = pattern.find_first_of('[') + 1;
	unsigned int end = pattern.find_last_of(']');
		
	if (end != std::string::npos) // If closing square bracket is found.
		--end;
	
	std::string group = pattern.substr(start, end);
	std::cout << group << std::endl;
		
	if(group[0] == '^'){ 
		return input_line.find_first_not_of(group) != std::string::npos;// Return true if any non-given characters are found.
	} else {
		return input_line.find_first_of(group) != std::string::npos; // Return true if any of the given characters are found.
	} 
}





bool match_pattern(const std::string& input_line, const std::string& pattern) {
	
	if (pattern.length() == 1) {
        return match_pattern(                 // input_line.find(pattern) != std::string::npos;
	} 
	if (pattern[0] == '\\'){
		return handlePattern_MATCH(input_line, pattern);
	} 
	if (pattern[0] == '['){
		return handlePattern_GROUP(input_line, pattern);
		
	}
	
	return 0;
}
	
	
	

	
	
	if (pattern.length() == 1) {
        return input_line.find(pattern) != std::string::npos;
    }
    else {
        throw std::runtime_error("Unhandled pattern " + pattern);
    }
	
}
*/


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
    //
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
