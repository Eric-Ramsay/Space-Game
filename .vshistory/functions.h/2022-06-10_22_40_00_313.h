#pragma once
template <typename Type> std::string to_str(const Type& t) {
	std::ostringstream os;
	os << t;
	return os.str();
}

float safeC(float a, float MAX) {
	int diff = a / MAX;
	if (a >= MAX) {
		return fmod(a, MAX);
	}
	else if (a < 0) {
		return safeC(a + MAX, MAX);
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

float dist(float x1, float y1, float x2, float y2) {
	return sqrtf(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

bool overlap(Position& a, float aR, Position& b, float bR) {
	if (fabs(a.x - b.x) < aR + bR) {
		if (fabs(a.y - b.y) < aR + bR) {
			return true;
		}
	}
	return false;
}

