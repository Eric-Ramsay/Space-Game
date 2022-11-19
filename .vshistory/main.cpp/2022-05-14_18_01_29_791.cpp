#include <SFML\Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "SP Game");

    sf::Texture texture;
    texture.loadFromFile("./Sprites/sprites.png");
    sf::Sprite sprite(texture, sf::IntRect(0, 32, 16, 32));
    sprite.SetScale(10, 10);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}