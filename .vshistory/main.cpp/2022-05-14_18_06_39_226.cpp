#include <SFML\Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "SP Game");
    sf::Clock clock;
    sf::Texture texture;
    texture.loadFromFile("./Sprites/sprites.png");
    sf::Sprite sprite(texture, sf::IntRect(0, 32, 16, 32));
    sprite.setScale(10, 10);
    float elapsedTime = 0.0f;

    while (window.isOpen()) {
        elapsedTime += clock.getElapsedTime().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (clock.getElapsedTime().asSeconds() > .1f) {
            timer
            clock.restart();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}