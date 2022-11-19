#include <SFML\Graphics.hpp>
#include <sstream>
#include <vector>
#include <deque>
#include <iostream>
// Global Variables - Yeah Yeah
// These are here to limit parameter list sizes
sf::RenderWindow window(sf::VideoMode(1920, 1080), "SP Game", sf::Style::Fullscreen);
sf::Texture sprites;
float zoom = .25;
int timer = 0;

// Creates a sprite sheet from a file string
sf::Texture createTexture(std::string file) {
    sf::Texture texture;
    texture.loadFromFile(file);
    return texture;
}

// Draws a sprite from the global sprite sheet
void drawSprite(int sX, int sY, int w, int h, float dX, float dY, int scale = 1, float rotation = 0.0f, sf::Color color = sf::Color(255, 255, 255)) {
    sf::IntRect rect(sX, sY, w, h);
    sf::Sprite sprite(sprites, rect);
	sprite.setOrigin(w / 2, h / 2);
    sprite.setScale(scale, scale);
    sprite.setPosition(dX, dY);
	sprite.setColor(color);
	sprite.setRotation(rotation);
    window.draw(sprite);
}

sf::Color getColor(std::string text = "") {
	return sf::Color(255, 255, 255);
}



//Prints text onto the screen
int Print(std::string text, int x = -1, int y = -1, float scale = 1.0f, bool center = false, int width = 1920 / 2) {
	int cap = width;
	int TEXT_W = 5;
	int TEXT_H = 7;

	sf::Color p = sf::Color(255, 255, 255);
	if (y == -1) {
		cap = x;
	}
	std::vector<int> splitIndexes;
	std::string line = "";
	bool skipping = false;
	std::string color = "";
	std::vector<int> lineWidth = { 0 };
	int spaces = 0; int splits = 0;
	int drawX = x;
	int drawY = y;
	int offX = 0; int offY = 0;
	int sH = TEXT_H; int sW = TEXT_W;
	int sY = 1;
	int sX = 65;
	bool prnt = true;

	int wordlen = 0;
	for (char& c : text) {
		if (c == '*') {
			skipping = !skipping;
		}
		else if (!skipping) {
			wordlen += scale * (charWidth(c) + 1);
			lineWidth[lineWidth.size() - 1] += scale * (charWidth(c) + 1);
			line += c;
			if (c == ' ' || c == '\n') {
				if (c == '\n' || lineWidth[lineWidth.size() - 1] > cap) {
					line = "";
					if (lineWidth[lineWidth.size() - 1] > cap && spaces > 0) {
						splitIndexes.push_back(spaces - 1);
						lineWidth[lineWidth.size() - 1] -= wordlen;
						lineWidth.push_back(wordlen);
					}
					else {
						splitIndexes.push_back(spaces);
						lineWidth.push_back(0);
					}
				}
				wordlen = 0;
				spaces++;
			}
		}
	}
	if (lineWidth[lineWidth.size() - 1] > cap && spaces > 0) {
		splitIndexes.push_back(spaces - 1);
		lineWidth[lineWidth.size() - 1] -= wordlen;
		lineWidth.push_back(wordlen);
	}
	spaces = 0;
	if (y == -1) {
		return splitIndexes.size();
	}
	if (center) {
		drawX = 1 + x - (lineWidth[splits] / 2); //if ur having problems try scale + x
		drawY = y - scale * lineWidth.size() * (1 + (sH / 2));
	}
	skipping = false;
	for (char& c : text) {
		sH = TEXT_H; sW = TEXT_W;
		if (c == '*') {
			skipping = !skipping;
			if (!skipping) {
				if (getColor(color) != p) {
					p = getColor(color);
				}
				else {
					p = sf::Color(255, 255, 255);
				}
				color = "";
			}
		}
		else if (!skipping) {
			offX = 0; offY = 0;
			sH = sH; sW = charWidth(c);
			sY = 0; sX = 0;
			prnt = true;
			if (c >= 48 && c <= 57) { //Numbers
				sY = sH + 1;
				sX = ((int)c - 48) * (sW + 1);
			}
			else if (c >= 65 && c <= 90) { //Letters, uppercase(?)
				sY = 0;
				sX = ((int)c - 65) * (sW + 1);
			}
			else if (c >= 97 && c <= 122) { //Letters again, lowercase(?)
				sY = 0;
				sX = ((int)c - 97) * (sW + 1);
			}
			else {
				sY = 17;
				switch (c) {
				case '"': case '\'': sX = 1; sY = 21; sW = 1; sH = 3; break;
				case '.': sX = 118; sW = 1; break;
				case ',': sX = 1; sW = 1; offY = 1; break;
				case '>': sX = 6; break;
				case '?': sX = 18; break;
				case '!': sX = 14; sW = 1; break;
				case '+': sX = 24; break;
				case '-': sX = 30; break;
				case '$': sX = 36; break;
				case ':': sX = 44; sW = 1; break;
				case '<': sX = 84; break;
				case '[': sX = 48; sW = 3; break;
				case ']': sX = 56; sW = 3; break;
				case '#': sX = 60; break;
				case '(': sX = 66; break;
				case ')': sX = 72; break;
				case '=': sX = 78; break;
				case '/': sX = 92; break;
				case '%': sX = 98; break;
				case '|': sX = 60; sW = 3; break;
				case ';': sX = 89; sW = 2; break;
				case ' ': sW = 2; prnt = false;
				case '\n': prnt = false;
				default: prnt = false;
				}
			}
			if (prnt) {
				//drawSprite(int sX, int sY, int w, int h, int dX, int dY, int scale = zoom, sf::Color color = sf::Color(255, 255, 255))
				drawSprite(sX + 320, sY, sW, sH, drawX + offX * scale, drawY + offY * scale, scale, 0, p);
			}
			if (c != '\n') {
				drawX += scale * (sW + offX + 1);
			}
			if (c == ' ' || c == '\n') {
				if (splitIndexes.size() > splits && splitIndexes[splits] == spaces++) {
					drawX = x;
					splits++;
					if (center) {
						drawX -= (lineWidth[splits] / 2);
					}
					drawY += scale * (sH + 3);
				}
			}
		}
		else {
			color += c;
		}
	}
	return splits;
}


