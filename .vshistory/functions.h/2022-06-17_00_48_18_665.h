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

float tLX(Part p, Position ship) {
	float x = p.x + ship.x;
	float w = parts[p.i].w;
	float h = parts[p.i].h;
	if (p.rotation % 2 != 0) {
		h = w;
		w = parts[p.i].h;
	}
	return x + w/2 * cos(p.a) - 
}
float tRX(Part p, Position ship) {
	float x = p.x + ship.x;
	float w = parts[p.i].w;
	if (p.rotation % 2 != 0) {
		w = parts[p.i].h;
	}
	return x + s2 * cos(ship.a - PI / 4) * w / 2.0;
}
float bLX(Part p, Position ship) {
	return -tRX(p, ship);
}
float bRX(Part p, Position ship) {
	return -tLX(p, ship);
}
float tLY(Part p, Position ship) {
	float y = p.y + ship.y;
	float h = parts[p.i].h;
	if (p.rotation % 2 != 0) {
		h = parts[p.i].w;
	}
	return y + s2 * cos(ship.a - PI / 4) * h / 2.0;
}
float bLY(Part p, Position ship) {
	float y = p.y + ship.y;
	float h = parts[p.i].h;
	if (p.rotation % 2 != 0) {
		h = parts[p.i].w;
	}
	return y + s2 * sin(ship.a - PI / 4) * h / 2.0;
}
float bRY(Part p, Position ship) {
	return -tLY(p, ship);
}
float tRY(Part p, Position ship) {
	return -bLY(p, ship);
}

Rect createRect(Part p, Position ship) {
	return Rect(Point(tLX(p, ship), tLY(p, ship)), Point(tRX(p, ship), tRY(p, ship)), Point(bLX(p, ship), bLY(p, ship)), Point(bRX(p, ship), bRY(p, ship)));
}

float minX(Rect r) {
	if (r.tl.x <= r.tr.x && r.tl.x <= r.bl.x && r.tl.x <= r.br.x) {
		return r.tl.x;
	}
	if (r.tr.x <= r.tl.x && r.tr.x <= r.bl.x && r.tr.x <= r.br.x) {
		return r.tr.x;
	}
	if (r.bl.x <= r.tr.x && r.bl.x <= r.tl.x && r.bl.x <= r.br.x) {
		return r.bl.x;
	}
	if (r.br.x <= r.tr.x && r.br.x <= r.bl.x && r.br.x <= r.tl.x) {
		return r.br.x;
	}
}

float minY(Rect r) {
	if (r.tl.y <= r.tr.y && r.tl.y <= r.bl.y && r.tl.y <= r.br.y) {
		return r.tl.y;
	}
	if (r.tr.y <= r.tl.y && r.tr.y <= r.bl.y && r.tr.y <= r.br.y) {
		return r.tr.y;
	}
	if (r.bl.y <= r.tr.y && r.bl.y <= r.tl.y && r.bl.y <= r.br.y) {
		return r.bl.y;
	}
	if (r.br.y <= r.tr.y && r.br.y <= r.bl.y && r.br.y <= r.tl.y) {
		return r.br.y;
	}
}

float maxX(Rect r) {
	if (r.tl.x >= r.tr.x && r.tl.x >= r.bl.x && r.tl.x >= r.br.x) {
		return r.tl.x;
	}
	if (r.tr.x >= r.tl.x && r.tr.x >= r.bl.x && r.tr.x >= r.br.x) {
		return r.tr.x;
	}
	if (r.bl.x >= r.tr.x && r.bl.x >= r.tl.x && r.bl.x >= r.br.x) {
		return r.bl.x;
	}
	if (r.br.x >= r.tr.x && r.br.x >= r.bl.x && r.br.x >= r.tl.x) {
		return r.br.x;
	}
}

float maxY(Rect r) {
	if (r.tl.y >= r.tr.y && r.tl.y >= r.bl.y && r.tl.y >= r.br.y) {
		return r.tl.y;
	}
	if (r.tr.y >= r.tl.y && r.tr.y >= r.bl.y && r.tr.y >= r.br.y) {
		return r.tr.y;
	}
	if (r.bl.y >= r.tr.y && r.bl.y >= r.tl.y && r.bl.y >= r.br.y) {
		return r.bl.y;
	}
	if (r.br.y >= r.tr.y && r.br.y >= r.bl.y && r.br.y >= r.tl.y) {
		return r.br.y;
	}
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

	float left = isLeft(P, A, D);
	float right = isLeft(P, C, B);
	float top = isLeft(P, B, A);
	float bot = isLeft(P, D, C);

	return left > 0 && right > 0 && top > 0 && bot > 0;
}

float partX(Position pos, Part p) {
	float co = cos(pos.a);
	float si = sin(pos.a);
	float offset = 0;
	return (co * (p.x) - si * p.y + pos.x);
}

float partY(Position pos, Part p) {
	float co = cos(pos.a);
	float si = sin(pos.a);
	return (si * p.x + co * p.y + pos.y);
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