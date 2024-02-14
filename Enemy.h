#pragma once
using namespace sf;

#include "Bullet.h"
#include "HealthBar.h"
#include "Utilities.h"

class Enemy {

public:
	static Clock levelUpTimer;
	Clock clock;
	CircleShape enemy;

	HealthBar healthBar;

	Vector2f pos;
	Vector2f dist;
	Vector2f dir;

	int speed = 200;
	int size = 30;
	int sides = size / 10;

	double mag;

	float separationRadius = size * 2 + size / 2;

	int health = 100;
	int damage = 10;
	int level = 1;
	int xpGive = size;

	int bulletSpeed = 500;

	int maxHealth;
	int currentHealth;

	float shootingSpeed = 0.2;
	float shootTime;

	Enemy() {};

	Enemy(int startLevel) : level(startLevel) {
		enemy.setRadius(size);
		enemy.setPointCount(sides);
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

	void Update(RenderWindow& window, Vector2f playerPos, const vector<Enemy>& enemies, float dt) {
		moveToPlayer(playerPos, enemies, dt);
		healthBar.Update(
			getPos(),
			getCurrentHealth(),
			getMaxHealth(),
			Vector2f(size * 2, sides),
			Vector2f(-size, size + 10)
		);

		shootTime = clock.getElapsedTime().asSeconds();
	}

	void render(RenderWindow& window) {
		window.draw(enemy);
		healthBar.render(window);
	}

	void moveToPlayer(Vector2f playerPos, const vector<Enemy>& enemies, float dt) {
		dist = playerPos - enemy.getPosition();

		mag = sqrt(pow(dist.x, 2) + pow(dist.y, 2));

		dir.x = dist.x / mag;
		dir.y = dist.y / mag;

		for (const auto& otherEnemy : enemies) {
			if (&otherEnemy != this) { // Exclude self
				Vector2f separationForce(0, 0);
				Vector2f distToOther = enemy.getPosition() - otherEnemy.enemy.getPosition();
				float distToOtherMag = magnitude(distToOther);

				if (distToOtherMag < separationRadius) {
					separationForce = normalize(distToOther) * (separationRadius - distToOtherMag);
					dir.x += separationForce.x;
					dir.y += separationForce.y;
				}
			}
		}


		setPos(
			getPos() +
			Vector2f(
				dir.x * speed * dt,
				dir.y * speed * dt
			)
		);

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

	void updateLevel() {
		if (levelUpTimer.getElapsedTime().asSeconds() >= 10) { // Level up every 60 seconds
			levelUp();
			levelUpTimer.restart();
		}
	}

	void levelUp() {
		level = getLevel();
		level++;
	}

	int getLevel() const {
		return level;
	}

	static Clock& getLevelUpTimer() {
		return levelUpTimer;
	}
};

