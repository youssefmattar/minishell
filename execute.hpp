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
    int pid;
    std::vector<std::string> programAndArgs;
    std::string inputSource;  // Path to file if InputType is file
    std::string outputTarget; // Path to file if OutputType is file*
    OutputType out;
    InputType in;
}Process;





std::vector<Process> separateJobs(std::vector<std::string> tokens){
    
    
    std::vector<Process> job;
    
    
    Process p;
    p.in = InputType::terminal; // Default
    p.out = OutputType::terminal; //default
    for(int i = 0; i< tokens.size(); i++){
        if(tokens[i]=="<"){
            p.in = InputType::file;
            p.inputSource = tokens[++i];
        }
        else if(tokens[i]== ">"){
            p.out = OutputType::fileOverWrite;
            p.outputTarget = tokens[++i];
        }
        else if(tokens[i]== ">>"){
            p.out = OutputType::fileAppend;
            p.outputTarget = tokens[++i];
        }
        else if(tokens[i]== "|"){
            p.out = OutputType::pipe;
            job.push_back(p);
            
            // Reset for the next process in the pipe chain
            p = Process(); 
            p.in = InputType::pipe;
        }
        else {
            p.programAndArgs.push_back(tokens[i]);
        }
        
    }
    job.push_back(p); // Push the last (or only) process


    return job;

    
    
    

}



int parseExecute(std::string commandf){
 
    std::stringstream ss(commandf); // Initialize with the string
    std::string part;
    
    
    

    int i = 0;

    ss>>part;



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

    }
    else if(part == "jobs"&& i == 0){

    }
    else if(part == "fg" && i == 0){

    }
    else{
        
        //parse arguments untill there is nothing or there is > >> < or |
        std::vector<std::string> tokens;
        tokens.push_back(part);//command name
        while(ss>>part){//command arguments and other commands after pipe
            tokens.push_back(part);
        }



        /*
        if(part == "|"){

        }
        else if(part == ">"){

        }
        else if(part == ">>"){

        }
        else if(part == "<"){
            
        }
        else if(part == "&"){

        }
        */



    }

    

    initial = "shell:";
    initial += getTerminalPath();
    initial += ">";

    return 0;

}






#endif