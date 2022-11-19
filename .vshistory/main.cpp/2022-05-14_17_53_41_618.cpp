#include <SFML\Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(512, 512), "SP Game");
    sf::RectangleShape player(sf::Vector2f(100.0f, 150.0f));
    player.setPosition(206, 206);
    sf::Texture playerTexture;
    playerTexture.loadFromFile("./Sprites/sprites.png");
    player.setTexture(&playerTexture);

    sf::Vector2u textureSize = playerTexture.getSize();
    textureSize.x /= 16;
    textureSize.y /= 32;

    player.setTextureRect(sf::IntRect(textureSize.x * 1, textureSize.y * 1, textureSize.x, textureSize.y));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(player);
        window.display();
    }

    return 0;
}