#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Player.h"
#include "Enemy.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "BulletManager.h"

using namespace std;
using namespace sf;

int width = 1920;
int height = 1080;
int ratio = width / height;

int rng(int min, int max) { return rand() % (max - min + 1) + min; }

int main()
{
	RenderWindow window(VideoMode(width, height), "SpaceShooter");
	window.setMouseCursorVisible(true);
	window.setVerticalSyncEnabled(false);

	Mouse mouse;

	Player player;
	player.setPos(width / 2, height / 2);

	PlayerManager playerManager;
	EnemyManager enemyManager;

	BulletManager playerBulletManager;
	BulletManager enemyBulletManager;	

	vector<Enemy> enemies;

	double dt;

	int offset = 50;

#pragma region HUD

	Font font;
	font.loadFromFile("Oxygen.ttf");

	Text scoreText;
	scoreText.setFont(font);
	scoreText.setPosition(25, 25);
	scoreText.setFillColor(Color::White);
	scoreText.setString("XP: ");
	scoreText.setStyle(Text::Bold);

	Text scoreNumber;
	scoreNumber.setFont(font);
	scoreNumber.setPosition(scoreText.getLocalBounds().getSize().x + 25, 25);
	scoreNumber.setFillColor(Color::White);
	scoreNumber.setStyle(Text::Bold);

	Text xpText;
	xpText.setFont(font);
	xpText.setPosition(width / 2 - 150, height - 50);
	xpText.setFillColor(Color::White);
	xpText.setString("EXPERIENCE: ");
	xpText.setStyle(Text::Bold);

	Text xpNumber;
	xpNumber.setFont(font);
	xpNumber.setPosition(xpText.getGlobalBounds().getSize().x + xpText.getPosition().x + 10, height - 50);
	xpNumber.setFillColor(Color::White);
	xpNumber.setStyle(Text::Bold);

#pragma endregion

#pragma region Sounds

	Sound shootSound;
	SoundBuffer shootSoundBuffer;
	shootSound.setBuffer(shootSoundBuffer);
	shootSoundBuffer.loadFromFile("sounds/laserShoot.wav");
	shootSound.setVolume(50);

	Sound hitSound;
	SoundBuffer hitSoundBuffer;
	hitSound.setBuffer(hitSoundBuffer);
	hitSoundBuffer.loadFromFile("sounds/hitHurt.wav");
	hitSound.setVolume(50);

	Sound dieSound;
	SoundBuffer dieSoundBuffer;
	dieSound.setBuffer(dieSoundBuffer);
	dieSoundBuffer.loadFromFile("sounds/explosion.wav");
	dieSound.setVolume(50);

	Sound pickupSound;
	SoundBuffer pickupSoundBuffer;
	pickupSound.setBuffer(hitSoundBuffer);
	pickupSoundBuffer.loadFromFile("sounds/pickupCoin.wav");
	pickupSound.setVolume(50);

#pragma endregion	

#pragma region Clocks

	Clock clock;
	Clock fpsClock;
	Clock shootingClock;
	Clock spawnClock;
	float shootTime = 0;
	float spawnTime = 0;
	float spawnCooldown = 2;

#pragma endregion

	while (window.isOpen()) {

		int randX = rng( -offset, width + offset );
		int randY = rng( -offset, height + offset );

		dt = clock.restart().asSeconds();

		float time = clock.getElapsedTime().asSeconds();

		float currentTime = fpsClock.restart().asSeconds();
		float fps = 1 / currentTime;

		shootTime = shootingClock.getElapsedTime().asSeconds();

		spawnTime = spawnClock.getElapsedTime().asSeconds();

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();

			if (event.type == event.MouseButtonPressed) {
				if (event.key.code == Mouse::Left) {
					playerBulletManager.bullets.push_back(player.shoot());
					shootSound.play();
				}
			}
		}

#pragma region Spawner

		if (spawnTime >= spawnCooldown) {
			if (enemies.size() < 4)
			{
				if ((randX < 0) ||
					(randY < 0) ||
					(randX > width) ||
					(randY > height))
				{
					Enemy enemy;
					enemy.setPos(Vector2f(randX, randY));
					enemy.setMaxHealth(player.level);
					enemy.setCurrentHealth();
					enemies.push_back(enemy);
					spawnClock.restart();
				}
			}
		}		

#pragma endregion		

		window.clear();
		player.xpToNextLvl = (player.level * player.level) * 5;

		xpNumber.setString(to_string(player.currentXP) + " / " + to_string(player.xpToNextLvl));

		window.draw(xpText);
		window.draw(xpNumber);

		player.draw(window);

		player.move(window, dt);
		player.lookToMouse(mouse, window);

		if (enemies.size() > 0) {
			for (int i = 0; i < enemies.size(); i++)
			{
				enemies[i].moveToPlayer(player.getPos(), dt);
				enemies[i].draw(window);
			}
		}
		
#pragma region Bullets Drawing/Checking

		if (playerBulletManager.bullets.size() > 0){
			for (int i = 0; i < playerBulletManager.bullets.size(); i++)
			{
				playerBulletManager.bullets[i].draw(window);
				playerBulletManager.bullets[i].move(
					player.bulletSpeed * playerBulletManager.bullets[i].dir.x * dt,
					player.bulletSpeed * playerBulletManager.bullets[i].dir.y * dt
				);
				
				if (
					playerBulletManager.bullets[i].getPos().x < 0 - offset ||
					playerBulletManager.bullets[i].getPos().y < 0 - offset ||
					playerBulletManager.bullets[i].getPos().x > window.getSize().x + offset ||
					playerBulletManager.bullets[i].getPos().y > window.getSize().y + offset
					)
				{
					playerBulletManager.bullets.erase(playerBulletManager.bullets.begin() + i);
				}				
			}
		}
		
		if (enemyBulletManager.bullets.size() > 0) {
			for (int i = 0; i < enemyBulletManager.bullets.size(); i++)
			{
				enemyBulletManager.bullets[i].draw(window);
				enemyBulletManager.bullets[i].move(
					enemyBulletManager.bullets[i].bulletSpeed * enemyBulletManager.bullets[i].dir.x * dt,
					enemyBulletManager.bullets[i].bulletSpeed * enemyBulletManager.bullets[i].dir.y * dt
				);			
				
				if (enemyBulletManager.bullets[i].getPos().x < 0 - offset ||
					enemyBulletManager.bullets[i].getPos().y < 0 - offset ||
					enemyBulletManager.bullets[i].getPos().x > window.getSize().x + offset ||
					enemyBulletManager.bullets[i].getPos().y > window.getSize().y + offset)
				{
					enemyBulletManager.bullets.erase(enemyBulletManager.bullets.begin() + i);
				}				
			}
		}		
		
#pragma endregion 		

		if (player.currentXP >= player.xpToNextLvl)
		{
			player.levelUP();
			pickupSound.play();
		}

		if (enemies.size() > 0 && shootTime >= 2){	

			int randEnemy = rng(0, enemies.size() - 1);
			
			enemyBulletManager.bullets.push_back(enemies[i].shoot());
			shootSound.play();
			shootingClock.restart();
			
		}

#pragma region Player Collisions

		if (enemyBulletManager.bullets.size() > 0){
			for (int i = 0; i < enemyBulletManager.bullets.size(); i++){
				if (enemyBulletManager.bullets[i].bullet.getGlobalBounds()
					.intersects(player.player.getGlobalBounds()))
				{
					hitSound.play();

					if (player.currentHealth > 0)
					{
						player.takeDmg(enemyManager.doDmg());
					}
					else {
						// player dead
					}
					enemyBulletManager.bullets.erase(enemyBulletManager.bullets.begin() + i);
				}
			}
		}
#pragma endregion

#pragma region Enemy Collisions

		if (enemies.size() > 0 && playerBulletManager.bullets.size() > 0){	
			for (int i = 0; i < enemies.size(); i++){
				for (int j = 0; j < playerBulletManager.bullets.size(); j++)
				{
					if (playerBulletManager.bullets[j].bullet.getGlobalBounds()
						.intersects(enemies[i].enemy.getGlobalBounds()))
					{
						hitSound.play();

						if (enemies[i].currentHealth > 0) {
							enemies[i].takeDmg(player.doDmg());
							playerBulletManager.bullets.erase(playerBulletManager.bullets.begin() + j);
						}

						if (enemies[i].currentHealth <= 0) {
							player.setCurrentXP(enemies[i].xpGive);
							enemies.erase(enemies.begin() + i);

							dieSound.play();
						}

						break;
					}
				}
			}
		}

#pragma endregion


		window.display();
	}
	return 0;
}
