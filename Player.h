#pragma once
using namespace std;
using namespace sf;

#include "Bullet.h"
#include "HealthBar.h"

class Player {
public:
	Clock clock;
	CircleShape player;

	Vector2f pos;
	Vector2f vel;
	Vector2f dist;
	Vector2f dir;

	Font font;
	Text levelNumber;

	float maxSpeed = 350;
	float acc = 0.7;
	float dacc = 100;

	int radius = 30;
	int thickness = 5;

	float bulletSpeed = 1500;
	float shootingSpeed = 5;
	float shootTime;

	int health = 100;
	int damage = 20;
	int currentXP;
	int totalXP;
	int xpToNextLvl;

	bool isAlive = true;
	bool isMoving;

	int level = 1;

	int maxHealth;
	int currentHealth;

	Player() {
		Setup();
	}
	void Setup() {
		player.setRadius(radius);
		player.setPointCount(3);

		player.setOrigin(radius, radius);

		player.setFillColor(Color::Black);
		player.setOutlineColor(Color::Cyan);
		player.setOutlineThickness(thickness);

		font.loadFromFile("Oxygen.ttf");
		levelNumber.setFont(font);

		setMaxHealth(level);
		setCurrentHealth();

		setLevelText();
	}

	void Update(RenderWindow& window, Mouse mouse, float dt) {
		pos = getPos();
		draw(window);
		lookToMouse(mouse, window);
		move(window, dt);

		shootTime = clock.getElapsedTime().asSeconds();

		xpToNextLvl = pow(level, 2) * 5;

		levelNumber.setPosition(player.getPosition().x - radius,
			player.getPosition().y + radius + 20);
		levelNumber.setString("lvl " + to_string(level));
	}

	void move(RenderWindow& window, float dt) {
		if (Keyboard::isKeyPressed(Keyboard::W)) {
			vel.y -= acc;
		}
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			vel.x -= acc;
		}
		if (Keyboard::isKeyPressed(Keyboard::S)) {
			vel.y += acc;
		}
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			vel.x += acc;
		}



		float speed = magnitude(vel);
		if (speed > maxSpeed) {
			vel = normalize(vel) * maxSpeed;
		}

		pos += vel * dt;

		setPos(pos);
	}

	void draw(RenderWindow& window) {
		window.draw(player);
		window.draw(levelNumber);
	}

	Vector2f getPos() {
		return player.getPosition();
	}

	void setPos(Vector2f newPos) {
		player.setPosition(newPos.x, newPos.y);
	}

	void setLevelText() {
		levelNumber.setOrigin(levelNumber.getGlobalBounds().getSize().x / 2,
			levelNumber.getGlobalBounds().getSize().y);
		levelNumber.setFont(font);
		levelNumber.setFillColor(Color::White);
		levelNumber.setStyle(Text::Bold);
		levelNumber.setCharacterSize(12);
	}

	void setShootSpeed() {
		shootingSpeed += 0.10;
	}

	void lookToMouse(Mouse mouse, RenderWindow& window) {
		Vector2i pixelPos = mouse.getPosition(window);
		Vector2f worldPos = window.mapPixelToCoords(pixelPos);
		Vector2f mousePos;
		float mag;

		mousePos = worldPos;

		dist = mousePos - player.getPosition();

		mag = sqrt(pow(dist.x, 2) + pow(dist.y, 2));

		dir.x = dist.x / mag;
		dir.y = dist.y / mag;

		double angle = (atan2(dist.y, dist.x) * (180 / 3.14)) + 90;

		player.setRotation(angle);
	}

	float magnitude(Vector2f vec) {
		return sqrt(pow(vec.x, 2) + pow(vec.y, 2));
	}

	Vector2f normalize(Vector2f vec) {
		float mag = magnitude(vec);
		if (mag) {
			vec /= (float)mag;
		}
		return vec;
	}

	Bullet shoot() {
		Bullet bullet;
		bullet.setPos(player.getPosition());

		double mag;
		mag = sqrt(pow(dist.x, 2) + pow(dist.y, 2));
		bullet.dir.x = dist.x / mag;
		bullet.dir.y = dist.y / mag;

		return bullet;
	}

	int doDmg() {
		return damage * level;
	}

	void takeDmg(int damage) {
		if (health - damage > 0) {
			currentHealth -= damage;
		}
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

	int getCurrentXP() {
		return currentXP;
	}

	void setCurrentXP(int XP) {
		currentXP += XP;
	}

	int levelUP() {
		currentXP -= xpToNextLvl;
		//setShootSpeed();
		return level += 1;
	}
};
