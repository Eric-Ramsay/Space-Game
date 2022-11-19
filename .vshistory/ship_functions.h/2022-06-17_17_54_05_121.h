void moveShip(Ship* s, bool up = false, bool down = false, bool left = false, bool right = false);

float half(float x1, float x2, float m1, float m2) {
	return ((x1 * m1) + (x2 * m2)) / (m1 + m2);
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

	for (Ship* s : ship->connected) {
		s->pos.dx = dx;
		s->pos.dy = dy;
		s->pos.da = da;
	}
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
	dockPos(a);
}

void updatePosition(Position& pos) {
	pos.a += pos.da;
	pos.x += sin(pos.a) * pos.dy + cos(pos.a) * pos.dx;
	pos.y += sin(pos.a) * pos.dx - cos(pos.a) * pos.dy;
}


void force(Ship* s, Position f, bool recurs = true) {
	float x = sin(f.a) * f.dy + cos(f.a) * f.dx;
	float y = cos(f.a) * f.dy - sin(f.a) * f.dx;

	float dx = x * cos(s->pos.a) - y * sin(s->pos.a);
	float dy = x * sin(s->pos.a) + y * cos(s->pos.a);

	//float dx = sin(s->pos.a) * y + cos(s->pos.a) * x;
	//float dy = sin(s->pos.a) * x - cos(s->pos.a) * y;

	s->pos.dx += dx / s->e_mass;
	s->pos.dy += dy / s->e_mass;
	//s->pos.da += f.da / s->e_mass;
	//s->pos.dx += f.dx / s->e_mass;
	//s->pos.dy += f.dy / s->e_mass;

	float angle = atan2(s->y - f.y, s->x - f.x) - f.a;
	float d = min(50, dist(f.x, f.y, s->x, s->y)) / 100;
	float x_force = sin(angle) * d * f.dx;
	float y_force = cos(angle) * d * f.dy;
	if (abs(x_force) < .001) {
		x_force = 0;
	}
	if (abs(y_force) < .001) {
		y_force = 0;
	}
	s->pos.da += (x_force + y_force) / (30 * s->e_mass);

	if (recurs) {
		for (Ship* ship : s->connected) {
			force(ship, f, false);
		}
	}
}

void collide(Ship* a, Ship* b) {
	Position aF = a->pos * a->mass;
	Position bF = b->pos * b->mass;

	aF.dx = sin(aF.a) * aF.dy + cos(aF.a) * aF.dx;
	aF.dy = cos(aF.a) * aF.dy - sin(aF.a) * aF.dx;

	bF.dx = sin(bF.a) * bF.dy + cos(bF.a) * bF.dx;
	bF.dy = cos(bF.a) * bF.dy - sin(bF.a) * bF.dx;

	aF.a = 0; bF.a = 0;

	force(b, aF);
	aF.x = b->pos.x;
	aF.y = b->pos.y;
	force(a, aF * -1);
}


void calculateMass(Ship* s) {
	double mx = 0.0;
	double my = 0.0;
	float mass = 0.0;
	for (Part& p : s->parts) {
		mass += parts[p.i].mass;
		mx += parts[p.i].mass * p.x;
		my += parts[p.i].mass * p.y;
	}

	s->x = mx / mass;
	s->y = my / mass;
	s->mass = mass;
}

void dockMass(Ship* s) {
	if (s->connected.size() == 0) {
		s->x = 0;
		s->y = 0;
		s->e_mass = s->mass;
		return;
	}
	double mx = s->x * s->mass;
	double my = s->y * s->mass;
	float mass = s->mass;
	for (Ship* ship : s->connected) {
		mass += ship->mass;
		mx += ship->x;
		my += ship->y;
	}
	if (mass > 0) {
		mx /= mass;
		my /= mass;
	}
	s->x = (float)mx;
	s->y = (float)my;
	s->e_mass = mass;
}

bool future_collision(Ship* a_src, Ship* b_src, int steps = 0) {
	Ship a = *a_src;
	Ship b = *b_src;
	for (int i = 0; i < steps; i++) {
		moveShip(&a);
		moveShip(&b);
		for (Part pa : a.parts) {
			for (Part pb : b.parts) {
				if (bounded(pa, a_src->pos, pb, b_src->pos)) {
					if (parts[pa.i].name != "Docking Hatch" || parts[pb.i].name != "Docking Hatch" || !a.docking || !b.docking) {
						//collide(a_src, b_src);
						return true;
					}
				}
			}
		}
	}
	if (steps == 0) {
		for (Part pa : a.parts) {
			for (Part pb : b.parts) {
				if (bounded(pa, a_src->pos, pb, b_src->pos)) {
					if (parts[pa.i].name != "Docking Hatch" || parts[pb.i].name != "Docking Hatch" || !a.docking || !b.docking) {
						//collide(a_src, b_src);
						return true;
					}
				}
			}
		}
	}
	return false;
}