void drawFutureTrajectory(Ship s, std::vector<Planet>& plan) {
	Ship copy = s;
	for (int i = 0; i < 2000; i++) {
		moveShip(&s);
		for (Planet p : plan) {
			applyGravity(&s, p);
		}
		if (i % 20 == 0) {
			drawSprite(34, 0, 2, 2, s.cx, s.cy);
		}
	}
}

void drawBullet(Bullet* b) {
	drawSprite(48, 32, 4, 1, b->pos.x, b->pos.y, 1, deg(b->pos.a), b->color);
}

void drawShip(Ship* s) {
	int sX = 0;
	for (int i = 0; i < s->parts.size(); i++) {
		Part p = s->parts[i];
		sX = p.sX + 16 * (p.rotation % 2);
		if (p.thrust > 0 && p.active) {
			sX += 32;
		}
		drawSprite(sX, p.sY + 16 * (p.rotation / 2), 16, 16, p.pos.x, p.pos.y, 1, deg(p.pos.a));
		if (p.turret) {
			drawSprite(32, 128, 16, 16, p.pos.x, p.pos.y, 1, deg(p.turret_angle));
		}
	}
}

void drawPlanet(Planet* p, sf::View& view) {
	static bool init = true;
	static sf::Shader planet;
	if (init) {
		init = false;
		if (!planet.loadFromFile("planet.frag", sf::Shader::Fragment)) {
			std::cout << "There was an error loading a fragment shader" << std::endl;
		}
	}
	planet.setUniform("offset", sf::Glsl::Vec2{ view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2 });
	planet.setUniform("position", sf::Glsl::Vec2{ p->x, p->y });
	planet.setUniform("position", sf::Glsl::Vec2{ window.getSize() });
	sf::CircleShape plan(p->size, p->size/2);
	plan.setPosition(p->x - p->size/2, p->y - p->size/2);
	float x = plan.getPosition().x;
	float y = plan.getPosition().y;
	window.draw(plan, &planet);
}



bool connectedParts(Part a1, Part b1, int relation) {
	Connection a = rotConnect(a1.connects, a1.rotation);
	Connection b = rotConnect(b1.connects, b1.rotation);
	if (relation == 0) { //a is above b
		return a.bot && b.top;
	}
	else if (relation == 1) { //a is to the right of b
		return a.left && b.right;
	}
	else if (relation == 2) { //a is below b
		return a.top && b.bot;
	}
	else if (relation == 3) { //a is to the left of b
		return a.right && b.left;
	}
	return false;
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