#pragma once
using namespace sf;

#include "Bullet.h"

class Enemy {
public:
	CircleShape enemy;

	HealthBar healthBar;

	Vector2f pos;
	Vector2f dist;
	Vector2f dir;

	int speed = 200;
	int size = 50;

	double mag;

	int health = 100;
	int damage = 10;
	int level = 1;
	int xpGive = size;

	int bulletSpeed = 500;

	int maxHealth;
	int currentHealth;

	Enemy() {
		enemy.setRadius(size);
		enemy.setPointCount(size / 10);
		enemy.setFillColor(Color::Black);

		enemy.setOutlineColor(Color::White);
		enemy.setOutlineThickness(5);

		enemy.setOrigin(
			enemy.getGlobalBounds().getSize().x / 2,
			enemy.getGlobalBounds().getSize().y / 2
		);

		setMaxHealth(level);
		setCurrentHealth();
	}

	void draw(RenderWindow& window) {
		window.draw(enemy);
		healthBar.render(window);
	}

	void moveToPlayer(Vector2f playerPos, float dt) {
		dist.x = playerPos.x - enemy.getPosition().x;
		dist.y = playerPos.y - enemy.getPosition().y;

		mag = sqrt(pow(dist.x, 2) + pow(dist.y, 2));

		dir.x = dist.x / mag;
		dir.y = dist.y / mag;

		setPos(
			getPos() +
			Vector2f(
				dir.x * speed * dt,
				dir.y * speed * dt
			)
		);

		healthBar.setPosition(
			enemy.getPosition().x - size,
			enemy.getPosition().y + size + 5
		);

		healthBar.setSize(size * 2, 7, getCurrentHealth(), getMaxHealth());
	}
	
	/*void lookAround(vector<Enemy> enemies) {

	}*/

	Bullet shoot() {
		Bullet bullet;
		bullet.setPos(enemy.getPosition());

		mag = sqrt(pow(dist.x, 2) + pow(dist.y, 2));
		bullet.dir.x = dist.x / mag;
		bullet.dir.y = dist.y / mag;

		return bullet;
	}

	void setPos(Vector2f pos) {
		enemy.setPosition(pos);
	}

	Vector2f getPos() {
		return enemy.getPosition();
	}

	int getMaxHealth() {
		return maxHealth;
	}

	void setMaxHealth(int level) {
		maxHealth = health * level;
	}

	int getCurrentHealth() {
		return currentHealth;
	}

	void setCurrentHealth() {
		currentHealth = getMaxHealth();
	}

	int doDmg() {
		return damage;
	}

	void takeDmg(int damage) {
		currentHealth -= damage;
	}	
};

