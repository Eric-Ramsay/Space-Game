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
float deg(float rad) {
	return safeC(rad * 180 / 3.14159265358979323846, 360);
}


struct Connection {
	bool top = true;
	bool bot = false;
	bool left = false;
	bool right = false;
	Connection(bool t = false, bool r = false, bool b = false, bool l = false) {
		top = t;
		right = r;
		bot = b;
		left = l;
	}
};
struct vec2 {
	float x = 0.0;
	float y = 0.0;
	vec2(float x1, float y1) {
		x = x1;
		y = y1;
	}
};
float dot2(vec2 a, vec2 b) {
	return a.x * b.x + a.y * b.y;
}

struct Part {
	std::string name = "";
	int index = -1;
	int sX = 0;
	int sY = 0;

	float x = 0;
	float y = 0;
	float angle = 0;
	float da = 0;
	float dx = 0;
	float dy = 0;


	int cargo_space = 0;
	int durability = 0;
	float damage = 0;

	float mass = 0;
	bool active = false;
	float thrust = 0;

	float active_energy = 0;
	float passive_energy = 0;
	Connection connects;
	int rotation = 0;
	bool rotatable = false;
	bool crucial = false;

	bool valid = true;

	bool turret = false;
	float turret_angle = 0.0;

	Part* top = nullptr;
	Part* bot = nullptr;
	Part* left = nullptr;
	Part* right = nullptr;

	Part(std::string n = "Nothing", int dura = 25, float m1 = 25, Connection c = Connection(true, false, false, false), bool cruc = false, float t1 = 0) {
		name = n;
		durability = dura;
		connects = c;
		crucial = cruc;
		mass = m1;
		thrust = t1;
	}
};

struct Bullet {
	sf::Color color;
	float x = 0.0;
	float y = 0.0;
	float angle = 0.0;
	float velocity = 0.0;
	int power = 1000;
	Bullet* next;

	Bullet(float x1, float y1, float a, float v, float p = 1000, sf::Color c = sf::Color(255, 0, 0)) {
		x = x1;
		y = y1;
		angle = a;
		velocity = v;
		power = p;
		color = c;
	}
};

struct Planet {
	float x = 0;
	float y = 0;
	float size = 10;
	Planet(float x1, float y1, float s) {
		x = x1;
		y = y1;
		size = s;
	}
};

struct Ship {
	float cx = 0.0;
	float cy = 0.0;
	std::vector<Part> parts = {};
};

