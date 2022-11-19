#include <SFML\Graphics.hpp>
#include <sstream>
#include <vector>
// Global Variables - Yeah Yeah
// These are here to limit parameter list sizes
sf::RenderWindow window(sf::VideoMode(1920, 1080), "SP Game", sf::Style::Fullscreen);
sf::Texture sprites;
int timer = 0;
int zoom = 10;

// Creates a sprite sheet from a file string
sf::Texture createTexture(std::string file) {
    sf::Texture texture;
    texture.loadFromFile(file);
    return texture;
}

// Draws a sprite from the global sprite sheet
void drawSprite(int sX, int sY, int w, int h, int dX, int dY, int scale = zoom, float rotation = 0.0f, sf::Color color = sf::Color(255, 255, 255)) {
    sf::IntRect rect(sX, sY, w, h);
    sf::Sprite sprite(sprites, rect);
    sprite.setScale(scale, scale);
    sprite.setPosition(dX, dY);
	sprite.setColor(color);
	sprite.setRotation(rotation);
    window.draw(sprite);
}

sf::Color getColor(std::string text = "") {
	return sf::Color(255, 255, 255);
}

int charWidth(char c) {
	//Assumes char width of 5 and char height of 7
	switch (c) {
	case '"':case ',': case '.': case '\'': case '!': case ':': return 1;
	case ';': return 2;
	case ' ': case '[': case ']': case '|': return 3;
	case '\n': return 0;
	default: return 5;
	}
}

template <typename Type> std::string to_str(const Type& t) {
	std::ostringstream os;
	os << t;
	return os.str();
}

