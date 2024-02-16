#pragma once
using namespace sf;

int rng(int min, int max) {
	return rand() % (max - min + 1) + min;
}

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

float distance(Vector2f self, Vector2f away) {
	Vector2f dist;
	dist = away - self;
	return magnitude(dist);
}

Vector2f lerp(const Vector2f& self, const Vector2f& away, float t) {
	return Vector2f((1.0f - t) * self.x + t * away.x, (1.0f - t) * self.y + t * away.y);
}


