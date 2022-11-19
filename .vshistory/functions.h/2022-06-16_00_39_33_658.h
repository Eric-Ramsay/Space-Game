float dot2(vec2 a, vec2 b) {
	return a.x * b.x + a.y * b.y;
}

float partX(Position pos, Part p) {
	float co = cos(pos.a);
	float si = sin(pos.a);
	return co * p.x - si * p.y + pos.x;
}

float partY(Position pos, Part p) {
	float co = cos(pos.a);
	float si = sin(pos.a);
	return si * p.x + co * p.y + pos.y;
}

float partW(Part p) {
	return p.w * -cos((PI / 2 * (1 + p.rotation)));
}

float partH(Part p) {
	return p.h * sin((PI / 2 * (1 + p.rotation)));
}

float dist(Position a, Position b) {
	return dist(a.x, a.y, b.x, b.y);
}

bool overlap(Position ap, Position bp, Part a, Part b) {
	return overlap(partX(ap, a), partY(ap, a), partW, 8, partX(bp, b), partY(bp, b), 8, 8);
}

bool overlap(Position ap, Part a, float x, float y, float w, float h) {
	return overlap(partX(ap, a), partY(ap, a), 8, 8, mX, mY, w, h);
}

unsigned long long int d2(Position a, Position b) {
	return d2(a.x, a.y, b.x, b.y);
}


bool planetClose(Position a, Planet p, int range) {
	return abs((int)p.x - (int)a.x) < range && abs((int)p.y - (int)a.y) < range;
}