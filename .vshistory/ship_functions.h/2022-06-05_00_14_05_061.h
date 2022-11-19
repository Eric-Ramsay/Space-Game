float half(float x1, float x2, float m1, float m2) {
	return ((x1 * m1) + (x2 * m2)) / (m1 + m2);
}

void link(Ship* a, Ship* b) {
	std::vector<Ship*> ships = b->connected;
	ships.push_back(b);
	for (Ship* s : ships) {
		if (s != a) {
			bool found = false;
			for (Ship* check : a->connected) {
				if (s == check) {
					found = true;
				}
			}
			if (!found) {
				a->connected.push_back(s);
			}
		}
	}
}

void updatePosition(Position& pos) {
	pos.a += pos.da;
	pos.x += sin(pos.a) * pos.dy;
	pos.y -= cos(pos.a) * pos.dy;
	pos.x += cos(pos.a) * pos.dx;
	pos.y += sin(pos.a) * pos.dx;
}

void collide(Ship* a, Ship* b, bool recursive = true) {
	float ax, ay, bx, by, angle, si, co, mass;
	mass = a->mass + b->mass;
	angle = b->pos.a - a->pos.a;
	si = sin(angle);
	co = cos(angle);
	bx = si * b->pos.dy * b->mass + co * b->pos.dx * b->mass;
	by = co * b->pos.dy * b->mass - si * b->pos.dx * b->mass;
	

	a->pos.dx = (.5 * b->mass * (dx - a->pos.dx) + a->mass * a->pos.dx + b->mass * dx) / mass;
	a->pos.dy = (.5 * b->mass * (dy - a->pos.dx) + a->mass * a->pos.dx + b->mass * dx) / mass;

	if (recursive) {
		collide(b, a, false);
	}
	else {
		a->pos.dx = (.5 * b->mass * (dx - a->pos.dx) + a->mass * a->pos.dx + b->mass * dx) / mass;
		a->pos.dy = (.5 * b->mass * (dy - a->pos.dx) + a->mass * a->pos.dx + b->mass * dx) / mass;
	}

	//velocity_a_final = (.5 * mass_b * (vel_b - vel_a) + mass_a * vel_a  + mass_b * vel_b) / (total_mass);
	//velocity_b_final = (.5 * mass_a * (vel_a - vel_b) + mass_a * vel_a  + mass_b * vel_b) / (total_mass);
	
}

void dockPos(Ship* ship) {
	float mass = ship->mass;
	float dx = ship->pos.dx * mass;
	float dy = ship->pos.dy * mass;
	float da = ship->pos.da * mass;
	for (Ship* s : ship->connected) {
		float a = s->pos.a - ship->pos.a;
		dx += sin(a) * s->pos.dy * s->mass;
		dx += cos(a) * s->pos.dx * s->mass;

		dy += cos(a) * s->pos.dy * s->mass;
		dy -= sin(a) * s->pos.dx * s->mass;

		da += s->pos.da * s->mass;
		mass += s->mass;
	}
	dx /= mass;
	dy /= mass;
	da /= mass;

	ship->pos.dx = dx;
	ship->pos.dy = dy;
	ship->pos.da = da;
}

void calculateMass(Ship* s) {
	float mx = 0.0;
	float my = 0.0;
	float mass = 0.0;
	for (Part& p : s->parts) {
		mass += p.mass;
		mx += p.mass * p.pos.x;
		my += p.mass * p.pos.y;
	}

	s->x = mx / mass;
	s->y = my / mass;
	s->mass = mass;

	for (Ship* ship : s->connected) {
		for (Part& p : ship->parts) {
			mass += p.mass;
			mx += p.mass * p.pos.x;
			my += p.mass * p.pos.y;
		}
	}
	if (mass > 0) {
		mx /= mass;
		my /= mass;
	}
	s->pos.x = mx;
	s->pos.y = my;
	s->e_mass = mass;
}

