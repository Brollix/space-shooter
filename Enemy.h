#pragma once
using namespace sf;

#include "Bullet.h"
#include "HealthBar.h"

class Enemy {
public:
	Clock clock;
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

	int shootingSpeed = 1;
	float shootTime;

	Enemy() {
		Setup();
	}

	void Setup() {
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

	void Update(RenderWindow& window, Vector2f playerPos, float dt) {
		moveToPlayer(playerPos, dt);
		healthBar.Update(getPos(), getCurrentHealth(), getMaxHealth(), Vector2f(-(size / 2) - 3, 60));

		shootTime = clock.getElapsedTime().asSeconds();
	}

	void render(RenderWindow& window) {
		window.draw(enemy);
		healthBar.render(window);
	}

	void moveToPlayer(Vector2f playerPos, float dt) {
		dist.x = playerPos.x - enemy.getPosition().x;
		dist.y = playerPos.y - enemy.getPosition().y;

		mag = sqrt(pow(dist.x, 2) + pow(dist.y, 2));

		dir.x = dist.x / mag;
		dir.y = dist.y / mag;

		if (mag > 150) {
			setPos(
				getPos() +
				Vector2f(
					dir.x * speed * dt,
					dir.y * speed * dt
				)
			);
		}
	}

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