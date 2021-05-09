#include <iostream>
#include <cmath>
#include <string>

std::string compress(const std::string& str) {
    size_t strSize = str.size();
    if(strSize == 0)
        return str;
    
    std::string result;
    
    char current = str[0];
    size_t inRow = 1u;
    for(size_t i = 1u; i<strSize; ++i) {
        if(str[i] == current)
            ++inRow;
        else {
            result += current;
            if(inRow > 1u)
                result += std::to_string(inRow);
            current = str[i];
            inRow = 1u;
        }
    }
    result += current;
    if(inRow > 1u)
        result += std::to_string(inRow);

    return result;
}

std::string decompress(const std::string& str) {
    if(str.size() == 0)
        return str;
    
    std::string result;
    char current = 0;
    size_t number = 0u;
    for(char c : str) {
        if(c >= '0' && c <= '9')
            number = 10*number + int(c - '0');
        else if(current != 0) {
            for(size_t i = 0u; i < (number>0 ? number : 1) ; ++i)
                result += current;
            current = c;
            number = 0u;
        }
        else 
            current = c;
        
    }
    for(size_t i = 0u; i < (number>0 ? number : 1) ; ++i)
        result += current;
    return result;
}

int main() {
    //Testing code
    for(int i = 0; i<1000; ++i) {
        std::string test;
        for(int x = 0; x< 7; ++x) {
            char c = 'a' + rand()%20;
            for(int y = 0; y<rand()%20; ++y)
                test += c;
        }
        std::string res = decompress(compress(test));
        std::cout << (res==test ? "    " : "BAD ") << test << "\t\t\t" << decompress(compress(test)) << '\n';
    }
    
    return 0;
}
