#pragma once
using namespace std;
using namespace sf;

#include "Bullet.h"
#include "HealthBar.h"

class Player {
public:

	CircleShape player;

	HealthBar healthBar;

	Vector2f pos;
	Vector2f dist;
	Vector2f dir;

	Font font;
	Text levelNumber;

	int speed = 350;

	double mag;
	
	int radius = 30;
	int thickness = 5;

	int bulletSpeed = 1000;

	int health = 100;
	int damage = 10;
	int currentXP;
	int totalXP;
	int xpToNextLvl = pow(level, 2) * 5;

	int level = 1;

	int maxHealth;
	int currentHealth;

	Player() {
		
		pos = this->player.getPosition();
		player.setRadius(radius);
		player.setPointCount(3);

		player.setOrigin(radius, radius);
		player.setPosition(640, 360);			

		player.setFillColor(Color::Black);
		player.setOutlineColor(Color::Cyan);
		player.setOutlineThickness(thickness);

		font.loadFromFile("Oxygen.ttf");

		levelNumber.setOrigin(
			levelNumber.getGlobalBounds().getSize().x / 2,
			levelNumber.getGlobalBounds().getSize().y
		);
		levelNumber.setFont(font);		
		levelNumber.setFillColor(Color::White);		
		levelNumber.setStyle(Text::Bold);
		levelNumber.setCharacterSize(12);

		setMaxHealth(level);
		setCurrentHealth();
	}

	void draw(RenderWindow& window) {
		window.draw(player);
		window.draw(levelNumber);
		healthBar.render(window);
	}

	void lookToMouse(Mouse& mouse, RenderWindow& window) {
		double mouseX = mouse.getPosition(window).x;
		double mouseY = mouse.getPosition(window).y;
		
		dist.x = mouseX - player.getPosition().x;
		dist.y = mouseY - player.getPosition().y;

		dir.x = dist.x / mag;
		dir.y = dist.y / mag;

		double angle = ( atan2( dist.y, dist.x ) * ( 180 / 3.14 ) ) + 90;

		player.setRotation(angle);
	}
	
	void move(RenderWindow& window, float dt) {
		pos = player.getPosition();

		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			pos.y -= speed * dt;
		}
		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			pos.y += speed * dt;
		}
		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			pos.x -= speed * dt;
		}
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			pos.x += speed * dt;
		}

		if (pos.x < 0){
			pos.x = window.getSize().x;
		}
		if (pos.y < 0){
			pos.y = window.getSize().y;
		}
		if (pos.x > window.getSize().x){
			pos.x = 0;
		}
		if (pos.y > window.getSize().y){
			pos.y = 0;
		}

		healthBar.setPosition(
			player.getPosition().x - radius,
			player.getPosition().y + (radius * 2) + 20
		);

		healthBar.setSize(radius * 2, 7, getCurrentHealth(), getMaxHealth());
		
		player.setPosition(pos);

		levelNumber.setPosition(
			player.getPosition().x - radius,
			player.getPosition().y + radius + 20
		);
		levelNumber.setString("lvl " + to_string(level));
	}
	
	Bullet shoot() {
		Bullet bullet;
		bullet.setPos(player.getPosition());

		mag = sqrt(pow(dist.x, 2) + pow(dist.y, 2));
		bullet.dir.x = dist.x / mag;
		bullet.dir.y = dist.y / mag;
		
		return bullet;
	}

	Vector2f getPos() {
		return player.getPosition();
	}

	void setPos(float x, float y) {
		player.setPosition(Vector2f(x, y));
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
		return level += 1;
	}
};

