#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // 1. Create a window (Width, Height, Title)

    unsigned int width = 800;
    unsigned int height = 600;
    sf::RenderWindow window(sf::VideoMode(width, height), "minishell");

    // 2. Load a font from a file
    sf::Font font;
    if (!font.loadFromFile("VT323-Regular.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }

    // 3. Create and configure the text object

    std::string displayString = "shell> ";
    sf::Text text;
    text.setFont(font); 
    text.setString(displayString);
    unsigned short charSize = 24;
    text.setCharacterSize(charSize); // in pixels
    text.setFillColor(sf::Color::Green);
    text.setStyle(sf::Text::Regular);
    
    // Center the text (optional)
    text.setPosition(1.f, 1.f);
    


    // 1. Setup the cursor shape
    sf::RectangleShape cursor(sf::Vector2f(10.f, 26.f)); // Width of 2px, height of 26px
    cursor.setFillColor(sf::Color::White);
    sf::Vector2f cursorPOS = text.findCharacterPos(displayString.length()-1);
    cursor.setPosition(cursorPOS);


    int scrollAmount = 0;
    // 4. The Main Loop
    while (window.isOpen()) {
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
                        scrollAmount-=4;
                    
                        sf::FloatRect visibleArea(0.f, scrollAmount, width, height);
                    
                        // 2. Apply the view to the window
                        window.setView(sf::View(visibleArea));

                        std::cout<<"scroll amount "<< scrollAmount<<std::endl;
                    }
                    
                   
                }
                else if (event.key.code == sf::Keyboard::PageDown) {
                    scrollAmount+=4;
                    sf::FloatRect visibleArea(0.f, scrollAmount, width, height);
                
                    // 2. Apply the view to the window
                    window.setView(sf::View(visibleArea));
                    std::cout<<"scroll amount "<< scrollAmount<<std::endl;
                }


            }

            // Handle typing
            if (event.type == sf::Event::TextEntered) {
                // 1. Handle Backspace (Unicode 8)
                if (event.text.unicode == 8) {
                    if (displayString.length() > 7) { // Don't delete the "Type: " prefix
                        displayString.pop_back();
                    }
                }

                //2. handle pressing enter
                else if (event.text.unicode == 13 || event.text.unicode == 10) {
                    displayString.pop_back();//delete old cursor
                    displayString += '\n';
                    displayString+= "c";//for cursor
                    text.setString(displayString);
                    
                }

                // 3. Handle Printable Characters (Unicode < 128 for standard ASCII)
                else if (event.text.unicode < 128) {
                    displayString.pop_back();//delete old cursor
                    displayString += static_cast<char>(event.text.unicode);
                    displayString+= "c";

                }



                // 3. Update the text object with the new string
                text.setString(displayString);

                sf::Vector2f cursorPOS = text.findCharacterPos(displayString.length()-1);
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