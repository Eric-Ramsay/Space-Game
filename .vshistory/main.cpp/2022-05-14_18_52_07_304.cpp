#include <SFML\Graphics.hpp>

//Global Variables - Yeah Yeah
//These are here to limit parameter list sizes
sf::RenderWindow window(sf::VideoMode(1920, 1080), "SP Game", sf::Style::Fullscreen);
sf::Texture sprites;
int timer = 0;
int zoom = 10;

sf::Texture createTexture(std::string file) {
    sf::Texture texture;
    texture.loadFromFile(file);
    return texture;
}

void drawSprite(int sX, int sY, int w, int h, int dX, int dY, int scale = 1) {
    sf::IntRect rect(sX, sY, w, h);
    sf::Sprite sprite(sprites, rect);
    sprite.setScale(scale, scale);
    sprite.setPosition(dX, dY);
    window.draw(sprite);
}

int main() {
    sf::Clock clock;
    sprites = createTexture("./Sprites/sprites.png");

    float elapsedTime = 0.0f;
    float targetTime = .1f;

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

        window.clear();
        drawSprite(8 * 16, 16 + (timer % 6) * 32, 16, 32, 10, 10, 10);
        drawSprite(8 * 16, 16 + (timer % 6) * 32, 16, 32, 100, 200, 10);
        window.display();
    }

    return 0;
}