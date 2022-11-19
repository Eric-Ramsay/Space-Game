#include <SFML\Graphics.hpp>

sf::Texture createTexture(std::string file) {
    sf::Texture texture;
    texture.loadFromFile(file);
    return texture;
}

void drawSprite(sf::RenderWindow window, sf::Texture t, int sX, int sY, int w, int h, int dX, int dY, int scale) {
    sf::IntRect(sX, sY, w, h);
    sf::Sprite()
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SP Game", sf::Style::Fullscreen);
    sf::Clock clock;
    sf::Texture sprites = createTexture("./Sprites/sprites.png");
    
    sf::IntRect spriteSource(0, 16, 16, 32);
    sf::Sprite sprite(sprites, spriteSource);
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
            timer = (timer + 1) % 1000;
        }
        sprite.setTextureRect(sf::IntRect(8*16, 16 + (timer % 6) * 32, 16, 32));

        window.clear();
        window.draw(sprite);
        sprite.setTextureRect(sf::IntRect(7 * 16, 16 + (timer % 6) * 32, 16, 32));
        sprite.setPosition(100, 200);
        window.draw(sprite);
        window.display();
    }

    return 0;
}