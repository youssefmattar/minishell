#ifndef STR_HELPERS
#define STR_HELPERS

#include<iostream>
#include <string>

std::string getBetween(const std::string& fullText, std::string startWord, std::string endWord, size_t stShift = 0) {
    
    // 1. Find where startWord begins
    size_t startPos = fullText.rfind(startWord);
    if (startPos == std::string::npos) {
        return ""; // startWord not found
    }

    // 2. Find the next newline AFTER startWord
    size_t endPos = fullText.find(endWord, startPos);
    
    // 3. Handle the case where there is no newline (take until the end of string)
    if (endPos == std::string::npos) {
        return fullText.substr(startPos);
    }

    // 4. Calculate length and extract
    size_t length = endPos - startPos;
    return fullText.substr(startPos+stShift, length);
}







#endif