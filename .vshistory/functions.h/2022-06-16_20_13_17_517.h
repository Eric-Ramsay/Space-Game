short partW(Part& p, float a) {
	float co = cos(a);
	float si = sin(a);
	if (p.rotation % 2 == 0) {
		return co * parts[p.i].w + parts[p.i].h * si;
	}
	return co * parts[p.i].h + parts[p.i].h * si;
}

short partH(Part& p, float a) {
	float co = cos(a);
	float si = sin(a);
	if (p.rotation % 2 == 0) {
		return co * parts[p.i].w + si * parts[p.i].h;
	}
	return co * parts[p.i].w + si * parts[p.i].w;
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

bool overlap(Part pa, Part pb, Position a, Position b) {

}

bool overlap(float x1, float y1, float a1, float w1, float h1, float x2, float y2, float a2, float w2, float h2) {
	return overlap()
}

bool overlap(Position ap, Part a, float x, float y, float w, float h) {
	return overlap(partX(ap, a), partY(ap, a), partW(a)/2.0, partH(a)/2.0, x, y, w, h);
}

unsigned long long int d2(Position a, Position b) {
	return d2(a.x, a.y, b.x, b.y);
}


bool planetClose(Position a, Planet p, int range) {
	return abs((int)p.x - (int)a.x) < range && abs((int)p.y - (int)a.y) < range;
}