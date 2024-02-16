#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Experience {
public:
	CircleShape exp;

	Vector2f pos;
	Vector2f dir;
	Vector2f dist;
	int amount;
	float lerpSpeed = 4;

	Experience(int amount, Vector2f pos) {
		this->pos = pos;
		this->amount = amount;

		exp.setRadius(7);
		exp.setPosition(this->pos);
		exp.setFillColor(Color::Blue);
	}

	void update(float dt, Vector2f playerPos, float playerPickupRadius) {
		dist = playerPos - pos;
		dir = normalize(dist);

		float mag = magnitude(dist);

		if (mag < playerPickupRadius)
		{
			pos = lerp(pos, playerPos, lerpSpeed * dt);
		}

		setPosition(pos);
	}

	void draw(RenderWindow& window) {
		window.draw(exp);
	}

	Vector2f getPosition() const {
		return exp.getPosition();
	}

	void setPosition(Vector2f pos) {
		exp.setPosition(pos);
	}

	FloatRect getGlobalBounds() {
		return exp.getGlobalBounds();
	}

	int getAmount() const {
		return amount;
	}
};



