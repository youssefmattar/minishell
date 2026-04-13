#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "globalVars.hpp"
#include "strhelp.hpp"
#include "execute.hpp"


int main() {
    // 1. Create a window (Width, Height, Title)

    width = 800;
    height = 600;
    
    //sf::RenderWindow window(sf::VideoMode(width, height), "minishell");

    // 2. Load a font from a file
    sf::Font font;
    if (!font.loadFromFile("VT323-Regular.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }

    // 3. Create and configure the text object

    initial = "shell> ";
    displayString = initial;
    //in globalvrs
    text.setFont(font); 
    text.setString(displayString);
    charSize = 24;
    text.setCharacterSize(charSize); // in pixels
    text.setFillColor(sf::Color::Green);
    text.setStyle(sf::Text::Regular);
    
    // Center the text (optional)
    text.setPosition(1.f, 1.f);
    


    // 1. Setup the cursor shape
    //in golbalvrs
    cursor.setFillColor(sf::Color::White);
    sf::Vector2f cursorPOS = text.findCharacterPos(displayString.length());
    cursor.setPosition(cursorPOS);


    int scrollAmount = 0;

    std::string command="";

    initial = "shell:";
    initial += getTerminalPath();
    initial += ">";
    displayString = initial;
    displayString+= "c";//for cursor
    text.setString(displayString);
    displayString.pop_back();//delete old cursor

    //there is no -1 because text dont know that we removed the c from displayString 
    // so there is still 'c' in text

    cursor.setPosition(text.findCharacterPos(displayString.length()));

        if (pipe(pipe_to_child) == -1) {
            perror("pipe failed");
            exit(1);
        }
        

        if(pipe(pipe_to_parent)==-1){
            perror("pipe failed");
            exit(1);
        }
        fcntl(pipe_to_parent[0], F_SETFL, O_NONBLOCK);

    // 4. The Main Loop
    while (window.isOpen()) {
        for(int i = 0; i<childPids.size(); i++){//remove terminated child process from vector
            int status;
            pid_t result = waitpid(childPids[i], &status, WNOHANG); 

            if (result == 0) {
                // Child is still running! Proceed with SFML events/rendering.
            } else if (result > 0) {
                // Child just finished. Handle it here.
                
                std::cout<< "child terminated "<<childPids[i]<<std::endl;
                close(pipe_to_child[0]);
                close(pipe_to_child[1]);
                close(pipe_to_parent[0]);
                close(pipe_to_parent[1]);
                childPids.erase(childPids.begin() + i);
                if(childPids.size() < 1){//not to write initial more than one time
                    displayString+=initial;
                    displayString+="c";
                    text.setString(displayString);
                    displayString.pop_back();//delete old cursor
                    cursor.setPosition(text.findCharacterPos(displayString.length()));
                
                    
                }
                break;
            }
        }

        for(int i = 0; i<childPidsBackground.size(); i++){//read data from background processes
           
            // 1. Define the buffer and the control block

            char buffer[256];
            // read() will now return -1 immediately if there is no data, 
            // instead of waiting (blocking).
            int bytesRead = read(childPidsBackground[i].pipe_to_parent[0], buffer, sizeof(buffer) - 1);

            if (bytesRead > 0) {
                buffer[bytesRead] = '\0'; // Null-terminate the string
                displayString += buffer;  // Only append if we actually got something!
                displayString += initial;
            
                text.setString(displayString);
                cursor.setPosition(text.findCharacterPos(displayString.length()));
               
                //std::cout<<" readta" <<readdta<<std::endl;
            } 
        }

        for(int i = 0; i<childPidsBackground.size(); i++){//remove terminated child process from vector
            int status;
            pid_t result = waitpid(childPidsBackground[i].pid, &status, WNOHANG); 

            if (result == 0) {
                // Child is still running! Proceed with SFML events/rendering.
            } else if (result > 0) {
                // Child just finished. Handle it here.
                
                std::cout<< "background child terminated "<<childPidsBackground[i].pid<<std::endl;

                char buffer[2];
                // read() will now return -1 immediately if there is no data, 
                // instead of waiting (blocking).

                //finish unread data
                int bytesRead = 0;
                do{

                    bytesRead = read(childPidsBackground[i].pipe_to_parent[0], buffer, sizeof(buffer) - 1);
                    if(bytesRead > 0){
                        buffer[bytesRead] = '\0'; // Null-terminate the string
                        displayString += buffer;  // Only append if we actually got something!

                        cursor.setPosition(text.findCharacterPos(displayString.length()));
                    }
                    //std::cout<<" readta" <<readdta<<std::endl;
                
                }while(bytesRead > 0);

                
                displayString+=std::string("background child terminated ")+ std::to_string(childPidsBackground[i].pid);
                displayString += "\n";
                displayString += initial;
            
                text.setString(displayString);
                cursor.setPosition(text.findCharacterPos(displayString.length()));


                close(childPidsBackground[i].pipe_to_child[0]);
                close(childPidsBackground[i].pipe_to_child[1]);
                close(childPidsBackground[i].pipe_to_parent[0]);
                close(childPidsBackground[i].pipe_to_parent[1]);
                childPidsBackground.erase(childPidsBackground.begin() + i);
                
                
                break;
            }
        }

        /* read from the pipe */
        // 1. Define the buffer and the control block

        char buffer[128];
        // read() will now return -1 immediately if there is no data, 
        // instead of waiting (blocking).
        int bytesRead = read(pipe_to_parent[0], buffer, sizeof(buffer) - 1);

        if (bytesRead > 0) {
            buffer[bytesRead] = '\0'; // Null-terminate the string
            displayString += buffer;  // Only append if we actually got something!
            //std::cout<<" readta" <<readdta<<std::endl;
            displayString+="c";
            text.setString(displayString);
            displayString.pop_back();//delete old cursor
            cursor.setPosition(text.findCharacterPos(displayString.length()));
        }

        

        sf::Event event;
        
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed){
                window.close();
            }  
            
            if (event.type == sf::Event::Resized) {
                // 1. Create a new view with the new dimensions
                width = event.size.width;
                height = event.size.height;
                sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
                
                // 2. Apply the view to the window
                window.setView(sf::View(visibleArea));

                // 3. Optional: Log the new size for debugging
                std::cout << "Window resized to: " << event.size.width << "x" << event.size.height << std::endl;
            }

            //keyboard events
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                if (event.key.code == sf::Keyboard::Space) {
                    std::cout << "Space bar pressed once!" << std::endl;
                }

                if (event.key.code == sf::Keyboard::PageUp) {
                    if(scrollAmount>0){
                        scrollAmount-=10;
                    
                        sf::FloatRect visibleArea(0.f, scrollAmount, width, height);
                    
                        // 2. Apply the view to the window
                        window.setView(sf::View(visibleArea));

                        std::cout<<"scroll amount "<< scrollAmount<<std::endl;
                    }
                    
                   
                }
                else if (event.key.code == sf::Keyboard::PageDown) {
                    scrollAmount+=10;
                    sf::FloatRect visibleArea(0.f, scrollAmount, width, height);
                
                    // 2. Apply the view to the window
                    window.setView(sf::View(visibleArea));
                    std::cout<<"scroll amount "<< scrollAmount<<std::endl;
                }

                if (event.key.code == sf::Keyboard::C && event.key.control) {//kill signal
                    // Success! Ctrl + C was pressed.
                    if(childPids.size()>0){
                        kill(childPids[0], SIGTERM);
                    }

                    //handle processes that was in background
                    for(int i = 0; i<childPidsBackground.size(); i++){

                        if(childPidsBackground[i].inputEnable == 1){
                            kill(childPidsBackground[i].pid, SIGTERM);
                        }
                    }
                }

            }

            // Handle typing
            if (event.type == sf::Event::TextEntered) {
                // 1. Handle Backspace (Unicode 8)
                if (event.text.unicode == 8) {
                    int initial_pos = displayString.rfind(initial);
                    if (displayString.length() > initial_pos + initial.length()) { // Don't delete the "Type: " prefix
                        displayString.pop_back();
                    }
                }

                //2. handle pressing enter
                else if (event.text.unicode == 13 || event.text.unicode == 10) {
                    displayString += '\n'; 
                    command = getBetween(displayString, initial, "\n", initial.length());
                    if(childPids.size()==0){
                        parseExecute(command);
                    }
                    else if(childPids.size() == 1){
                        char nl = '\n';
                        write(pipe_to_child[1], &nl, 1);
                    }
                    //displayString+=initial;

                    
                }

                // 3. Handle Printable Characters (Unicode < 128 for standard ASCII)
                else if (event.text.unicode < 128) {
                    size_t lastNewlinePos = displayString.find_last_of('\n');

                    //dont let text go off screen
                    if (text.getGlobalBounds().width > width-30) {
                        displayString += "\n";         
                    }
                    
                    char c = static_cast<char>(event.text.unicode);
                    displayString += c;

                    // SEND THIS CHARACTER TO CHILD (Step 2)
                    if(childPids.size()>0){//dont add bad data to pipe
                        write(pipe_to_child[1], &c, 1);
                    }

                    //write input to background process if came back to foreground
                    for(int i = 0; i<childPidsBackground.size(); i++){

                        if(childPidsBackground[i].inputEnable == 1){
                            write(childPidsBackground[i].pipe_to_child[1], &c, 1);
                        }
                    }
                    
                     
                

                }

                



                // 3. Update the text object with the new string
                displayString+= "c";//for cursor
                text.setString(displayString);
                displayString.pop_back();//delete old cursor

                //there is no -1 because text dont know that we removed the c from displayString 
                // so there is still 'c' in text
                sf::Vector2f cursorPOS = text.findCharacterPos(displayString.length());
                cursor.setPosition(cursorPOS);
                std::cout<<"cursor: "<<cursorPOS.x << " " << cursorPOS.y<<std::endl;
                
            }

        }

        // 5. Rendering
        window.clear(sf::Color(48, 10, 36)); // Clear previous frame
        //window.draw(cursor);
        window.draw(text);               // Draw the text object
        window.draw(cursor);
        window.display();                // Show everything on screen
    }

    return 0;
}