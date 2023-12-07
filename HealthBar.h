#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

using namespace sf;
using namespace std;

class HealthBar
{
public:

	RectangleShape bar;
	Text number;
	Font font;
	Vector2f pos;

	HealthBar() {
		font.loadFromFile("Oxygen.ttf");
		number.setFont(font);
		bar.setFillColor(Color(200, 50, 50));
	}

	void setPosition(float x, float y) {
		bar.setPosition(Vector2f(x, y));
		number.setPosition(Vector2f(x, y + 25));
	}

	void setSize(float x, float y, float currentHealth, float maximumHealth) {
		float percentage = currentHealth / maximumHealth;
		bar.setSize(Vector2f(x * percentage, y));
		number.setString("health: " + to_string(currentHealth));
	}

	void setOrigin(float x, float y) {
		bar.setOrigin(Vector2f(x, y));
	}

	void render(RenderWindow& window) {		
		window.draw(bar);
	}
};