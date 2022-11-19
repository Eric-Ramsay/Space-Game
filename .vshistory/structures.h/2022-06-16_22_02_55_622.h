#pragma once
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

struct Position {
	float x = 0;
	float dx = 0;

	float y = 0;
	float dy = 0;

	float a = 0;
	float da = 0;

	bool operator ==(Position rhs) {
		return close(x, rhs.x) && close(y, rhs.y) && close(dx, rhs.dx) && close(dy, rhs.dy);
	}

	void operator =(const Position& rhs) {
		x = rhs.x;
		y = rhs.y;
		dx = rhs.dx;
		dy = rhs.dy;
		a = rhs.a;
		da = rhs.da;
	}

	Position& operator *(float rhs)	{
		Position ape = *this;
		ape.dx *= rhs;
		ape.dy *= rhs;
		//ape.da *= rhs;
		
		return ape;
	}

	Position(float x1 = 0, float y1 = 0, float a1 = 0, float dx1 = 0, float dy1 = 0, float da1 = 0) {
		x = x1;
		y = y1;
		a = a1;
		dx = dx1;
		dy = dy1;
		da = da1;
	}
};


struct PathPos {
	int num = 0;
	Position pos;
	bool up;
	bool down;
	bool left;
	bool right;
	int parent = -1;
	float g = 0;
	float h = 0;
	float v = 0;
	float f = 0;
	bool operator ==(PathPos rhs) {
		return pos == rhs.pos;
	}
	PathPos(Position p = Position(), int par = -1, bool u = false, bool d = false, bool l = false, bool r = false) {
		pos = p;
		parent = par;
		up = u;
		down = d;
		left = l;
		right = r;
	}
};

struct UI_Part {
	std::string name = "";
	int sX = 0;
	int sY = 0;
	short w = 16;
	short h = 16;
	int cargo_space = 0;
	int durability = 0;
	float mass = 0;
	float thrust = 0;
	float active_energy = 0;
	float passive_energy = 0;
	Connection connects;
	bool turret = false;
	bool crucial = false;

	UI_Part(std::string n = "Nothing", short w1 = 16, short h1 = 16, int dura = 25, float m1 = 25, Connection c = Connection(true, false, false, false), bool cruc = false, float t1 = 0) {
		name = n;
		durability = dura;
		connects = c;
		crucial = cruc;
		mass = m1;
		thrust = t1;
		w = w1;
		h = h1;
	}
};

struct Rect {
	float tl_x = tLX(p, ship);
	float tl_y = tLY(p, ship);
	float tr_x = tRX(p, ship);
	float tr_y = tRY(p, ship);
	float bl_x = tLX(p, ship);
	float bl_y = tLX(p, ship);
	float br_x = tLX(p, ship);
	float br_y = tLX(p, ship);
};

Rect createRect(Part p, Position ship) {

}

struct Part {
	int i = -1;
	float x = 0;
	float y = 0;
	float damage = 0;
	bool active = false;
	int rotation = 2;
	bool valid = true;
	float turret_angle = 0.0;
};

std::vector<UI_Part> initParts() {
	std::vector<UI_Part> parts;
	Connection all(true, true, true, true);
	//UI_Part(std::string n = "Nothing", int dura = 25, Connection c(), bool crucial = false) {
	parts.push_back(UI_Part("Nothing"));
	parts.push_back(UI_Part("Ship Control", 16, 16, 25, 25, all, true));
	parts.push_back(UI_Part("Weapons Platform", 16, 15, 35, 20));
	parts[parts.size() - 1].turret = true;
	parts.push_back(UI_Part("Large Thruster", 14, 10, 40, 30, Connection(true, false, false, false), false, 4));
	parts.push_back(UI_Part("Fuel Cell", 16, 16, 20, 25, all));
	parts.push_back(UI_Part("Cargo Cell", 16, 16, 25, 25, all));
	parts.push_back(UI_Part("Armor Plating", 16, 8, 250, 20));
	parts.push_back(UI_Part("Small Thruster", 8, 8, 20, 15, Connection(true, false, false, false), false, 2));
	parts.push_back(UI_Part("Shield Generator", 14, 9, 15, 20));
	parts.push_back(UI_Part("Radiator", 14, 16, 20, 15, Connection(true, false, true, false)));
	parts.push_back(UI_Part("Solar Panel", 16, 16, 20, 15, Connection(true, false, true, false)));
	parts.push_back(UI_Part("Docking Hatch", 2, 16, 20, 15, Connection(true, false, false, false)));
	parts.push_back(UI_Part("Station Control", 16, 16, 25, 25, all, true));
	for (int i = 0; i < parts.size(); i++) {
		parts[i].sY = 32 * i;
	}

	return parts;
}

struct Bullet {
	sf::Color color;
	Position pos;
	int power = 1000;
	Bullet* next;

	Bullet(Position p, float pow = 1000, sf::Color c = sf::Color(255, 0, 0)) {
		pos = p;
		power = pow;
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

struct Track {
	std::string msg = "";
	Position pos;
	Track(std::string m, Position p) {
		pos = p;
		msg = m;
	}
};

struct Ship {
	bool station = false;
	bool docking = true;
	Position pos;

	float mass = 0.0;
	float energy = 100000;
	float e_mass = 0.0;
	float x = 0.0;
	float y = 0.0;
	std::vector<Ship*> connected = {};
	std::vector<Part> parts = {};

	std::deque<PathPos> path = {};

	std::vector<Track> HUD = {};

	bool pursue_path = false;
	float tX = 0.0;
	float tY = 0.0;
};
