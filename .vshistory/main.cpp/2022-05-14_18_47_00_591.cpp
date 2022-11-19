#include <SFML\Graphics.hpp>

sf::Texture createTexture(std::string file) {
    sf::Texture texture;
    texture.loadFromFile(file);
    return texture;
}

void drawSprite(sf::RenderWindow& window, sf::Texture tex, int sX, int sY, int w, int h, int dX, int dY, int scale = 1) {
    sf::IntRect rect(sX, sY, w, h);
    sf::Sprite sprite(tex, rect);
    sprite.setScale(scale, scale);
    sprite.setPosition(dX, dY);
    window.draw(sprite);
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
        drawSprite(window, sprites, 8 * 16, 16 + (timer % 6) * 32, 16, 32, 100, 10, 10);
        drawSprite(window, sprites, 8 * 16, 16 + (timer % 6) * 32, 16, 32, 100, 100, 10);
        window.display();
    }

    return 0;
}