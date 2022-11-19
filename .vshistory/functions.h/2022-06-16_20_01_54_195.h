short partW(Part& p) {
	if (p.rotation % 2 == 0) {
		return parts[p.i].w;
	}
	return parts[p.i].h;
}

short partH(Part& p) {
	if (p.rotation % 2 == 0) {
		return parts[p.i].h;
	}
	return parts[p.i].w;
}

float partX(Position pos, Part p) {
	float co = cos(pos.a);
	float si = sin(pos.a);
	float offset = 0;
	float w = ((16 - partW(p)) / 2.0);
	if (p.rotation == 1) {
		offset = co * w - si * w;
	}
	else if (p.rotation == 3) {
		offset = cos(pos.a - rad(90)) * (-(16 - partW(p)) / 2.0);
	}
	return (co * (p.x) - si * p.y + pos.x) + offset;
}

float partY(Position pos, Part p) {
	float co = cos(pos.a);
	float si = sin(pos.a);
	float offset = 0;
	if (p.rotation == 0) {
		offset = sin(pos.a - rad(90)) * (-(16 - partH(p)) / 2.0);
	}
	else if (p.rotation == 2) {
		offset = sin(pos.a - rad(90)) * ((16 - partH(p)) / 2.0);
	}
	return (si * p.x + co * p.y + pos.y) + offset;
}

float dist(Position a, Position b) {
	return dist(a.x, a.y, b.x, b.y);
}

bool overlap(float cX, float cY, float w, float h, float a, float x, float y) {
	if (a == 0) {
		return abs(cX - x) < w / 2 && abs(cY - y) < h / 2;
	}

	float co = cos(a);
	float si = sin(a);

	double x1 = co * x - si * y;
	double y1 = co * y + si * x;

	double x2 = co * cX - si * cY;
	double y2 = co * cY + si * cX;

	return abs(x2 - x1) < w / 2 && abs(y2 - y1) < h / 2;
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