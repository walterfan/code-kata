#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <sstream>


std::string trim(const std::string& str, const std::string& whitespace )
{
    std::string::size_type strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    std::string::size_type strEnd = str.find_last_not_of(whitespace);
    std::string::size_type strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

void tokenize(const std::string& input, std::vector<std::string>& tokens) {
    bool is_in_quotation = false;
    bool is_delimiter = false;
    size_t input_length = input.length();
    std::ostringstream oss;
    size_t checked_chars = 0;
    for(size_t i = 0 ; i < input_length; ++i) {
        char c = input[i];
        oss << c;
        if(c == ' ' || c == ',') {
            checked_chars ++;
            if (!is_delimiter && !is_in_quotation) {
                auto token = oss.str();
                tokens.push_back(trim(token, " ,\""));
                oss = std::ostringstream();
                checked_chars += token.length();
            }
            is_delimiter = true;
        } else {
            is_delimiter = false;
        }

        if (c == '\"') {
            checked_chars ++;
            if (!is_in_quotation) {
                is_in_quotation = true;
            } else {
                is_in_quotation = false;
            }    
        } 

    }

    if (checked_chars < input_length) {
        tokens.push_back(trim(input.substr(checked_chars, input_length), " ,\""));
    }
}

void regex_test(const std::string& input) {
    // Regular expression pattern to match key-value pairs
    std::regex pattern("(\\w+)=?\"?([^\\s\"\\]+)\"?"); // Matches key=value or key="value" pairs

    // Iterator for searching through the input string
    std::sregex_iterator it(input.begin(), input.end(), pattern);
    std::sregex_iterator end;

    // Loop through matched key-value pairs
    while (it != end) {
        // Access the matched key-value pair
        std::smatch match = *it;

        // Extract the key and value
        std::string key = match.str(1);
        std::string value = match.str(2);

        // Print key-value pair
        std::cout << "Key=" << key << ", Value=" << value << std::endl;

        // Move to the next match
        ++it;
    }

}

int kata14_tokenize(int argc, char** argv) {
    std::vector<std::string> tokens;
    // Updated input string
    std::string input = "name key1=value1 key2=\"value2\" key3=\"value=3\", key4=123 ";
    tokenize(input, tokens);
    std::cout << "-------" << input << std::endl;
    for(auto& token: tokens) {
        std::cout << "token: " << token << std::endl;
    }

    tokens.clear();
    std::string caps="capsfilter caps=\"video/x-raw,width=640,height=480,framerate=30/1\"";
    std::cout << "-------" << caps << std::endl;
    tokenize(caps, tokens);

    for(auto& token: tokens) {
        std::cout << "token: " << token << std::endl;
    }
    return 0;
}
