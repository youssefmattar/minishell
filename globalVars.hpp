#ifndef _GLOBAL_VARIABLES_
#define _GLOBAL_VARIABLES_ //defined only once for whole program
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <queue>
#include <cstring>
#include <fcntl.h>  // File Control Definitions
#include <sys/wait.h>
#include <aio.h>
#include <sys/types.h>
#include <signal.h>

//global variables


enum class OutputType{terminal, pipe, fileOverWrite, fileAppend};
enum class InputType{terminal, pipe, file, background};
typedef struct Process{
    pid_t pid;
    std::vector<std::string> programAndArgs;
    std::string inputSource;  // Path to file if InputType is file
    std::string outputTarget; // Path to file if OutputType is file*
    OutputType out;
    InputType in;
    int pipe_to_child[2];//forbackground processes
    int pipe_to_parent[2];//forbackground processes
    bool running = 0;

}Process;

typedef struct ProcessBack{
    pid_t pid;
    bool inputEnable = 0;
    int pipe_to_child[2];//forbackground processes
    int pipe_to_parent[2];
    bool running = 0;
}ProcessBack;


    unsigned int width = 800;
    unsigned int height = 600;
    
    sf::RenderWindow window(sf::VideoMode(width, height), "minishell");
    unsigned short charSize = 24;
    std::string initial = "shell> ";
    std::string displayString = initial;
    sf::Text text;

    sf::RectangleShape cursor(sf::Vector2f(10.f, 26.f)); // Width of 2px, height of 26px

    std::vector<pid_t> childPids;//foreground
    std::vector<ProcessBack> childPidsBackground;
    std::vector<std::string> prevcomm;
    int prevcommIndex = 0;

    int pipe_to_child[2]; // Global: [0] is read, [1] is write
    int pipe_to_parent[2]; // Global: [0] is read, [1] is write



#endif