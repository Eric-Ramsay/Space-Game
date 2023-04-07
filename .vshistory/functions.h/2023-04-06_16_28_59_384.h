float fX(Position f, float angle) {
	float x = sin(f.a) * f.dy + cos(f.a) * f.dx;
	float y = cos(f.a) * f.dy - sin(f.a) * f.dx;
	return x * cos(angle) - y * sin(angle);

}
float fY(Position f, float angle) {
	float x = sin(f.a) * f.dy + cos(f.a) * f.dx;
	float y = cos(f.a) * f.dy - sin(f.a) * f.dx;
	return x * sin(angle) + y * cos(angle);
}

float partW(Part& p, float a) {
	float w = parts[p.i].w;
	float h = parts[p.i].h;
	if (p.rotation % 2 != 0) {
		w = h;
		h = parts[p.i].w;
	}
	if (a == 0) {
		return w;
	}
	return abs(w * cos(a) - h * sin(a));
}

float partH(Part& p, float a) {
	float w = parts[p.i].w;
	float h = parts[p.i].h;
	if (p.rotation % 2 != 0) {
		w = h;
		h = parts[p.i].w;
	}
	if (a == 0) {
		return w;
	}
	return abs(w * sin(a) + h * cos(a));
}

float offX(Part p) {
	if (p.rotation == 1) {
		return parts[p.i].offY;
	}
	else if (p.rotation == 3) {
		return -parts[p.i].offY;
	}
	return 0;
}

float offY(Part p) {
	if (p.rotation == 0) {
		return -parts[p.i].offY;
	}
	else if (p.rotation == 2) {
		return parts[p.i].offY;
	}
	return 0;
}

float partX(Position pos, Part p) {
	float co = cos(pos.a);
	float si = sin(pos.a);
	return (co * (p.x + offX(p)) - si * (p.y + offY(p)) + pos.x);
}

float partY(Position pos, Part p) {
	float co = cos(pos.a);
	float si = sin(pos.a);
	return (si * (p.x + offX(p)) + co * (p.y + offY(p)) + pos.y);
}

float tLX(Part p, Position ship) {
	float x = partX(ship, p);
	short w = parts[p.i].w;
	short h = parts[p.i].h;
	if (p.rotation % 2 != 0) {
		h = w;
		w = parts[p.i].h;
	}
	return x - ((w / 2) * cos(ship.a) - (h / 2) * sin(ship.a));
}
float tLY(Part p, Position ship) {
	float y = partY(ship, p);
	short w = parts[p.i].w;
	short h = parts[p.i].h;
	if (p.rotation % 2 != 0) {
		h = w;
		w = parts[p.i].h;
	}
	return y - ((w / 2) * sin(ship.a) + (h / 2) * cos(ship.a));
}
float tRX(Part p, Position ship) {
	float x = partX(ship, p);
	short w = parts[p.i].w;
	short h = parts[p.i].h;
	if (p.rotation % 2 != 0) {
		h = w;
		w = parts[p.i].h;
	}
	return x + ((w / 2) * cos(ship.a) + (h / 2) * sin(ship.a));
}
float tRY(Part p, Position ship) {
	float y = partY(ship, p);
	short w = parts[p.i].w;
	short h = parts[p.i].h;
	if (p.rotation % 2 != 0) {
		h = w;
		w = parts[p.i].h;
	}
	return y - (-(w / 2) * sin(ship.a) + (h / 2) * cos(ship.a));
}
float bLX(Part p, Position ship) {
	float x = partX(ship, p);
	short w = parts[p.i].w;
	short h = parts[p.i].h;
	if (p.rotation % 2 != 0) {
		h = w;
		w = parts[p.i].h;
	}
	return x - ((w / 2) * cos(ship.a) + (h / 2) * sin(ship.a));
}
float bRX(Part p, Position ship) {
	float x = partX(ship, p);
	short w = parts[p.i].w;
	short h = parts[p.i].h;
	if (p.rotation % 2 != 0) {
		h = w;
		w = parts[p.i].h;
	}
	return x + ((w / 2) * cos(ship.a) - (h / 2) * sin(ship.a));
}
float bLY(Part p, Position ship) {
	float y = partY(ship, p);
	short w = parts[p.i].w;
	short h = parts[p.i].h;
	if (p.rotation % 2 != 0) {
		h = w;
		w = parts[p.i].h;
	}
	return y + (-(w / 2) * sin(ship.a) + (h / 2) * cos(ship.a));
}
float bRY(Part p, Position ship) {
	float y = partY(ship, p);
	short w = parts[p.i].w;
	short h = parts[p.i].h;
	if (p.rotation % 2 != 0) {
		h = w;
		w = parts[p.i].h;
	}
	return y + ((w / 2) * sin(ship.a) + (h / 2) * cos(ship.a));
}

Rect createRect(Part p, Position ship) {
	return Rect(Point(tLX(p, ship), tLY(p, ship)), Point(tRX(p, ship), tRY(p, ship)), Point(bLX(p, ship), bLY(p, ship)), Point(bRX(p, ship), bRY(p, ship)));
}

