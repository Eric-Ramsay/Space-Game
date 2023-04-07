#include <SFML\Graphics.hpp>
#include <vector>
#include <deque>
#include <sstream>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>

#include "constants.h"
#include "algos.h"
#include "structures.h"
#include "globals.h"
#include "functions.h"


#include "ship_functions.h"
#include "draw.h"

// Handles main game loop
int main() {
	window.setVerticalSyncEnabled(true);
	srand(time(NULL));
	sf::Shader space;
	if (!space.loadFromFile("space.frag", sf::Shader::Fragment)) {
		std::cout << "There was an error loading a fragment shader" << std::endl;
		return -1;
	}
	sf::View view(sf::Vector2f(1920, 1080), sf::Vector2f(1920, 1080));
	view.zoom(zoom);
	auto clock = sf::Clock{};

	float bdy = 0;
	float bdx = 0;

	Ship* player = nullptr;;
	float elapsedTime = 0.0f;
	float targetTime = .01f;
	bool accelerated = false;
	parts = initParts();
	Planet plan(1500, 1500, 1500);
	std::vector<Ship*> ships = {};
	std::vector<Explosion> explosions = {};
	std::vector<Planet> planets = {};
	float min = 9999999999999999;
	Position star_track_POS;
	long x; long y;
	/*for (int i = 0; i < 200; i++) {
		do {
			x = 4 * (rand() % 32000) - 64000;
			y = 4 * (rand() % 32000) - 64000;
		} while (abs(x) < 5000 && abs(y) < 5000);
		std::cout << x << " " << y << std::endl << std::endl;
		//planets.push_back(Planet(x, y, 128 + rand() % 4000));
	}
	for (Planet p : planets) {
		if (dist(p.x, p.y, 0, 0) < min) {
			min = dist(x, y, 0, 0);
			star_track_POS.x = p.x;
			star_track_POS.y = p.y;
		}
	}*/
	
	Bullet* bullets = nullptr; //Linked list of all projectiles

	int index = 0;
	int part = 0;
	bool in_shipyard = true;
	std::vector<Part> shipyard = {};
	for (int i = 0; i < 18 * 18; i++) {
		shipyard.push_back(Part());
	}

	window.setFramerateLimit(60);
	float dX, dY, FPS;
	float mX = 0; float mY = 0;
	sf::RectangleShape shape(view.getSize());
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool inGame = true;
	while (window.isOpen()) {
		if (!in_shipyard && player != nullptr) {
			view.setCenter(player->pos.x, player->pos.y);
		}
		window.setView(view);
		window.clear();
		shape.setPosition(view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2);
		sf::Vector2<float> shipVar(view.getCenter().x * .025, view.getCenter().y * .025);
		space.setUniform("offset", sf::Glsl::Vec2({shipVar}));
		window.draw(shape, &space);

		FPS = 1.f / clock.getElapsedTime().asSeconds();

		elapsedTime += clock.getElapsedTime().asSeconds();
		clock.restart();

		sf::Event event;
		if (inGame) {
			mX = view.getCenter().x - view.getSize().x / 2 + sf::Mouse::getPosition().x * zoom;
			mY = view.getCenter().y - view.getSize().y / 2 + sf::Mouse::getPosition().y * zoom;
			if (!in_shipyard) {
				up = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
				down = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
				left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
				right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
			}
		}
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::LostFocus) {
				inGame = false;
			}
			else if (event.type == sf::Event::GainedFocus) {
				inGame = true;
			}
			else if (event.type == sf::Event::KeyReleased) {
				if (in_shipyard) {
					if (event.key.code == sf::Keyboard::R) {
						shipyard[index].rotation = (shipyard[index].rotation + 1) % 4;
						valid_design(shipyard);
					}
					else if (event.key.code == sf::Keyboard::Up) {
						part = safeC(part + 1, parts.size());
					}
					else if (event.key.code == sf::Keyboard::Down) {
						part = safeC(part - 1, parts.size());
					}
					else if (event.key.code == sf::Keyboard::W) {
						index = safeC(index - 18, shipyard.size());
					}
					if (event.key.code == sf::Keyboard::A) {
						index = safeC(index - 1, shipyard.size());
					}
					else if (event.key.code == sf::Keyboard::S) {
						index = safeC(index + 18, shipyard.size());
					}
					else if (event.key.code == sf::Keyboard::D) {
						index = safeC(index + 1, shipyard.size());
					}
					else if (event.key.code == sf::Keyboard::Space) {
						shipyard[index].i = part;
						int tries = 0;
						while (!valid_design(shipyard) && tries++ < 4) {
							shipyard[index].rotation = (shipyard[index].rotation + 1) % 4;
						}
					}
					else if (event.key.code == sf::Keyboard::Delete) {
						shipyard[index].i = 0;
					}
					if (event.key.code == sf::Keyboard::Enter) {
						if (valid_design(shipyard)) {
							in_shipyard = false;
							Ship* ship = new Ship();
							for (int i = 0; i < shipyard.size(); i++) {
								shipyard[i].x = (i % 18) * 16;
								shipyard[i].y = (i / 18) * 16;
								if (shipyard[i].i > 0) {
									if (parts[shipyard[i].i].crucial) {
										if (parts[shipyard[i].i].name == "Station Control") {
											ship->station = true;
										}
									}
									ship->parts.push_back(shipyard[i]);
								}
							}
							calculateMass(ship);
							for (Part& p : ship->parts) {
								p.x -= ship->x;
								p.y -= ship->y;
							}
							ships.push_back(ship);
							if (player == nullptr) {
								player = ship;
								view.setCenter(player->pos.x, player->pos.y);
								//player->HUD.push_back(Track("Nearest Star", star_track_POS));
							}
						}
					}
				}
				else {
					if (event.key.code == sf::Keyboard::P) {
						in_shipyard = true;
						shipyard = {};
						for (int i = 0; i < 18 * 18; i++) {
							shipyard.push_back(Part());
						}
					}
					if (event.key.code == sf::Keyboard::L) {
						if (player != nullptr) {
							player->docking = !(player->docking);
						}
					}
				}
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (!in_shipyard) {
					if (event.key.code == sf::Keyboard::Space) {
						if (player != nullptr) {
							player->pos.dx = 0;
							player->pos.dy = 0;
							player->pos.da = 0;
							player->path = {};
							bdx = 0;
							bdy = 0;
						}
					}
					if (event.key.code == sf::Keyboard::Up) {
						bdy += .1;
					}
					else if (event.key.code == sf::Keyboard::Down) {
						bdy -= .1;
					}
					if (event.key.code == sf::Keyboard::Left) {
						bdx -= .1;
					}
					else if (event.key.code == sf::Keyboard::Right) {
						bdx += .1;
					}
				}
			}
			else if (event.type == sf::Event::MouseWheelMoved) {
				if (event.mouseWheel.delta < 0 && zoom < 6.0) {
					zoom += .05;
					view = sf::View(sf::Vector2f(1920, 1080), sf::Vector2f(1920, 1080));
					view.zoom(zoom);
					shape = sf::RectangleShape(view.getSize());
				}
				else if (event.mouseWheel.delta > 0 && zoom > .25) {
					view = sf::View(sf::Vector2f(1920, 1080), sf::Vector2f(1920, 1080));
					zoom -= .05;
					view.zoom(zoom);
					shape = sf::RectangleShape(view.getSize());
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left && player != nullptr) {
					/*if (player != nullptr) {
						player->path = path(*player, mX, mY, planets);
					}*/

					//Bullet* b = new Bullet(mouseForce);
					//b->next = bullets;
					//bullets = b;
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (player != nullptr) {
						for (Part p : player->parts) {
							if (parts[p.i].turret) {
								Position pos(partX(player->pos, p), partY(player->pos, p), player->pos.a, player->pos.dx, player->pos.dy, player->pos.da);
								pos.a = p.turret_angle;
								pos.dx = sin(pos.a) * pos.dy + cos(pos.a) * pos.dx;
								pos.dy = sin(pos.a) * pos.dx - cos(pos.a) * pos.dy;
								Bullet* b = new Bullet(pos);
								//b->pos.x += cos(b->pos.a + rad(90)) * 6;
								//b->pos.y += sin(b->pos.a + rad(90)) * 6;
								b->next = bullets;
								bullets = b;
							}
						}
					}
				}
			}
		}

		//Here stuff that shouldn't be linked to framerate is handled, ie Ship Movement
		if (elapsedTime >= targetTime) {
			accelerated = false;
			elapsedTime -= targetTime;
			Bullet* b = bullets;
			if (!in_shipyard) {
				targetGuns(player, mX, mY);
			}
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

			std::vector<Explosion> e_copy = {};
			for (Explosion& e : explosions) {
				updateExplosion(e);
				if (e.max > 0) {
					e_copy.push_back(e);
				}
			}
			explosions = e_copy;

			checkCollision(ships);
			for (Ship* s : ships) {
				if (s == player) {
					if (player->path.size() > 0) {
						PathPos path = (player->path).front();
						player->path[0].num--;
						if (player->path[0].num <= 0) {
							player->path.pop_front();
						}
						moveShip(s, path.up, path.down, path.left, path.right);
						if (!(player->pos == path.pos)) {
							float aper = 3;
						}
					}
					else {
						moveShip(s, up, down, left, right);
					}
				}
				else {
					moveShip(s);
				}
				dockPos(s);
				updatePosition(s->pos, s->x, s->y);
				for (Planet p : planets) {
					applyGravity(s->pos, p);
				}
			}
		}

		//Drawing Code is handled here
		if (in_shipyard) {
			dX = 50 + view.getCenter().x - view.getSize().x / 2.0;
			dY = 50 + view.getCenter().y - view.getSize().y / 2.0;
			for (int i = 0; i < shipyard.size(); i++) {
				if (shipyard[i].i > 0) {
					sf::Color color = sf::Color(255, 255, 255);
					bool can_connect = false;
					//Check if piece can be placed this way
					if (!(shipyard[i].valid)) {
						color = sf::Color(255, 0, 0);
					}
					drawSprite(parts[shipyard[i].i].sX + 16 * (shipyard[i].rotation % 2), parts[shipyard[i].i].sY + 16 * (shipyard[i].rotation / 2), 16, 16, dX + (i % 18) * 16, dY + (i / 18) * 16, 1, 0.0f, color);
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
				//drawPosition(Position(s->x, s->y));
				drawFutureTrajectory(*s, planets);
				drawPath(*s, planets);
			}
			Bullet* b = bullets;
			while (b != nullptr) {
				drawBullet(b);
				b = b->next;
			}
			for (Explosion e : explosions) {
				drawExplosion(&e);
			}
			for (Planet p : planets) {
				drawPlanet(&p, view);
			}
		}
		
		if (player != nullptr) {
			if (player->HUD.size() > 0) {
				for (Track t : player->HUD) {
					float x = player->pos.x;
					float y = player->pos.y;
					float angle = atan2(t.pos.y - y, t.pos.x - x);
					x += cos(angle) * view.getSize().x / 2.1;
					y += sin(angle) * view.getSize().y / 2.1;
					Print(t.msg, x, y);
				}
			}
			//On screen debugging is handled here
			if (!in_shipyard) {
				float x = player->pos.x;
				float y = player->pos.y;
				float x1 = player->pos.x;
				float y1 = player->pos.y;
				float a = atan2(-player->pos.dy, player->pos.dx);
				float a1 = atan2(mY - player->pos.y, mX - player->pos.x);

				//drawPosition(mX, mY, 5, 0, sf::Color::Red);
				/*a = atan2(-bdy, bdx);
				for (int i = 0; i < 10; i++) {
					x1 += cos(a) * 10;
					y1 += sin(a) * 10;
					drawPosition(x1, y1, 1, 0, sf::Color::Red);
				}*/
				//Print(to_str(posA(player->pos, Position(mX, mY))), player->pos.x - 200, player->pos.y - 100);
				//Print("X: " + to_str(multX(player->pos, Position(mX, mY))), player->pos.x - 200, player->pos.y - 75);
				//Print("Y: " + to_str(multY(player->pos, Position(mX, mY))), player->pos.x - 200, player->pos.y - 50);
			}
		}
		window.display();
	}

	return 0;
}