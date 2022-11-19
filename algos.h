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

float min(float a, float b) {
	if (a < b) {
		return a;
	}
	return b;
}

float max(float a, float b) {
	if (a < b) {
		return b;
	}
	return a;
}

float dist(float x1, float y1, float x2, float y2) {
	return sqrtf(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

unsigned long long int d2(long x, long y, long x1, long y1) {
	return (x - x1) * (x - x1) + (y - y1) * (y - y1);
}

bool close(float x1, float x2) {
	if (x1 > x2) {
		return (x1 - x2) < .01;
	}
	return (x2 - x1) < .01;
}

bool overlap(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
	if (fabs(x1 - x2) < w1 + w2) {
		if (fabs(y1 - y2) < h1 + h2) {
			return true;
		}
	}
	return false;
}