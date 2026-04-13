#ifndef _GLOBAL_VARIABLES_
#define _GLOBAL_VARIABLES_
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
    unsigned int width = 800;
    unsigned int height = 600;
    sf::RenderWindow window(sf::VideoMode(width, height), "minishell");
    unsigned short charSize = 24;
    std::string initial = "shell> ";
    std::string displayString = initial;
    sf::Text text;

    sf::RectangleShape cursor(sf::Vector2f(10.f, 26.f)); // Width of 2px, height of 26px

    std::vector<pid_t> childPids;

    int pipe_to_child[2]; // Global: [0] is read, [1] is write
    int pipe_to_parent[2]; // Global: [0] is read, [1] is write



#endif