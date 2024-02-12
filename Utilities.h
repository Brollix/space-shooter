#ifndef UTILITIES_H
#define UTILITIES_H

using namespace sf;

float magnitude(Vector2f vec) {
	return sqrt(pow(vec.x, 2) + pow(vec.y, 2));
}

Vector2f normalize(Vector2f vec) {
	float mag = magnitude(vec);
	if (mag) {
		vec /= (float)mag;
	}
	return vec;
}

int rng(int min, int max) {
	return rand() % (max - min + 1) + min;
}

#endif
