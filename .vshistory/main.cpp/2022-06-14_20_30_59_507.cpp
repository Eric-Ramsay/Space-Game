#include <SFML\Graphics.hpp>
#include <vector>
#include <deque>
#include <sstream>
#include <iostream>

#include "constants.h"
#include "algos.h"
#include "structures.h"
#include "functions.h"
#include "ship_functions.h"
#include "draw.h"


std::deque<PathPos> path(Ship a, float x, float y, std::vector<Planet> planets) {
	//God forgive me for what I am about to write
	//For now, leave things simple

	Ship copy = a;
	PathPos current(a.pos);
	current.h = 9999;
	int mindex = -1;
	float closest = 99999;
	std::deque<PathPos> path = {};
	std::deque<PathPos> open = { current };
	std::vector<PathPos> closed = {};
	bool stop = false;
	bool found = false;
	float tot_dist = dist(a.pos.x, a.pos.y, x, y);

	while (closed.size() < 300 && open.size() > 0 && !stop) {
		int mind = 0;
		float val = open[mind].h;
		float comp, dis;
		window.clear();
		drawShip(&a);
		for (int i = 1; i < open.size(); i++) {
			drawPosition(open[i].pos);
			comp = open[i].h;
			if (comp < val) {
				mind = i;
				val = comp;
			}
		}
		window.display();
		current = open[mind];
		open.erase(open.begin() + mind);
		closed.push_back(current);
		dis = dist(current.pos.x, current.pos.y, x, y);
		std::cout << "Iteration: " << closed.size() << std::endl;
		std::cout << "Distance: " << dis << std::endl;
		//std::cout << "Closest Iteration: " << mindex << std::endl;
		std::cout << "Closest: " << closest << std::endl << std::endl;

		if (dis < closest) {
			mindex = closed.size() - 1;
			closest = dis;
		}
		if (dis < 10) {
			stop = true;
		}
		else {
			for (int u = 0; u < 2; u++) {
				for (int d = 0; d < 2; d++) {
					for (int l = 0; l < 2; l++) {
						for (int r = 0; r < 2; r++) {
							if ((!u || !d) && (!l || !r)) {
								found = false;
								copy.pos = current.pos;
								float cur_fuel = copy.energy;
								int num = 10 + sqrt(2*dis);
								for (int i = 0; i < num; i++) {
									moveShip(&copy, u, d, l, r);
									for (Planet p : planets) {
										applyGravity(copy.pos, p);
									}
								}
								PathPos ape(copy.pos, closed.size() - 1, u, d, l, r);
								ape.num = num;
								for (Planet p : planets) {
									if (planetaryCollision(&copy, p)) {
										found = true;
										break;
									}
								}
								if (!found) {
									for (PathPos p : closed) {
										if (ape == p) {
											found = true;
											break;
										}
									}
									if (!found) {
										int size = open.size();
										for (int i = size - 1; i >= 0; i--) {
											if (ape == open[i]) {
												found = true;
												break;
											}
										}
										if (!found) {
											ape.h = dist(x, y, copy.pos.x, copy.pos.y);
											ape.g = dist(a.pos.x, a.pos.y, ape.pos.x, ape.pos.y);
											ape.v = 2 * calcV(ape.pos, x, y, planets, ape.num) * max(0, ((ape.g - ape.h) / tot_dist));
											ape.f = 0;// cur_fuel - copy.energy;
											ape.h = ape.h + .75 * ape.g + ape.v + ape.f * .1;
											open.push_back(ape);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (!stop) {
		current = closed[mindex];
	}
	while (current.parent > -1) {
		path.push_front(current);
		current = closed[current.parent];
	}
	path.push_front(current);
	return path;
}

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

	Ship* player = nullptr;;
	Planet p(1500, 1500, 1500);
	float elapsedTime = 0.0f;
	float targetTime = .01f;
	bool accelerated = false;
	std::vector<Part> parts = initParts();

	std::vector<Ship*> ships = {};
	std::vector<Planet> planets = {p};
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
		space.setUniform("offset", sf::Glsl::Vec2{ shape.getPosition() });
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
						shipyard[index] = parts[part];
						int tries = 0;
						while (!valid_design(shipyard) && tries++ < 4) {
							shipyard[index].rotation = (shipyard[index].rotation + 1) % 4;
						}
					}
					if (event.key.code == sf::Keyboard::Enter) {
						if (valid_design(shipyard)) {
							in_shipyard = false;
							Ship* ship = new Ship();
							for (int i = 0; i < shipyard.size(); i++) {
								shipyard[i].x = (i % 18) * 16;
								shipyard[i].y = (i / 18) * 16;
								if (shipyard[i].index > 0) {
									if (shipyard[i].crucial) {
										if (shipyard[i].name == "Station Control") {
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
							}
						}
					}
				}
				else {
					if (event.key.code == sf::Keyboard::P) {
						in_shipyard = true;
						shipyard = {};
						for (int i = 0; i < 18 * 18; i++) {
							shipyard.push_back(parts[0]);
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
						}
					}
				}
			}
			else if (event.type == sf::Event::MouseWheelMoved) {
				if (event.mouseWheel.delta < 0 && zoom < 3.0) {
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
				if (event.mouseButton.button == sf::Mouse::Left) {
					Position mouseForce;
					mouseForce.x = mX;
					mouseForce.y = mY;
					float angle = atan2(player->y - mY, player->x - mX);
					mouseForce.a = angle + rad(90);// rad(90);
					mouseForce.dx;
					mouseForce.dy = 3;
					//force(player, mouseForce);
					if (player != nullptr) {
						player->path = path(*player, mX, mY, planets);
					}

					//Bullet* b = new Bullet(mouseForce);
					//b->next = bullets;
					//bullets = b;
					//bullets = b;

				}
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					for (Part p : player->parts) {
						if (p.turret) {
							Position pos(partX(player->pos, p), partY(player->pos, p), p.turret_angle, 0, -3);
							Bullet* b = new Bullet(pos);
							b->pos.x += cos(b->pos.a + rad(90)) * 6;
							b->pos.y += sin(b->pos.a + rad(90)) * 6;
							b->next = bullets;
							bullets = b;
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
				//drawFutureTrajectory(*s, planets);
				drawPath(*s, planets);
			}
			Bullet* b = bullets;
			while (b != nullptr) {
				drawBullet(b);
				b = b->next;
			}
			for (Planet p : planets) {
				drawPlanet(&p, view);
			}
		}

		//On screen debugging is handled here
		if (player != nullptr && !in_shipyard) {
			Print(to_str(player->energy), mX, mY, true);

			if (player->connected.size() > 0) {
				Print("docked", view.getCenter().x, view.getCenter().y - view.getSize().y / 2.0 + 40, 1, true);
			}
			//Print(to_str(player->path.size()), player->pos.x - 50, player->pos.y);

			//Print(to_str((int)(g)), player->pos.x + 50, player->pos.y - 50);
			Print("X: " + to_str((int)(player->pos.x)), player->pos.x + 50, player->pos.y - 25);
			//Print("Vel Dist: " + to_str((int)(v)), player->pos.x + 50, player->pos.y);
			Print("Y: " + to_str((int)(player->pos.y)), player->pos.x + 50, player->pos.y + 25);
			for (Planet p : planets) {
				if (planetaryCollision(player, p)) {
					Print("Collision with Planet", player->pos.x - 50, player->pos.y);
				}
			}
		}
		window.display();
	}

	return 0;
}