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

float isLeft(Point pt, Point v1, Point v2) {
	return (v2.x - v1.x) * (pt.y - v1.y) - (pt.x - v1.x) * (v2.y - v1.y);
}

bool bounded(Part p, Position ship, float x, float y, float w = 1, float h = 1) {
	Rect part = createRect(p, ship);
	Point P = Point(x, y);
	Point A = part.tl;
	Point B = part.tr;
	Point C = part.br;
	Point D = part.bl;

	return isLeft(P, A, D) < 0 && isLeft(P, C, B) < 0 && isLeft(P, B, A) < 0 && isLeft(P, D, C) < 0;
}

bool bounded(Part p1, Position ship1, Part p2, Position ship2) {
	Rect part = createRect(p2, ship2);

	return bounded(p1, ship1, part.tl.x, part.tl.y) || bounded(p1, ship1, part.tr.x, part.tr.y) || bounded(p1, ship1, part.bl .x, part.bl.y) || bounded(p1, ship1, part.br.x, part.br.y) ;
}

float dist(Position a, Position b) {
	return dist(a.x, a.y, b.x, b.y);
}

bool overlap(Part pa, Position a, float x2, float y2, float w2, float h2) {
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
}

unsigned long long int d2(Position a, Position b) {
	return d2(a.x, a.y, b.x, b.y);
}


bool planetClose(Position a, Planet p, int range) {
	return abs((int)p.x - (int)a.x) < range && abs((int)p.y - (int)a.y) < range;
}