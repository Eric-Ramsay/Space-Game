float half(float x1, float x2, float m1, float m2) {
	return ((x1 * m1) + (x2 * m2)) / (m1 + m2);
}

void dock(Ship* ship, Ship* station) {
	ship->docked_at = station;
	station->cx = half(station->cx, ship->cx, station->mass, ship->mass);
	station->cy = half(station->cy, ship->cy, station->mass, ship->mass);

	ship->cx = half(station->cx, ship->cx, station->mass, ship->mass);
	ship->cy = half(station->cy, ship->cy, station->mass, ship->mass);

	station->pos.da = half(station->pos.da, ship->pos.da, station->mass, ship->mass);
	station->pos.dx = half(station->pos.dx, ship->pos.dx, station->mass, ship->mass);
	station->pos.dy = half(station->pos.dy, ship->pos.dy, station->mass, ship->mass);

	ship->pos.da = half(station->pos.da, ship->pos.da, station->mass, ship->mass);
	ship->pos.dx = half(station->pos.dx, ship->pos.dx, station->mass, ship->mass);
	ship->pos.dy = half(station->pos.dy, ship->pos.dy, station->mass, ship->mass);
}

void checkCollision(Ship* a, Ship* b) {
	int max_dist = a->parts.size();
	if (b->parts.size() > max_dist) {
		max_dist = b->parts.size();
	}
	max_dist *= 16;
	if (fabs(a->cy - b->cy) < max_dist) {
		if (fabs(a->cx - b->cx) < max_dist) {
			for (Part pa : a->parts) {
				for (Part pb : b->parts) {
					if (overlap(pa.pos, 8, pb.pos, 8)) {
						if (pa.name == "Docking Hatch" && pb.name == "Docking Hatch") {
							if (a->station && !b->station) {
								if (b->docking) {
									dock(b, a);
								}
							}
							else if (b->station && !a->station) {
								if (a->docking) {
									dock(a, b);
								}
							}
						}
					}
				}
			}
		}
	}
}

void applyGravity(Ship* s, Planet plan) {
	float d = dist(plan.x, plan.y, s->cx, s->cy);
	if (d <= 20 * plan.size) {
		float angle = atan2(s->cy - plan.y, s->cx - plan.x);
		float dx = (6 * plan.size) / (1000 + d * d);
		float dy = (6 * plan.size) / (1000 + d * d);
		for (Part& p : s->parts) {
			p.pos.dx += dx * -cos(angle - p.pos.a);
			p.pos.dy += dy * sin(angle - p.pos.a);
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

void calculateMass(Ship* s) {
	float mx = 0.0;
	float my = 0.0;
	float mass = 0.0;
	for (Part& p : s->parts) {
		if (s->docked_at != nullptr) {
			p.pos.da = parent_pos.da;
			p.pos.dx = parent_pos.dx;
			p.pos.dy = parent_pos.dy;
		}
		updatePosition(p.pos);
		mass += p.mass;
		mx += p.mass * p.pos.x;
		my += p.mass * p.pos.y;
	}
	if (mass > 0) {
		mx /= mass;
		my /= mass;
	}
	s->cx = mx;
	s->cy = my;
	s->mass = mass;
}

void moveShip(Ship* s, bool up = false, bool down = false, bool left = false, bool right = false) {
	//Calculate center of mass and center of thrust
	float da = 0;
	float dx = 0;
	float dy = 0;

	//First, calculate center of mass
	Position parent_pos;
	if (s->docked_at != nullptr) {
		parent_pos = s->docked_at->parts[0].pos;
	}
	for (Part& p : s->parts) {
		if (s->docked_at != nullptr) {
			p.pos.da = parent_pos.da;
			p.pos.dx = parent_pos.dx;
			p.pos.dy = parent_pos.dy;
		}
		updatePosition(p.pos);
	}
	calculateMass(s);

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
				float angle = atan2(p.pos.y - my, p.pos.x - mx) - p.pos.a;
				float x_force = -sin(angle) * dxt * dist(p.pos.x, p.pos.y, mx, my) / 2;
				float y_force = -cos(angle) * dyt * dist(p.pos.x, p.pos.y, mx, my) / 2;
				T += x_force;
				T += y_force;
				da += T / (10000 * s->mass);
				if ((da > 0 && p.pos.da < 0) || (da < 0 && p.pos.da > 0)) {
					da *= 2;
				}
				dx += dxt;
				dy += dyt;
			}
		}
	}
	dy /= 10 * s->mass;
	dx /= 10 * s->mass;
	if (abs(da) < 0.001) {
		da = 0;
	}

	sf::Vector2i m = sf::Mouse::getPosition();
	float d, angle;
	for (Part& p : s->parts) {
		float co = cos(p.pos.da);
		float si = sin(p.pos.da);
		p.pos.dy += dy;
		p.pos.dx += dx;
		float x_dif = p.pos.x - mx;
		float y_dif = p.pos.y - my;
		p.pos.x = co * x_dif - si * y_dif + mx;
		p.pos.y = si * x_dif + co * y_dif + my;
		p.pos.da += da;
	}
}
