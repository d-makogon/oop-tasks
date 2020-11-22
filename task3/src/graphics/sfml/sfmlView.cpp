#include "sfmlView.h"

#include <SFML/Graphics.hpp>

void bs::SFMLView::Do(bool revealPlayer1, bool revealPlayer2)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window", sf::Style::Close);
    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("image.jpg"))
        return;
    sf::Sprite sprite(texture);
    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
        return;
    sf::Text text("Hello SFML", font, 50);
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear();
        // Draw the sprite
        window.draw(sprite);
        // Draw the string
        window.draw(text);
        // Update the window
        window.display();
    }

}

bs::SFMLView::~SFMLView()
= default;
