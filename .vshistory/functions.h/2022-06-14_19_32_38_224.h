float dot2(vec2 a, vec2 b) {
	return a.x * b.x + a.y * b.y;
}

float dist(Position a, Position b) {
	return dist(a.x, a.y, b.x, b.y);
}

bool overlap(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
	if (fabs(a.x - b.x) < 8 + 8) {
		if (fabs(a.y - b.y) < 8 + 8) {
			return true;
		}
	}
	return false;
}

bool overlap(Part a, Part b) {
	if (fabs(a.x - b.x) < 8 + 8) {
		if (fabs(a.y - b.y) < 8 + 8) {
			return true;
		}
	}
	return false;
}