void checkCollision(Ship* a, Ship* b) {
	int max_dist = a->parts.size();
	if (b->parts.size() > max_dist) {
		max_dist = b->parts.size();
	}
	max_dist *= 16;
	if (fabs(a->y - b->y) < max_dist) {
		if (fabs(a->x - b->x) < max_dist) {
			for (Part pa : a->parts) {
				for (Part pb : b->parts) {
					if (overlap(pa.pos, 8, pb.pos, 8)) {
						if (pa.name == "Docking Hatch" && pb.name == "Docking Hatch") {
							if (a->docking && b->docking) {
								link(a, b);
								link(b, a);
							}
						}
						else {
							link(a, b);
							link(b, a);
						}
					}
				}
			}
		}
	}
}

void applyGravity(Ship* s, Planet plan) {
	float d = dist(plan.x, plan.y, s->pos.x, s->pos.y);
	if (d <= 20 * plan.size) {
		float angle = atan2(s->pos.y - plan.y, s->pos.x - plan.x);
		float dx = (6 * plan.size) / (1000 + d * d);
		float dy = (6 * plan.size) / (1000 + d * d);
		for (Part& p : s->parts) {
			p.pos.dx += dx * -cos(angle - p.pos.a);
			p.pos.dy += dy * sin(angle - p.pos.a);
		}
	}
}

void moveShip(Ship* s, bool up = false, bool down = false, bool left = false, bool right = false) {
	//Calculate center of mass and center of thrust
	float da = 0;
	float dx = 0;
	float dy = 0;

	if (s->parts.size() > 0) {
		s->pos.a = s->parts[0].pos.a;
	}
	calculateMass(s);
	dockPos(s);
	for (Part& p : s->parts) {
		p.pos.da = s->pos.da;
		p.pos.dx = s->pos.dx;
		p.pos.dy = s->pos.dy;
		updatePosition(p.pos);
	}
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
				float angle = atan2(p.pos.y - s->pos.y, p.pos.x - s->pos.x) - p.pos.a;
				float x_force = -sin(angle) * dxt * dist(p.pos.x, p.pos.y, s->pos.x, s->pos.y) / 2;
				float y_force = -cos(angle) * dyt * dist(p.pos.x, p.pos.y, s->pos.x, s->pos.y) / 2;
				T += x_force;
				T += y_force;
				da += T / (5000 * s->e_mass);
				if ((da > 0 && p.pos.da < 0) || (da < 0 && p.pos.da > 0)) {
					da *= 2;
				}
				dx += dxt;
				dy += dyt;
			}
		}
	}
	dy /= 10 * s->e_mass;
	dx /= 10 * s->e_mass;
	if (abs(da) > 0 && abs(da) < 0.0001) {
		da = 0;
	}

	s->pos.dy += dy;
	s->pos.dx += dx;
	s->pos.da += da;
	dockPos(s);
	for (Part& p : s->parts) {
		float co = cos(p.pos.da);
		float si = sin(p.pos.da);
		float x_dif = p.pos.x - s->pos.x;
		float y_dif = p.pos.y - s->pos.y;
		p.pos.x = co * x_dif - si * y_dif + s->pos.x;
		p.pos.y = si * x_dif + co * y_dif + s->pos.y;
	}
	
}

void targetGuns(Ship* s, float x, float y) {
	for (Part& p : s->parts) {
		if (p.turret) {
			float angle = rad(90) + atan2(p.pos.y - y, p.pos.x - x);
			p.turret_angle = angle;
		}
	}
}

void moveBullet(Bullet* b) {
	updatePosition(b->pos);
	b->power--;
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
		std::deque<int> open_list = { command };
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
	parts.push_back(Part("Ship Control", 25, 25, all, true));
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
	parts.push_back(Part("Docking Hatch", 20, 15, Connection(true, false, false, false)));
	parts.push_back(Part("Station Control", 25, 25, all, true));
	for (int i = 0; i < parts.size(); i++) {
		parts[i].sY = 32 * i;
		parts[i].index = i;
	}

	return parts;
}

