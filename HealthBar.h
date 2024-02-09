#pragma once

using namespace sf;
using namespace std;

class HealthBar {
public:

	RectangleShape bar;
	Text number;
	Font font;

	HealthBar() {
		font.loadFromFile("Oxygen.ttf");
		number.setFont(font);
		bar.setFillColor(Color(200, 50, 50));
	}

	void Update(Vector2f pos, float currentHealth, float maximumHealth, Vector2f dim, Vector2f offset = { 0, 0 }) {
		setSize(dim, currentHealth, maximumHealth);
		setString(currentHealth);
		setPosition(pos + offset);
	}

	void setPosition(Vector2f pos) {
		bar.setPosition(pos);
		//number.setPosition(pos.x, pos.y + 25);
	}

	void setString(float currentHealth) {
		number.setString(to_string(currentHealth) + "HP");
	}

	void setSize(Vector2f dim, float currentHealth, float maximumHealth) {
		float percentage = currentHealth / maximumHealth;
		bar.setSize(Vector2f(dim.x * percentage, dim.y));
	}

	void setOrigin(float x, float y) {
		bar.setOrigin(Vector2f(x, y));
	}

	void render(RenderWindow& window) {
		//window.draw(number);
		window.draw(bar);
	}
};