float safeC(float a, float MAX) {
	if (a >= MAX) {
		return fmod(a, MAX);
	}
	else if (a < 0) {
		return safeC(a + MAX, MAX);
	}
	return a;
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

float rad(float deg) {
	return (deg * 3.14159265358979323846) / 180.0f;
}

struct Connection {
	bool top = true;
	bool bot = true;
	bool left = true;
	bool right = true;
};

struct Part {
	std::string name = "";
	int index = -1;
	int sX = 0;
	int sY = 0;

	float x = 0;
	float y = 0;
	float angle = 0;
	float velocity = 0;

	int cargo_space = 0;
	int durability = 0;
	float damage = 0;
	float mass = 0;
	float active_thrust = 0;
	float active_energy = 0;
	float passive_energy = 0;
	Connection connects[4];
	int rotation = 0;
	bool rotatable = false;
	bool crucial = false;

	bool valid = true;

	Part* top = nullptr;
	Part* bot = nullptr;
	Part* left = nullptr;
	Part* right = nullptr;

	Part(std::string n = "Nothing", int dura = 25) {
		name = n;
		durability = dura;
	}
};

struct Ship {
	float max_speed = 25;
	int duration = -1;
	int sX = 0;
	int sY = 0;
	int w = 0;
	int h = 0;
	std::vector<Part> parts = {};
	Ship(float x1, float y1, float a, float v, int sx, int sy, int w1, int h1, int d = -1) {
		sX = sx;
		sY = sy;
		w = w1;
		h = h1;
		duration = d;
	}
};

void moveShip(Ship* s) {
	//Given a ship, move it
	//First calculate its center of mass
	float x_top = 0.0;
	float bot = 0.0;
	float y_top = 0.0;
	float y_bot = 0.0;
	for (int i = 0; i < s->parts.size(); i++) {
		Part p = (s->parts)[i];
		bot += p.mass;
		x_top += p.mass * p.x;
		y_top += p.mass * p.y;
		p.x += cos(rad(p.angle)) * p.velocity;
		p.y += sin(rad(p.angle)) * p.velocity;
	}
	x_top /= bot;
	y_top /= bot;
	//s->x += cos(rad(s->angle)) * s->velocity
}

void drawShip(Ship* s) {
	for (int i = 0; i < s->parts.size(); i++) {
		Part p = s->parts[i];
		drawSprite(p.sX, p.sY, 16, 16, p.x, p.y, 4, p.angle);
	}
}

bool valid_design(std::vector<Part>& shipyard) {
	int command = -1;
	for (int i = 0; i < shipyard.size(); i++) {
		if (shipyard[i].index > 0) {
			shipyard[i].valid = false;
		}
		else {
			shipyard[i].valid = true;
		}
	}
	for (int i = 0; i < shipyard.size(); i++) {
		if (shipyard[i].crucial) {
			if (command > -1) {
				return false;
			}
			command = i;
		}
	}
	if (command > -1) { //1 command module, check if everything is connected to it
		std::deque<int> open_list = {command};
		shipyard[command].valid = true;
		while (open_list.size() > 0) {
			int current = deque.pop_front();
			for (int a = -1; a < 2; a += 2) {
				if (shipyard[safeC(current + a)].index > 0 && !shipyard[safeC(current + a)].valid) {
					if (connectedParts(shipyard[current], shipyard[safeC(current + a)]) {
						open_list.push_back(safeC(current + a))
					}
				}
				if (shipyard[safeC(current + a*18)].index > 0) {
					if (connectedParts(shipyard[current], shipyard[safeC(current + a*18)]) {

					}
				}
			}
		}
	}
	else {
		return false;
	}
}

std::vector<Part> initParts() {
	std::vector<Part> parts;

	parts.push_back(Part("Nothing"));
	parts.push_back(Part("Command Module"));
	parts.push_back(Part("Large Thruster"));
	parts.push_back(Part("Fuel Cell"));
	parts.push_back(Part("Weapons Platform"));
	parts.push_back(Part("Cargo Cell"));
	parts.push_back(Part("Armor Plating"));
	parts.push_back(Part("Small Thruster"));
	parts.push_back(Part("Shield Generator"));
	parts.push_back(Part("Radiator"));
	parts.push_back(Part("Solar Panel"));

	for (int i = 0; i < parts.size(); i++) {
		parts[i].sY = 32 * i;
		parts[i].index = i;
	}

	return parts;
}

// Handles main gain loop
int main() {
    sf::Clock clock;
    sprites = createTexture("./Sprites/sprites.png");
	Ship player(0, 0, 0, 0, 240, 16, 13, 13);
    float elapsedTime = 0.0f;
    float targetTime = .05f;
	bool accelerated = false;
	std::vector<Ship*> things = {&player};
	std::vector<Part> parts = initParts();
	std::vector<Part> shipyard = {};
	bool in_shipyard = true;
	int index = 0;
	for (int i = 0; i < 18 * 18; i++) {
		shipyard.push_back(parts[0]);
	}

    while (window.isOpen()) {
        elapsedTime += clock.getElapsedTime().asSeconds();
        clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::R) {
					shipyard[index].rotation = (shipyard[index].rotation + 1) % 4;
				}
				if (in_shipyard) {
					if (event.key.code == sf::Keyboard::Up) {
						shipyard[index] = parts[safeC(shipyard[index].index + 1, parts.size())];
					}
					else if (event.key.code == sf::Keyboard::Down) {
						shipyard[index] = parts[safeC(shipyard[index].index - 1, parts.size())];
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
				}
			}
        }
		
        if (elapsedTime >= targetTime) {
			accelerated = false;
            elapsedTime -= targetTime;
            timer = (timer + 1) % 1000;
        }

		/*sf::Vector2i mousePos = sf::Mouse::getPosition();

		const float PI = 3.14159265;

		int x = mousePos.x;
		int y = mousePos.y;
		float angle = 90 + atan2(1080 / 2 - y, 1920 / 2 - x) * 180 / 3.14159265359;*/

        window.clear();
		if (in_shipyard) {
			for (int i = 0; i < shipyard.size(); i++) {
				if (shipyard[i].index > 0) {
					sf::Color color = sf::Color(255, 255, 255);
					bool can_connect = false;
					//Check if piece can be placed this way
					if (shipyard[i].connects.top && shipyard[safeC(i - 18, shipyard.size())].index > 0 && shipyard[safeC(i - 18, shipyard.size())].connects.bottom) {
						can_connect = true;
					}
					else if (shipyard[i].connects.bottom && shipyard[safeC(i + 18, shipyard.size())].index > 0 && shipyard[safeC(i + 18, shipyard.size())].connects.top) {
						can_connect = true;
					}
					else if (shipyard[i].connects.left && shipyard[safeC(i - 1, shipyard.size())].index > 0 && shipyard[safeC(i - 1, shipyard.size())].connects.right) {
						can_connect = true;
					}
					else if (shipyard[i].connects.right && shipyard[safeC(i - 1, shipyard.size())].index > 0 && shipyard[safeC(i - 1, shipyard.size())].connects.left) {
						can_connect = true;
					}
					if (!can_connect) {
						color = sf::Color(255, 0, 0);
					}
					drawSprite(shipyard[i].sX + 16 * (shipyard[i].rotation % 2), shipyard[i].sY + 16 * (shipyard[i].rotation / 2), 16, 16, 50 + (i % 18) * 16*4, 50 + (i / 18) * 16 * 4, 4, 0.0f, color);
				}
				if (i == index) {
					drawSprite(64, 0, 16, 16, 50 + (i % 18) * 16 * 4, 50 + (i / 18) * 16 * 4, 4);
				}
			}
		}
		else {
			for (Ship* s : things) {
				drawShip(s);
			}
		}
		
        window.display();
    }

    return 0;
}