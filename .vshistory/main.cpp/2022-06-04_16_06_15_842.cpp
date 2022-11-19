#include <SFML\Graphics.hpp>
#include "constants.h"
#include "structures.h"
#include "functions.h"
#include "ship_functions.h"
#include <sstream>
#include <vector>
#include <deque>
#include <iostream>


// Handles main gain loop
int main() {
	sf::Shader space;
	if (!space.loadFromFile("space.frag", sf::Shader::Fragment)) {
		std::cout << "There was an error loading a fragment shader" << std::endl;
		return -1;
	}
	sf::View view(sf::Vector2f(1920, 1080), sf::Vector2f(1920, 1080));
	view.zoom(zoom);
	auto clock = sf::Clock{};
    sprites = createTexture("./Sprites/sprites.png");
	Ship* player = nullptr;;
	//Planet p(1500, 1500, 1500);
    float elapsedTime = 0.0f;
    float targetTime = .01f;
	bool accelerated = false;
	std::vector<Part> parts = initParts();

	std::vector<Ship*> ships = {};
	std::vector<Planet> planets = {};
	Bullet* bullets = nullptr; //Linked list of all projectiles
	
	int index = 0;
	int part = 0;
	bool in_shipyard = true;
	std::vector<Part> shipyard = {};
	for (int i = 0; i < 18 * 18; i++) {
		shipyard.push_back(parts[0]);
	}


	window.setFramerateLimit(61);
	float dX, dY, FPS;
	float mX, mY;
	window.setView(view);
	sf::RectangleShape shape(view.getSize());

    while (window.isOpen()) {
		FPS = 1.f / clock.getElapsedTime().asSeconds();

        elapsedTime += clock.getElapsedTime().asSeconds();
        clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::KeyReleased) {
				if (in_shipyard) {
					if (event.key.code == sf::Keyboard::R) {
						shipyard[index].rotation = (shipyard[index].rotation + 1) % 4;
						valid_design(shipyard);
					}
					else if (event.key.code == sf::Keyboard::Up) {
						//shipyard[index] = parts[safeC(shipyard[index].index + 1, parts.size())];
						part = safeC(part + 1, parts.size());

					}
					else if (event.key.code == sf::Keyboard::Down) {
						//shipyard[index] = parts[safeC(shipyard[index].index - 1, parts.size())];
						part = safeC(part - 1, parts.size());
					}
					if (event.key.code == sf::Keyboard::A) {
						index = safeC(index - 1, shipyard.size());
					}
					else if (event.key.code == sf::Keyboard::D) {
						index = safeC(index + 1, shipyard.size());
					}
					else if (event.key.code == sf::Keyboard::W) {
						index = safeC(index - 18, shipyard.size());
					}
					else if (event.key.code == sf::Keyboard::S) {
						index = safeC(index + 18, shipyard.size());
					}
					else if (event.key.code == sf::Keyboard::Space) {
						shipyard[index] = parts[part];
						valid_design(shipyard);
					}
					if (event.key.code == sf::Keyboard::Enter) {
						if (valid_design(shipyard)) {
							in_shipyard = false;
							Ship* ship = new Ship();
							for (int i = 0; i < shipyard.size(); i++) {
								shipyard[i].pos.x = (i % 18) * 16;
								shipyard[i].pos.y = (i / 18) * 16;
								if (shipyard[i].index > 0) {
									if (shipyard[i].crucial) {
										if (shipyard[i].name == "Station Control") {
											ship->station = true;
										}
									}
									ship->parts.push_back(shipyard[i]);
								}
							}
							ships.push_back(ship);
							if (player == nullptr) {
								player = ship;
							}
						}
					}
				}
				if (event.key.code == sf::Keyboard::P) {
					in_shipyard = true;
					shipyard = {};
					for (int i = 0; i < 18 * 18; i++) {
						shipyard.push_back(parts[0]);
					}
				}
				if (event.key.code == sf::Keyboard::L) {
					player->docking = !(player->docking);
				}
			}
			else if (event.type == sf::Event::MouseWheelMoved) {
				if (event.mouseWheel.delta < 0 && zoom < 3.0) {
					zoom += .05;
					view = sf::View(sf::Vector2f(1920, 1080), sf::Vector2f(1920, 1080));
					view.zoom(zoom);
					shape = sf::RectangleShape(view.getSize());
				}
				else if (event.mouseWheel.delta > 0 && zoom > .25){
					view = sf::View(sf::Vector2f(1920, 1080), sf::Vector2f(1920, 1080));
					zoom -= .05;
					view.zoom(zoom);
					shape = sf::RectangleShape(view.getSize());
				}
			}
        }
		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;
		if (player != nullptr) {
			mX = player->cx + sf::Mouse::getPosition().x * zoom - view.getSize().x / 2;
			mY = player->cy + sf::Mouse::getPosition().y * zoom - view.getSize().y / 2;
			if (elapsedTime >= targetTime) {
				accelerated = false;
				elapsedTime -= targetTime;
				timer = (timer + 1) % 1000;

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					up = true;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
					right = true;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					down = true;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					left = true;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
					for (Part& p : player->parts) {
						p.pos.dx = 0;
						p.pos.dy = 0;
						p.pos.da = 0;
					}
				}
				targetGuns(player, mX, mY);

				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					for (Part p : player->parts) {
						if (p.turret) {
							Position pos = p.pos;
							pos.a = p.turret_angle + rad(90);
							Bullet* b = new Bullet(pos);
							b->pos.x += cos(b->pos.a) * 6;
							b->pos.y += sin(b->pos.a) * 6;
							b->next = bullets;
							bullets = b;
						}
					}
				}
				Bullet* b = bullets;
				while (b != nullptr) {
					moveBullet(b);
					if (b->next != nullptr) {
						if (b->next->power <= 0) {
							Bullet* del = b->next;
							b->next = b->next->next;
							delete del;
						}
					}
					b = b->next;
				}
				for (Ship* s : ships) {
					if (s == player) {
						moveShip(s, up, down, left, right);
					}
					else {
						moveShip(s);
					}
					for (Planet p : planets) {
						applyGravity(player, p);
					}
				}
			}
		}
		
		if (!in_shipyard) {
			view.setCenter(player->cx, player->cy);
		}
		window.setView(view);
		window.clear();
		shape.setPosition(view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2);
		space.setUniform("offset", sf::Glsl::Vec2{ shape.getPosition() });
		window.draw(shape, &space);
		
		if (in_shipyard) {
			dX = 50 + view.getCenter().x - view.getSize().x / 2.0;
			dY = 50 + view.getCenter().y - view.getSize().y / 2.0;
			for (int i = 0; i < shipyard.size(); i++) {
				if (shipyard[i].index > 0) {
					sf::Color color = sf::Color(255, 255, 255);
					bool can_connect = false;
					//Check if piece can be placed this way
					if (!(shipyard[i].valid)) {
						color = sf::Color(255, 0, 0);
					}
					drawSprite(shipyard[i].sX + 16 * (shipyard[i].rotation % 2), shipyard[i].sY + 16 * (shipyard[i].rotation / 2), 16, 16, dX + (i % 18) * 16, dY + (i / 18) * 16, 1, 0.0f, color);
				}
				if (i == index) {
					drawSprite(64, 0, 16, 16, dX + (i % 18) * 16, dY + (i / 18) * 16);
				}
			}
			drawSprite(parts[part].sX, parts[part].sY, 16, 16, view.getCenter().x + view.getSize().x / 2.0 - 32, view.getCenter().y - view.getSize().y / 2 + 32);
		}
		else {
			for (Ship* s : ships) {
				drawShip(s);
				s->docked_at = nullptr;
				for (Ship* b : ships) {
					if (s != b) {
						checkCollision(s, b);
					}
				}
			}
			Bullet* b = bullets;
			while (b != nullptr) {
				drawBullet(b);
				b = b->next;
			}
			for (Planet p : planets) {
				drawPlanet(&p, view);
				drawFutureTrajectory(*player, planets);
			}
		}
		//Print(to_str((int)FPS), view.getCenter().x, view.getCenter().y);
		if (player != nullptr && !in_shipyard) {
			Print(to_str((int)player->cy), view.getCenter().x - 50, view.getCenter().y - view.getSize().y / 2.0 + 20);
			Print(to_str((int)player->cx), view.getCenter().x + 50, view.getCenter().y - view.getSize().y / 2.0 + 20);
			if (player->docked_at != nullptr) {
				Print("docked", view.getCenter().x, view.getCenter().y, 3);
			}
		}
        window.display();
    }

    return 0;
}