float dist(float x1, float y1, float x2, float y2) {
	return sqrtf(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void targetGuns(Ship* s, float x, float y) {
	for (Part& p : s->parts) {
		if (p.turret) {
			float angle = rad(90) + atan2(p.y - y, p.x - x);
			p.turret_angle = angle;
		}
	}
}

void applyGravity(Ship* s, Planet p) {
	float d = dist(p.x, p.y, s->cx, s->cy)
	if (d <= 5 * p.size) {

	}
}

void moveShip(Ship* s, bool up, bool down, bool left, bool right) {
	//Calculate center of mass and center of thrust
	float mx = 0.0;
	float my = 0.0;
	float mass = 0.0;
	
	float da = 0;
	float dx = 0;
	float dy = 0;

	//First, calculate center of mass
	for (Part& p : s->parts) {
		p.angle += p.da;
		p.x += sin(p.angle) * p.dy;
		p.y -= cos(p.angle) * p.dy;
		p.x += cos(p.angle) * p.dx;
		p.y += sin(p.angle) * p.dx;
		mass += p.mass;
		mx += p.mass * p.x;
		my += p.mass * p.y;
	}
	if (mass > 0) {
		mx /= mass;
		my /= mass;
	}
	else {
		return;
	}
	s->cx = mx;
	s->cy = my;

	//Next calculate thrust
	for (Part& p : s->parts) {
		float T = 0;
		float dyt = 0;
		float dxt = 0;
		if (p.thrust > 0) {
			p.active = false;
			if (p.rotation == 0 && up || p.rotation == 1 && right || p.rotation == 2 && down || p.rotation == 3 && left) {
				p.active = true;
				//da += torque;
				switch (p.rotation) {
				case 0: dyt += p.thrust; break;
				case 1: dxt += p.thrust; break;
				case 2: dyt -= p.thrust; break;
				case 3: dxt -= p.thrust; break;
				}
				float angle = atan2(p.y - my, p.x - mx) - p.angle;
				float x_force = -sin(angle) * dxt * dist(p.x, p.y, mx, my) / 2;
				float y_force = -cos(angle) * dyt * dist(p.x, p.y, mx, my) / 2;
				T += x_force;
				T += y_force;
				da += T / (50000 * mass);
				dx += dxt;
				dy += dyt;
			}
		}
	}
	dy /= 10*mass;
	dx /= 10*mass;	

	sf::Vector2i m = sf::Mouse::getPosition();
	float d, angle;
	for (Part& p : s->parts) {
		float co = cos(p.da);
		float si = sin(p.da);
		p.dy += dy;
		p.dx += dx;
		float x_dif = p.x - mx;
		float y_dif = p.y - my;
		p.x = co * x_dif - si * y_dif + mx;
		p.y = si * x_dif + co * y_dif + my;
		p.da += da;
	}
}

void moveBullet(Bullet* b) {
	b->x += cos(b->angle) * b->velocity;
	b->y += sin(b->angle) * b->velocity;
	b->power--;
}

void drawBullet(Bullet* b) {
	drawSprite(48, 32, 4, 1, b->x, b->y, 1, deg(b->angle), b->color);
}

void drawShip(Ship* s) {
	int sX = 0;
	for (int i = 0; i < s->parts.size(); i++) {
		Part p = s->parts[i];
		sX = p.sX + 16 * (p.rotation % 2);
		if (p.thrust > 0 && p.active) {
			sX += 32;
		}
		drawSprite(sX, p.sY + 16 * (p.rotation / 2), 16, 16, p.x, p.y, 1, deg(p.angle));
		if (p.turret) {
			drawSprite(32, 128, 16, 16, p.x, p.y, 1, deg(p.turret_angle));
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
	plan.setPosition(p->x, p->y);
	window.draw(plan, &planet);
}

Connection rotConnect(Connection co, int r) {
	Connection c = co;
	//0 -> a 0 degree rotation, no change
	//1 -> a 90 degree clockwise rotation
	//2 -> 180 degree rotation, a flip
	//3 -> 270 degree rotation, akaa a 90 degree counterclockwise rotation
	switch (r) {
	case 1: c.top = co.left; c.right = co.top; c.bot = co.right; c.left = co.bot; break;
	case 2: c.top = co.bot; c.right = co.left; c.bot = co.top; c.left = co.right; break;
	case 3: c.top = co.right; c.right = co.bot; c.bot = co.left; c.left = co.top; break;
	}
	return c;
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
		if (shipyard[i].index > 0) {
			if (shipyard[i].crucial) {
				if (command > -1) {
					return false;
				}
				command = i;
			}
		}
	}
	if (command > -1) { //1 command module, check if everything is connected to it
		std::deque<int> open_list = {command};
		while (open_list.size() > 0) {
			int current = open_list.front();
			open_list.pop_front();
			shipyard[current].valid = true;
			for (int i = 0; i < 4; i++) {
				int cons = 0;
				switch (i) {
				case 0: cons = safeC(current + 18, shipyard.size()); break; //current part is above considered part
				case 1: cons = safeC(current - 1, shipyard.size()); break; //current part is to the right of the considered
				case 2: cons = safeC(current - 18, shipyard.size()); break; //current part is below the considered
				case 3: cons = safeC(current + 1, shipyard.size()); break; //current part is to the left of the considered
				}
				if (shipyard[cons].index > 0 && !shipyard[cons].valid) { 
					if (connectedParts(shipyard[current], shipyard[cons], i)) {
						open_list.push_back(cons);
					}
				}
			}
		}
	}
	else {
		return false;
	}
	for (int i = 0; i < shipyard.size(); i++) {
		if (!shipyard[i].valid) {
			return false;
		}
	}
	return true;
}

std::vector<Part> initParts() {
	std::vector<Part> parts;

	Connection all(true, true, true, true);
	//Part(std::string n = "Nothing", int dura = 25, Connection c(), bool crucial = false) {

	parts.push_back(Part("Nothing"));
	parts.push_back(Part("Command Module", 25, 25, all, true));
	parts.push_back(Part("Weapons Platform", 35, 20));
	parts[parts.size() - 1].turret = true;
	parts.push_back(Part("Large Thruster", 40, 30, Connection(true, false, false, false), false, 40));
	parts.push_back(Part("Fuel Cell", 20, 25, all));
	parts.push_back(Part("Cargo Cell", 25, 25, all));
	parts.push_back(Part("Armor Plating", 250, 20));
	parts.push_back(Part("Small Thruster", 20, 15, Connection(true, false, false, false), false, 20));
	parts.push_back(Part("Shield Generator", 15, 20));
	parts.push_back(Part("Radiator", 20, 15, Connection(true, false, true, false)));
	parts.push_back(Part("Solar Panel", 20, 15, Connection(true, false, true, false)));

	for (int i = 0; i < parts.size(); i++) {
		parts[i].sY = 32 * i;
		parts[i].index = i;
	}

	return parts;
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
	Ship player;
	Planet p(0, 0, 1500);
    float elapsedTime = 0.0f;
    float targetTime = .01f;
	bool accelerated = false;
	std::vector<Ship*> things = {&player};
	std::vector<Part> parts = initParts();
	std::vector<Part> shipyard = {};
	Bullet* bullets = nullptr; //Linked list of all projectiles
	bool in_shipyard = true;
	int index = 0;
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
				if (event.key.code == sf::Keyboard::R) {
					shipyard[index].rotation = (shipyard[index].rotation + 1) % 4;
					valid_design(shipyard);
				}
				if (in_shipyard) {
					if (event.key.code == sf::Keyboard::Up) {
						shipyard[index] = parts[safeC(shipyard[index].index + 1, parts.size())];
						valid_design(shipyard);
					}
					else if (event.key.code == sf::Keyboard::Down) {
						shipyard[index] = parts[safeC(shipyard[index].index - 1, parts.size())];
						valid_design(shipyard);
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
				if (event.key.code == sf::Keyboard::Enter) {
					if (valid_design(shipyard)) {
						in_shipyard = false;
						for (int i = 0; i < shipyard.size(); i++) {
							shipyard[i].x = (i % 18) * 16;
							shipyard[i].y = (i / 18) * 16;
							if (shipyard[i].index > 0) {
								player.parts.push_back(shipyard[i]);
							}
						}
					}
				}
			}
			else if (event.type == sf::Event::MouseWheelMoved) {
				if (event.mouseWheel.delta < 0 && zoom < 2.0) {
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
		mX = player.cx + sf::Mouse::getPosition().x*zoom - view.getSize().x / 2;
		mY = player.cy + sf::Mouse::getPosition().y*zoom - view.getSize().y / 2;
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
			targetGuns(&player, mX, mY);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				for (Part p : player.parts) {
					if (p.turret) {
						Bullet* b = new Bullet(p.x, p.y, p.turret_angle + rad(90), hypot(p.dx, p.dy) + 1);
						b->x += cos(b->angle) *6;
						b->y += sin(b->angle) *6;
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
			if (player.parts.size() > 0) {
				moveShip(&player, up, down, left, right);
			}
			
		}
		
		if (!in_shipyard) {
			view.setCenter(player.cx, player.cy);
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
		}
		else {
			for (Ship* s : things) {
				drawShip(s);
				if (s->parts.size() > 0) {
					
					//Print(to_str(s->parts[0].da), 1920 / 2, 1080 / 2, 4);
				}
			}
			Bullet* b = bullets;
			while (b != nullptr) {
				drawBullet(b);
				b = b->next;
			}
			drawPlanet(&p, view);
			//drawStars(view.getCenter().x, view.getCenter().y, view.getSize().x, view.getSize().y);
		}
		//Print(to_str((int)FPS), view.getCenter().x, view.getCenter().y);
		Print(to_str((int)player.cy), view.getCenter().x - 50, view.getCenter().y - view.getSize().y * .4);
		Print(to_str((int)player.cx), view.getCenter().x + 50, view.getCenter().y - view.getSize().y * .4);
        window.display();
    }

    return 0;
}