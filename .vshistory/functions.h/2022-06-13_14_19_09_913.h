float dot2(vec2 a, vec2 b) {
	return a.x * b.x + a.y * b.y;
}

float dist(Position a, Position b) {
	return dist(a.x, a.y, b.x, b.y);
}