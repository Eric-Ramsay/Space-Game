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
	if (p.rotation % 2 != 0) {
		w = parts[p.i].h;
	}
	return x + sin(ship.a - PI/4) * w / 2.0;
}
float tRX(Part p, Position ship) {
	float x = p.x + ship.x;
	float w = parts[p.i].w;
	if (p.rotation % 2 != 0) {
		w = parts[p.i].h;
	}
	return x + cos(ship.a - PI / 4) * w / 2.0;
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
	return y + cos(ship.a - PI / 4) * h / 2.0;
}
float bLY(Part p, Position ship) {
	float y = p.y + ship.y;
	float h = parts[p.i].h;
	if (p.rotation % 2 != 0) {
		h = parts[p.i].w;
	}
	return y + sin(ship.a - PI / 4) * h / 2.0;
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

float sign(Point p1, Point p2, Point p3) {
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool inTri(Point pt, Point v1, Point v2, Point v3) {
	bool b1, b2, b3;

	b1 = sign(pt, v1, v2) < 0.0f;
	b2 = sign(pt, v2, v3) < 0.0f;
	b3 = sign(pt, v3, v1) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}

float triArea(Point A, Point B, Point C) {
	return abs(A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y));
}

bool bounded(Part p, Position ship, float x, float y, float w = 1, float h = 1) {
	Rect part = createRect(p, ship);
	short area = (parts[p.i].w * parts[p.i].h);
	Point pt = Point(x, y);
	int a1 = 100 * triArea(part.tl, pt, part.bl);
	int a2 = 100 * triArea(part.bl, pt, part.br);
	int a3 = 100 * triArea(part.br, pt, part.tr);
	int a4 = 100 * triArea(part.tl, pt, part.tr);
	if (a1 + a2 + a3 + a4 < area) {
		std::cout << "what" << std::endl;
	}
	return a1 + a2 + a3 + a4 <= 100 * area;
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