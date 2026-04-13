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
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

int pipefds[2];//for |
void executeSeparatedJobs(std::vector<Process> jobs){

    for(int i = 0; i<jobs.size(); i++){
        

            pid_t pid;
            pid_t pid2;
            // 1. Create a temporary vector to hold the pointers
            std::vector<char*> c_args;

            // 2. Fill it with the c_str() pointers from your strings
            for (auto& s : jobs[i].programAndArgs) {
                c_args.push_back(const_cast<char*>(s.c_str()));
            }

            // 3. Add the mandatory NULL terminator
            c_args.push_back(nullptr);

            if(jobs[i].out != OutputType::pipe){
                // --- PARENT DEBUG BLOCK ---
                std::cout << "\n--- FORKING NEW CHILD ---" << std::endl;
                std::cout << "Command: " << c_args[0] << std::endl;
                std::cout << "Input Type (Enum): " << (int)jobs[i].in << std::endl;
                std::cout << "Output Type (Enum): " << (int)jobs[i].out << std::endl;

                // Verify pipes are valid (should be small positive integers like 3, 4, 5...)
                std::cout << "Pipe to Child FDs:  [" << pipe_to_child[0] << ", " << pipe_to_child[1] << "]" << std::endl;
                std::cout << "Pipe to Parent FDs: [" << pipe_to_parent[0] << ", " << pipe_to_parent[1] << "]" << std::endl;
                // --------------------------

                pid = fork();
                if(pid == 0){ //child

                    if(jobs[i].out == OutputType::fileOverWrite){
                        int fd = open(jobs[i].outputTarget.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                            std::cout<<"output type fileOverwrite\n";
                        dup2(fd, STDOUT_FILENO); // Redirect stdout to the file
                        close(fd);               // Don't need the extra copy of the fd anymore
                    }else if(jobs[i].out == OutputType::fileAppend){
                        std::cout<<"output type fileApp\n";
                        int fd = open(jobs[i].outputTarget.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);

                        dup2(fd, STDOUT_FILENO); // Redirect stdout to the file
                        close(fd);               // Don't need the extra copy of the fd anymore
                    }else if(jobs[i].out == OutputType::terminal){
                        std::cout<<"output type terminal\n";
                        dup2(pipe_to_parent[1], STDOUT_FILENO);
                    }

                    if(jobs[i].in == InputType::terminal){
                        //std::cout<<"input type terminal\n";
                        dup2(pipe_to_child[0], STDIN_FILENO);
                    }
                    else if(jobs[i].in == InputType::file){
                        //std::cout<<"input type file\n";
                        int fd = open(jobs[i].inputSource.c_str(), O_RDONLY);

                        dup2(fd, STDIN_FILENO);  // Redirect stdin to come from the file
                        close(fd);
                    }
                    //close(pipe_to_child[0]);
                    close(pipe_to_child[1]);
                    close(pipe_to_parent[0]);
                    //close(pipe_to_parent[1]);
                    // 4. Use .data() to get the char** that execvp wants
                    int err = execvp(c_args[0], c_args.data());
                    if (err){
                        std:: cout<< "exec failed "<< strerror(errno)<<std::endl;
                        _exit(1);
                    }
                }
                else if(pid<0){//error
                    std::cout<<"error";
                }
                else { /* parent process */
                   
                    std::cout<<"parent\n";
                    close(pipe_to_child[0]);//close read pipe end
                    close(pipe_to_parent[1]);//close write pipe ends
                    childPids.push_back(pid);
                }
            } 
            else  if(jobs[i].out == OutputType::pipe && jobs[i+1].in == InputType::pipe){
                pipe(pipefds);
                // 1. Create a temporary vector to hold the pointers
                std::vector<char*> c_args2;

                // 2. Fill it with the c_str() pointers from your strings
                for (auto& s : jobs[i+1].programAndArgs) {
                    c_args2.push_back(const_cast<char*>(s.c_str()));
                }

                // 3. Add the mandatory NULL terminator
                c_args2.push_back(nullptr);

                pid = fork();
                if(pid == 0){//child 1 process
                    if(jobs[i].in == InputType::terminal){
                            //std::cout<<"input type terminal\n";
                        dup2(pipe_to_child[0], STDIN_FILENO);
                        //close(pipe_to_child[1]);
                    }
                    else if(jobs[i].in == InputType::file){
                        //std::cout<<"input type file\n";
                        int fd = open(jobs[i].inputSource.c_str(), O_RDONLY);

                        dup2(fd, STDIN_FILENO);  // Redirect stdin to come from the file
                        close(fd);
                    }

                    // Child 1: Write side (e.g., "ls")
                    dup2(pipefds[1], STDOUT_FILENO); // stdout -> pipe write
                    close(pipefds[0]); // Close unused read end
                    close(pipefds[1]); 

                    close(pipe_to_child[1]);
                    close(pipe_to_parent[1]);
                    //close(pipe_to_parent[1]);
                    // 4. Use .data() to get the char** that execvp wants
                    int err = execvp(c_args[0], c_args.data());
                    if (err){
                        std:: cout<< "exec failed "<< strerror(errno)<<std::endl;
                        _exit(1);

                    }


                }
                else if(pid < 0){//error

                }
                else{//parent
                    std::cout<<"parent\n";
                    close(pipe_to_child[0]);//close read pipe end
                    //close(pipe_to_parent[1]);//close write pipe ends
                    //close(pipefds[1]);
                    childPids.push_back(pid);
                }
                i++;
                pid2 = fork();

                if(pid2 == 0){//child 2
                    if(jobs[i].out == OutputType::fileOverWrite){
                        int fd = open(jobs[i].outputTarget.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                            std::cout<<"output type fileOverwrite\n";
                        dup2(fd, STDOUT_FILENO); // Redirect stdout to the file
                        close(fd);               // Don't need the extra copy of the fd anymore
                    }else if(jobs[i].out == OutputType::fileAppend){
                        std::cout<<"output type fileApp\n";
                        int fd = open(jobs[i].outputTarget.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);

                        dup2(fd, STDOUT_FILENO); // Redirect stdout to the file
                        close(fd);               // Don't need the extra copy of the fd anymore
                    }else if(jobs[i].out == OutputType::terminal){
                        std::cout<<"output type terminal\n";
                        dup2(pipe_to_parent[1], STDOUT_FILENO);
                    }
                    close(pipe_to_child[0]);
                    close(pipe_to_parent[0]);

                    dup2(pipefds[0], STDIN_FILENO);  // stdin <- pipe read
                    close(pipefds[1]); // Close unused write end
                    close(pipefds[0]);


                    int err = execvp(c_args2[0], c_args2.data());
                     if (err){
                        std:: cout<< "exec failed "<< strerror(errno)<<std::endl;
                        _exit(1);
                        
                    }
                }
                else if(pid2<0){//error

                }
                else{//parent
                    std::cout<<"parent\n"; 
                    close(pipefds[0]); 
                    close(pipefds[1]);
                    close(pipe_to_child[0]);
                    close(pipe_to_parent[1]);//close write pipe ends
                    childPids.push_back(pid2);
                }

                
                

            
            }   
        
       
    }
}


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
 
    if(commandf.length()>1){
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
            while(ss>>part){//tokens
                tokens.push_back(part);
            }

            executeSeparatedJobs(separateJobs(tokens));





        }
    }else{
        std::cout<<"invalid command\n";
        initial = "shell:";
        initial += getTerminalPath();
        initial += ">";
        displayString += initial;
    }    

    

    initial = "shell:";
    initial += getTerminalPath();
    initial += ">";

    return 0;

}






#endif