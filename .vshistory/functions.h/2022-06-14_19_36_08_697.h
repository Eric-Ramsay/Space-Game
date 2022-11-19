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

float dist(Position a, Position b) {
	return dist(a.x, a.y, b.x, b.y);
}

bool overlap(Part a, Part b) {
	return overlap(a.x, a.y, 8, 8, b.x, b.y, 8, 8);
}
