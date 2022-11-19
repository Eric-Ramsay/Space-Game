#pragma once
template <typename Type> std::string to_str(const Type& t) {
	std::ostringstream os;
	os << t;
	return os.str();
}

float safeC(float a, float MAX) {
	int diff = 1 + (-a / MAX);
	if (a >= MAX) {
		return fmod(a, MAX);
	}
	else if (a < 0) {
		return safeC(a + MAX*diff, MAX);
	}
	return a;
}

float rad(float deg) {
	return (deg * 3.14159265358979323846) / 180.0f;
}
float deg(float rad) {
	return safeC(rad * 180 / 3.14159265358979323846, 360);
}

float dot2(vec2 a, vec2 b) {
	return a.x * b.x + a.y * b.y;
}

float min(float a, float b) {
	if (a < b) {
		return a;
	}
	return b;
}

float dist(Position a, Position b) {
	return dist(a.x, a.y, b.x, b.y);
}

bool overlap(Part a, Part b) {
	if (fabs(a.x - b.x) < 8 + 8) {
		if (fabs(a.y - b.y) < 8 + 8) {
			return true;
		}
	}
	return false;
}

