#pragma once
class Experience
{
public:
	int amount;
	Vector2f position;
	CircleShape expCircle;

	Experience(int amount, Vector2f position) :
		amount(amount), position(position) {

		expCircle.setRadius(5);
		expCircle.setPosition(position);
		expCircle.setFillColor(Color::Blue);
	}

	void draw(RenderWindow& window) {
		window.draw(expCircle);
	}
};

