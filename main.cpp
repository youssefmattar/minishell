#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // 1. Create a window (Width, Height, Title)
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Text Example");

    // 2. Load a font from a file
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }

    // 3. Create and configure the text object
    sf::Text text;
    text.setFont(font); 
    text.setString("Hello, SFML!");
    text.setCharacterSize(24); // in pixels
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    
    // Center the text (optional)
    text.setPosition(300.f, 250.f);

    // 4. The Main Loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // 5. Rendering
        window.clear(sf::Color::Black); // Clear previous frame
        window.draw(text);               // Draw the text object
        window.display();                // Show everything on screen
    }

    return 0;
}