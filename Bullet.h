#pragma once
#include <iostream>
using namespace std;

class Bullet
{
public:

	int radius = 7;
	int bulletSpeed = 500;
	Vector2f playerSpeed;
	Vector2f dir;
	CircleShape bullet;

	Bullet() {
		bullet.setFillColor(Color::White);
		bullet.setRadius(radius);
		bullet.setOrigin(radius, radius);
	}

	void draw(RenderWindow& window) {		
		window.draw(bullet);
	}

	Vector2f getPos() {
		return bullet.getPosition();
	}

	void setPos(Vector2f pos) {		
		bullet.setPosition(pos);
	}

	void move(float offsetX, float offsetY) {
		bullet.setPosition(
			bullet.getPosition() + 
			Vector2f(offsetX, offsetY)
		);
	}
};