void checkCollision(std::vector<Ship*> ships) {
	std::vector<std::vector<Ship*>> collisions = {};
	for (Ship* a : ships) {
		a->connected = {};
		for (Ship* b : ships) {
			if (a != b) {
				bool can_collide = true;
				for (int i = 0; i < collisions.size(); i++) {
					if (collisions[i][0] == a || collisions[i][1] == a) {
						if (collisions[i][0] == b || collisions[i][1] == b) {
							can_collide = false;
							i = collisions.size();
						}
					}
				}
				if (can_collide) {
					int max_dist = a->parts.size() + b->parts.size() * 16;
					if (dist(a->pos, b->pos) < max_dist) {
						if (future_collision(a, b, 1)) {
							can_collide = false;
							//collisions.push_back({ a, b });
						}
						for (Part pa : a->parts) {
							for (Part pb : b->parts) {
								if (bounded(pa, a->pos, pb, b->pos)) {
									if (parts[pa.i].name == "Docking Hatch" && parts[pb.i].name == "Docking Hatch") {
										if (a->docking && b->docking) {
											link(a, b);
											link(b, a);
										}
									}
									else {
										if (can_collide) {
											//collide(a, b);
											//can_collide = false;
											//collisions.push_back({ a, b });
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
}

bool planetaryCollision(Ship* s, Planet plan) {
	if (planetClose(s->pos, plan, plan.size + s->parts.size() * 16)) {
		for (Part p : s->parts) {
			float x = partX(s->pos, p);
			float y = partY(s->pos, p);
			float dis = dist(partX(s->pos, p), partY(s->pos, p), plan.x, plan.y);
			if (dis < plan.size) {
				return true;
			}
		}
	}
	return false;
}



void applyGravity(Position& pos, Planet plan) {
	unsigned long long int d = d2(plan.x, plan.y, pos.x, pos.y);
	if (d < (25 * plan.size * plan.size)) {
		float angle = atan2(pos.y - plan.y, pos.x - plan.x);
		float dx = (6*plan.size) / (1000 + d);
		float dy = (6*plan.size) / (1000 + d);
		pos.dx += dx * -cos(angle - pos.a);
		pos.dy += dy * sin(angle - pos.a);
	}
}

void moveShip(Ship* s, bool up, bool down, bool left, bool right) {
	dockMass(s);
	for (Part& p : s->parts) {
		if (parts[p.i].thrust > 0) {
			p.active = false;
			if (p.rotation == 0 && up || p.rotation == 1 && right || p.rotation == 2 && down || p.rotation == 3 && left) {
				p.active = true;
				Position thrust;
				thrust.a = s->pos.a;
				//Pretty sure this is negative bc right and left are flipped if u go by 90 degree increments. whoops
				thrust.dx = parts[p.i].thrust * -cos((PI / 2 * (1 + p.rotation)));
				thrust.dy = parts[p.i].thrust * sin((PI / 2 * (1 + p.rotation)));
				thrust.x = partX(s->pos, p);
				thrust.y = partY(s->pos, p);
				force(s, thrust);
				if (s->energy > 0) {
					s->energy -= parts[p.i].thrust;
				}
				else {
					s->energy = 0;
				}
			}
		}
	}
	updatePosition(s->pos);
	float co = cos(s->pos.da);
	float si = sin(s->pos.da);
	float x_dif = s->pos.x - s->x;
	float y_dif = s->pos.y - s->y;
	s->pos.x = co * x_dif - si * y_dif + s->x;
	s->pos.y = si * x_dif + co * y_dif + s->y;
}

void targetGuns(Ship* s, float x, float y) {
	for (Part& p : s->parts) {
		if (parts[p.i].turret) {
			float angle = rad(90) + atan2(partY(s->pos, p) - y, partX(s->pos, p) - x);
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
	Connection a = rotConnect(parts[a1.i].connects, a1.rotation);
	Connection b = rotConnect(parts[b1.i].connects, b1.rotation);
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
	return true;
	int command = -1;
	for (int i = 0; i < shipyard.size(); i++) {
		if (shipyard[i].i > 0) {
			shipyard[i].valid = false;
		}
		else {
			shipyard[i].valid = true;
		}
	}
	for (int i = 0; i < shipyard.size(); i++) {
		if (shipyard[i].i > 0) {
			if (parts[shipyard[i].i].crucial) {
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
				if (shipyard[cons].i > 0 && !shipyard[cons].valid) {
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

float calcV(Position p, float x, float y, std::vector<Planet> planets, int iter = 60) {
	for (int i = 0; i < 3*iter; i++) {
		updatePosition(p);
		for (Planet plan : planets) {
			applyGravity(p, plan);
		}
	}
	return dist(x, y, p.x, p.y);
}

std::deque<PathPos> path(Ship a, float x, float y, std::vector<Planet> planets, bool slow_down = true, bool lax = true) {
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
		for (int i = 1; i < open.size(); i++) {
			comp = open[i].h;
			if (comp < val) {
				mind = i;
				val = comp;
			}
		}
		current = open[mind];
		open.erase(open.begin() + mind);
		closed.push_back(current);
		dis = dist(current.pos.x, current.pos.y, x, y);

		if (mindex == -1 || dis < closest) {
			mindex = closed.size() - 1;
			closest = dis;
		}
		if (dis < 10 || lax && dis < (10 + .1 * closed.size())) {
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
								int num = 10 + sqrt(2 * dis);
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
											if (slow_down) {
												ape.v = 2 * calcV(ape.pos, x, y, planets, ape.num) * max(.5, ((ape.g - ape.h) / tot_dist));
											}
											ape.f = cur_fuel - copy.energy;
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