float isLeft(Point P, Point A, Point B) {
	//std::cout << (P.x - A.x) * (B.y - A.y) - (P.y - A.y) * (B.x - A.x) << std::endl;
	//return (P.x - A.x) * (B.y - A.y) - (P.y - A.y) * (B.x - A.x);
	return (B.x - A.x) * (P.y - A.y) - (P.x - A.x) * (B.y - A.y);
}

//Checks if a point is inside of a rectangle
bool bounded(Part p, Position ship, float x, float y, float w = 1, float h = 1) {
	Rect part = createRect(p, ship);
	Point P = Point(x, y);
	Point A = part.tl;
	Point B = part.tr;
	Point C = part.br;
	Point D = part.bl;

	return isLeft(P, A, D) <= 0 && isLeft(P, C, B) <= 0 && isLeft(P, B, A) <= 0 && isLeft(P, D, C) <= 0;
}

//Checks if two parts overlap by checking if the corner of each part is inside of a rectangle
bool bounded(Part p1, Position ship1, Part p2, Position ship2) {
	Rect part = createRect(p2, ship2);
	//std::cout << "--- tl ---" << std::endl;
	bool tl = bounded(p1, ship1, part.tl.x, part.tl.y);
	//std::cout << "--- tr ---" << std::endl;
	bool tr = bounded(p1, ship1, part.tr.x, part.tr.y);
	//std::cout << "--- bl ---" << std::endl;
	bool bl = bounded(p1, ship1, part.bl.x, part.bl.y);
	//std::cout << "--- br ---" << std::endl;
	bool br = bounded(p1, ship1, part.br.x, part.br.y);

	if (tl || tr || bl || br) {
		return true;
	}
	else {
		part = createRect(p1, ship1);
		//std::cout << "--- tl1 ---" << std::endl;
		bool tl1 = bounded(p2, ship2, part.tl.x, part.tl.y);
		//std::cout << "--- tr1 ---" << std::endl;
		bool tr1 = bounded(p2, ship2, part.tr.x, part.tr.y);
		//std::cout << "--- bl1 ---" << std::endl;
		bool bl1 = bounded(p2, ship2, part.bl.x, part.bl.y);
		//std::cout << "--- br1 ---" << std::endl;
		bool br1 = bounded(p2, ship2, part.br.x, part.br.y);
		if (tl1 || tr1 || bl1 || br1) {
			//std::cout << "wtf" << std::endl;
			return true;
		}
	}
}

float dist(Position a, Position b) {
	return dist(a.x, a.y, b.x, b.y);
}

/*bool overlap(Part pa, Position a, float x2, float y2, float w2, float h2) {
	float x1 = partX(a, pa);
	float y1 = partY(a, pa);
	short w1 = partW(pa, a.a);
	short h1 = partH(pa, a.a);

	return overlap(x1, y1, w1/2.0, h1/2.0, x2, y2, w2, h2);
}	

bool overlap(Part pa, Position a, Part pb, Position b) {
	float x1 = partX(a, pa);
	float y1 = partY(a, pa);
	short w1 = partW(pa, a.a);
	short h1 = partH(pa, a.a);

	float x2 = partX(b, pb);
	float y2 = partY(b, pb);
	short w2 = partW(pb, b.a);
	short h2 = partH(pb, b.a);

	return overlap(x1, y1, w1/2.0, h1/2.0, x2, y2, w2/2.0, h2/2.0);
}*/

unsigned long long int d2(Position a, Position b) {
	return d2(a.x, a.y, b.x, b.y);
}


bool planetClose(Position a, Planet p, int range) {
	return abs((int)p.x - (int)a.x) < range && abs((int)p.y - (int)a.y) < range;
}

//calculates 0 to 1 value for how closely an angle is moving toward a position
float posA(Position a, Position b) {
	float ang = atan2(-a.dy, a.dx);
	float ang1 = atan2(b.y - a.y, b.x - a.x);
	return (abs(rad(180) - abs(ang - ang1))) / rad(180);
}

float multX(Position a, Position b) {
	float dirA = atan2(-a.dy, a.dx);
	float dirB = atan2(-b.dy, b.dx);
	float pos = atan2(b.y - a.y, b.x - a.x);
	return posA(a, b)  * abs(cos(dirA));
}

float multY(Position a, Position b) {
	float dirA = atan2(-a.dy, a.dx);
	float dirB = atan2(-b.dy, b.dx);
	float pos = atan2(b.y - a.y, b.x - a.x);
	return posA(a, b) * abs(sin(dirA));
}


void updateExplosion(Explosion& e) {
	e.size++;
	int n = e.points.size();
	float angle = 0;
	if (e.size < e.max) {
		for (int i = 0; i < n; i++) {
			angle += (2 * PI) / n;
			e.points[i].x = e.x + ((2 + (rand() % 70) / 100.0) * e.size) * cos(angle);
			e.points[i].y = e.y + ((2 + (rand() % 70) / 100.0) * e.size) * sin(angle);
		}
	}
	else {
		e.max -= .001;
	}
}