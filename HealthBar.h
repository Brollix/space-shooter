#pragma once

using namespace sf;
using namespace std;

class HealthBar {
public:

	RectangleShape bar;
	Text number;
	shared_ptr<sf::Font> font;;

	HealthBar() {
		font = std::make_shared<Font>();
		font->loadFromFile("Oxygen.ttf");
		number.setFont(*font);
		number.setCharacterSize(16);
		bar.setFillColor(Color(200, 50, 50));
	}

	void Update(Vector2f pos, int currentHealth, int maximumHealth, Vector2f dim, Vector2f offset = { 0, 0 }) {
		setSize(dim, currentHealth, maximumHealth);
		setString(currentHealth, maximumHealth);
		setPosition(pos + offset);
	}

	void setPosition(Vector2f pos) {
		bar.setPosition(pos);
		number.setPosition(pos.x, pos.y + 25);
	}

	void setString(int currentHealth, int maximumHealth) {
		number.setString(to_string(currentHealth) + "/" + to_string(maximumHealth));
	}

	void setSize(Vector2f dim, int currentHealth, int maximumHealth) {
		float percentage = (float)currentHealth / (float)maximumHealth;
		bar.setSize(Vector2f(dim.x * percentage, dim.y));
	}

	void setOrigin(float x, float y) {
		bar.setOrigin(Vector2f(x, y));
	}

	void render(RenderWindow& window) {
		window.draw(number);
		window.draw(bar);
	}
};