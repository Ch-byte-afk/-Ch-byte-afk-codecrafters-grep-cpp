#include <iostream>
#include <string>

bool handlePattern_MATCH(const std::string& input_line, const std::string& pattern){
	
	switch (pattern[1]){
		
		case 'd':
			return input_line.find_first_of("0123456789") != std::string::npos; // Return true if digit is present.
			
		case 'w':
			return input_line.find_first_of("_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") != std::string::npos; // Return true if alpha-numeric character present. Inefficient.
			
		default:
			throw std::runtime_error("Unhandled pattern " + pattern);
	}
}

bool handlePattern_GROUP(const std::string& input_line, const std::string& pattern){
			
	unsigned int start = pattern.find_first_of('[') + 1;
	unsigned int end = pattern.find_last_of(']');
		
	std::string group = pattern.substr(start, end);
		
	if (end != std::string::npos) // If closing square bracket is found.
		--end;
		
	std::cout << group << std::endl;
		
	if(group[0] = '^'){ 
		return input_line.find_first_not_of(group) != std::string::npos;// Return true if any non-given characters are found.
	} else {
		return input_line.find_first_of(group) != std::string::npos; // Return true if any of the given characters are found.
	} 
}





bool match_pattern(const std::string& input_line, const std::string& pattern) {
	/*if(pattern == "\\d"){
		return input_line.find_first_of("0123456789") != std::string::npos; // Return true if digit is present.
	} else {
		return input_line.find(pattern) != std::string::npos;
	}*/
	
	if (pattern.length() == 0){
		throw std::runtime_error("Pattern cannot be blank.");
		
	} else if (pattern.length() == 1) {
        return input_line.find(pattern) != std::string::npos;
		
	} else if( pattern[0] == '\\'){
		return handlePattern_MATCH(input_line, pattern);

	} else if(pattern[0] == '['){
		return handlePattern_GROUP(input_line, pattern);
		
	} else {
		throw std::runtime_error("Unhandled pattern " + pattern);
	}
	
	
	

	
	/*
	if (pattern.length() == 1) {
        return input_line.find(pattern) != std::string::npos;
    }
    else {
        throw std::runtime_error("Unhandled pattern " + pattern);
    }
	*/
}

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
