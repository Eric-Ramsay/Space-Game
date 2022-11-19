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
	return = w * cos(a) - h * sin(a);
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
	return = w * sin(a) - h * sin(a);
}



float partH(Part& p, float a) {

	var height = w * Math.sin(a) + h * Math.cos(a);

	return height;

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