#ifndef _HEADER_EXECUTE_
#define _HEADER_EXECUTE_
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <queue>
#include <vector>
#include <cstring>

std::string getTerminalPath() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) return "error";

    std::string path(cwd);
    const char* homeEnv = getenv("HOME");
    
    if (homeEnv != NULL) {
        std::string home(homeEnv);
        
        // Check if the current path starts with the home directory
        if (path.find(home) == 0) {
            // Replace home path with '~'
            // Example: /home/user/docs -> ~/docs
            path.replace(0, home.length(), "~");
        }
    }
    
    return path;
}


enum class OutputType{terminal, pipe, fileOverWrite, fileAppend};
enum class InputType{terminal, pipe, file};
typedef struct Process{
    std::vector<std::string> programAndArgs;
    std::string inputSource;  // Path to file if InputType is file
    std::string outputTarget; // Path to file if OutputType is file*
    OutputType out;
    InputType in;
};

/*std::string singleCommandexecuter(std::vector commandAndArgsParam, int returnValue){
    
}*/



int parseExecute(std::string commandf){
 
    std::stringstream ss(commandf); // Initialize with the string
    std::string part;
    
    //std::vector<std::string> programAndArgs;
    

    int i = 0;

    while(ss>>part){



        if(part=="pwd" && i == 0){
            char buffer[PATH_MAX];

            if (getcwd(buffer, PATH_MAX) != NULL) {
                std::cout<<"Current  directory: "<< buffer<<std::endl;
                displayString+= " current directory: ";
                displayString+= buffer;
                displayString+="\n";

            } else {
                displayString+= "getcwd() error";
            }

            displayString+= "c";//for cursor
            text.setString(displayString);
            displayString.pop_back();//delete old cursor

            //there is no -1 because text dont know that we removed the c from displayString 
            // so there is still 'c' in text
            /*
            sf::Vector2f cursorPOS = text.findCharacterPos(displayString.length());
            cursor.setPosition(cursorPOS);
            std::cout<<"cursor: "<<cursorPOS.x << " " << cursorPOS.y<<std::endl;*/
        }
        else if(part == "cd"&& i == 0){
            ss>>part;
            std::string targetPath = part;
            // 1. Handle the "~" shortcut manually
            if (targetPath == "~" || targetPath.empty()) {
                targetPath = std::string(getenv("HOME"));
            }

            // 2. Attempt to change directory
            if (chdir(targetPath.c_str()) == 0) {
                displayString += "Directory changed successfully.\n";
            } else {
                // 3. Report the specific error (e.g., Folder doesn't exist)
                displayString += "cd: ";
                displayString += std::string(strerror(errno));
                displayString += "\n";
            }
            displayString+= "c";//for cursor
            text.setString(displayString);
            displayString.pop_back();//delete old cursor

            //there is no -1 because text dont know that we removed the c from displayString 
            // so there is still 'c' in text
            /*sf::Vector2f cursorPOS = text.findCharacterPos(displayString.length());
            cursor.setPosition(cursorPOS);
            std::cout<<"cursor: "<<cursorPOS.x << " " << cursorPOS.y<<std::endl;*/

        }
        else if(part == "jobs"&& i == 0){

        }
        else if(part == "fg" && i == 0){

        }
        else{
           

            if(part == "&"){

            }
            else if(part == "|"){

            }
            else if(part == ">"){

            }
            else if(part == ">>"){

            }
            else if(part == "<"){
                
            }
            else{

            }
            
        }
        i++;
    }
    

    initial = "shell:";
    initial += getTerminalPath();
    initial += ">";

    return 0;

}






#endif