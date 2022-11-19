#include <SFML\Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "SP Game");
    sf::Clock clock;
    sf::Texture texture;
    texture.loadFromFile("./Sprites/sprites.png");
    sf::IntRect spriteSource(0, 16, 16, 32);
    sf::Sprite sprite(texture, spriteSource);
    sprite.setScale(10, 10);
    float elapsedTime = 0.0f;
    float targetTime = .1f;
    int timer = 0;

    while (window.isOpen()) {
        elapsedTime += clock.getElapsedTime().asSeconds();
        clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (elapsedTime >= targetTime) {
            elapsedTime -= targetTime;
            timer = (timer + 1) % 33;
        }
        sprite.setTextureRect(sf::IntRect(8*16, 16 + (timer % 6) * 32, 16, 32));

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}