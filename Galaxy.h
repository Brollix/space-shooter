#pragma once

#include <random>
#include <cmath>

using namespace sf;
using namespace std;

class Galaxy {
private:
	int windowWidth;
	int windowHeight;
	int numStars;
	std::vector<sf::Vector2f> stars;

public:
	Galaxy(int width, int height, int numStars)
		: windowWidth(width), windowHeight(height), numStars(numStars) {}

	void generate() {
		// Generate stars in a logarithmic spiral pattern
		const float a = 7; // Controls how tightly wound the spiral is
		const float b = 0.1; // Controls how spread out the spiral arms are
		const float numRotations = 16.108f; // Number of spiral rotations

		for (int i = 0; i < numStars; ++i) {
			float theta = i * (2 * 3.1415 * numRotations / numStars);
			float r = a * exp(b * theta);

			float x = windowWidth / 2 + r * cos(theta);
			float y = windowHeight / 2 + r * sin(theta);

			stars.push_back(sf::Vector2f(x, y));
		}
	}

	void draw(sf::RenderWindow& window) const {
		// Draw stars
		for (const auto& star : stars) {
			sf::CircleShape shape(1); // Star size
			shape.setPosition(star);
			shape.setFillColor(Color::White); // Star color
			window.draw(shape);
		}
	}

	const std::vector<sf::Vector2f>& getStars() const {
		return stars;
	}

};

