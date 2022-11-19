float dot2(vec2 a, vec2 b) {
	return a.x * b.x + a.y * b.y;
}

float partW(Part p) {
	if (p.rotation % 2 == 0) {
		return p.w;
	}
	return p.h;
}

float partH(Part p) {
	if (p.rotation % 2 == 0) {
		return p.h;
	}
	return p.w;
}

float partX(Position pos, Part p) {
	float co = cos(pos.a);
	float si = sin(pos.a);
	float x = co * p.x - si * p.y + pos.x;
	float offset = 0;
	if (p.rotation == 1) {
		offset = (16 - partW(p)) / 2.0;
	}
	else if (p.rotation == 3) {
		offset = -(16 - partW(p)) / 2.0;
	}
	return x + offset;
}

float partY(Position pos, Part p) {
	float co = cos(pos.a);
	float si = sin(pos.a);
	float y = si * p.x + co * p.y + pos.y;
	float offset = 0;
	if (p.rotation == 0) {
		offset = -(16 - partH(p)) / 2.0;
	}
	else if (p.rotation == 2) {
		offset = (16 - partH(p)) / 2.0;
	}
	return y + offset;
}

float dist(Position a, Position b) {
	return dist(a.x, a.y, b.x, b.y);
}

bool overlap(Position ap, Position bp, Part a, Part b) {
	return overlap(partX(ap, a), partY(ap, a), partW(a)/2.0, partH(a)/2.0, partX(bp, b), partY(bp, b), partW(b)/2.0, partH(b)/2.0);
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