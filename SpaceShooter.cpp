#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Player.h"
#include "PlayerManager.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "BulletManager.h"
#include "Experience.h"

#include "Utilities.h"

#include "Star.h"

//#include "HUD.h"

using namespace std;
using namespace sf;

int main() {
	int width = 1920;
	int height = 1080;
	int ratio = width / height;

	int offset = 50;
	double dt = 0;

	RenderWindow window(VideoMode(width, height), "SpaceShooter");
	View view(Vector2f(width / 2, height / 2), Vector2f(width, height));

	Mouse mouse;
	Font font;
	font.loadFromFile("Oxygen.ttf");

	Player player;

	PlayerManager playerManager;
	EnemyManager enemyManager;

	BulletManager playerBulletManager;
	BulletManager enemyBulletManager;

	vector<Enemy> enemies;
	vector<Experience> pendingXP;

#pragma region Parallax

	Shader starsShader;

	if (!starsShader.loadFromFile("starFragment.shader", sf::Shader::Fragment)) {
		throw runtime_error("Error loading fragment shader");
	}

	int minStarSize = 1;
	int maxStarSize = 3;

	int starCount = 100;

	bool colorSwitch = 0;

	vector<Star> stars;

	for (int i = 0; i < starCount; i++)
	{
		Star star(
			rng(
				minStarSize,
				maxStarSize
			),
			rng(0, 1),
			Vector2f(rng(-width / 2, width / 2), rng(-height / 2, height / 2))
		);

		stars.push_back(star);
	}


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
	Clock spawnClock;
	float spawnTime = 0;
	float spawnCooldown = 2;

#pragma endregion

	while (window.isOpen()) {

		window.setView(view);

		view.setCenter(player.getPos());
		Vector2f viewport = window.mapPixelToCoords(
			Vector2i(
				view.getViewport().getPosition()),
			view
		);

		int randX = rng(
			viewport.x - offset,
			viewport.x + width + offset
		);

		int randY = rng(
			viewport.y - offset,
			viewport.y + height + offset
		);

		dt = clock.restart().asSeconds();

		float time = clock.getElapsedTime().asSeconds();

		float currentTime = fpsClock.restart().asSeconds();
		float fps = (1 / currentTime) * 100;

		spawnTime = spawnClock.getElapsedTime().asSeconds();

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) window.close();
		}

		if (player.currentXP >= player.xpToNextLvl) {
			player.levelUP();
			pickupSound.play();
		}

		for (int i = 0; i < pendingXP.size(); i++)
		{
			if (distance(player.getPos(), pendingXP[i].position) < player.pickUpRadius) {
				player.gainXP(pendingXP[i].amount);
				pendingXP.erase(pendingXP.begin() + i);
			}
		}

#pragma region Spawner

		if (spawnTime >= spawnCooldown) {
			if (
				randX < viewport.x ||
				randY < viewport.y ||
				randX > viewport.x + width ||
				randY > viewport.y + height
				) {
				Enemy enemy;
				enemy.setPos(Vector2f(randX, randY));
				enemy.setMaxHealth(player.level);
				enemy.setCurrentHealth();
				enemies.push_back(enemy);
				spawnClock.restart();
			}
		}

#pragma endregion

#pragma region Shoot

		if (Mouse::isButtonPressed(Mouse::Left) /* && player.isAlive */) {
			if (player.shootTime >= (1 / player.shootingSpeed)) {
				playerBulletManager.bullets.push_back(player.shoot());
				shootSound.play();
				player.clock.restart();
			}
		}

		if (enemies.size() > 0) {
			for (int i = 0; i < enemies.size(); i++) {
				if (enemies[i].shootTime >= (1 / enemies[i].shootingSpeed)) {
					int randEnemy = rng(0, i);

					enemyBulletManager.bullets.push_back(enemies[randEnemy].shoot());
					shootSound.play();

					enemies[i].clock.restart();
				}

			}
		}

#pragma endregion

#pragma region Player Collisions

		if (enemyBulletManager.bullets.size() > 0) {
			for (int i = 0; i < enemyBulletManager.bullets.size(); i++) {
				if (enemyBulletManager.bullets[i].bullet.getGlobalBounds().intersects(
					player.player.getGlobalBounds())) {
					hitSound.play();

					if (player.currentHealth > 0) {
						player.takeDmg(enemyManager.doDmg());
					}
					else {
						// player dead
					}
					enemyBulletManager.bullets.erase(
						enemyBulletManager.bullets.begin() +
						i
					);
				}
			}
		}
#pragma endregion

#pragma region Enemy Collisions

		if (enemies.size() > 0 && playerBulletManager.bullets.size() > 0) {
			for (int i = 0; i < enemies.size(); i++) {
				for (int j = 0; j < playerBulletManager.bullets.size(); j++) {
					if (playerBulletManager.bullets[j]
						.bullet.getGlobalBounds()
						.intersects(enemies[i].enemy.getGlobalBounds())) {
						hitSound.play();

						if (enemies[i].currentHealth > 0) {
							enemies[i].takeDmg(player.doDmg());
							playerBulletManager.bullets.erase(
								playerBulletManager.bullets.begin() + j);
						}

						if (enemies[i].currentHealth <= 0) {
							pendingXP.push_back(
								Experience(
									enemies[i].xpGive,
									enemies[i].getPos()
								));
							enemies.erase(enemies.begin() + i);

							dieSound.play();
						}

						break;
					}
				}
			}
		}

#pragma endregion

#pragma region Rendering

		window.clear();
#pragma region Stars moving & drawing

		for (int i = 0; i < stars.size(); i++) {

			stars[i].applyShader(starsShader);
			stars[i].update(normalize(player.vel));

			if (stars[i].getPos().x < viewport.x) {
				stars[i].pos.x = viewport.x + width;
			}
			if (stars[i].getPos().y < viewport.y) {
				stars[i].pos.y = viewport.y + height;
			}
			if (stars[i].getPos().x > viewport.x + width) {
				stars[i].pos.x = viewport.x;
			}
			if (stars[i].getPos().y > viewport.y + height) {
				stars[i].pos.y = viewport.y;
			}

			stars[i].render(window);
		}

#pragma endregion


		for (int i = 0; i < pendingXP.size(); i++) {
			pendingXP[i].draw(window);
		}


#pragma region Bullets Drawing

		if (playerBulletManager.bullets.size() > 0) {
			for (int i = 0; i < playerBulletManager.bullets.size(); i++) {
				playerBulletManager.bullets[i].draw(window);
				playerBulletManager.bullets[i].move(
					player.bulletSpeed *
					playerBulletManager.bullets[i].dir.x * dt,
					player.bulletSpeed *
					playerBulletManager.bullets[i].dir.y * dt
				);

				if (
					playerBulletManager.bullets[i].getPos().x <
					viewport.x ||
					playerBulletManager.bullets[i].getPos().y <
					viewport.y ||
					playerBulletManager.bullets[i].getPos().x >
					viewport.x + width ||
					playerBulletManager.bullets[i].getPos().y >
					viewport.y + height
					) {
					playerBulletManager.bullets.erase(
						playerBulletManager.bullets.begin() + i
					);
				}
			}
		}

		if (enemyBulletManager.bullets.size() > 0) {
			for (int i = 0; i < enemyBulletManager.bullets.size(); i++) {
				enemyBulletManager.bullets[i].draw(window);
				enemyBulletManager.bullets[i].move(
					enemyBulletManager.bullets[i].bulletSpeed *
					enemyBulletManager.bullets[i].dir.x * dt,
					enemyBulletManager.bullets[i].bulletSpeed *
					enemyBulletManager.bullets[i].dir.y * dt);

				if (
					enemyBulletManager.bullets[i].getPos().x <
					viewport.x ||
					enemyBulletManager.bullets[i].getPos().y <
					viewport.y ||
					enemyBulletManager.bullets[i].getPos().x >
					viewport.x + width ||
					enemyBulletManager.bullets[i].getPos().y >
					viewport.y + height
					) {
					enemyBulletManager.bullets.erase(
						enemyBulletManager.bullets.begin() + i
					);
				}
			}
		}

#pragma endregion


		if (player.isAlive) {
			player.Update(window, mouse, dt);
		}

		if (enemies.size() > 0) {
			for (int i = 0; i < enemies.size(); i++) {
				enemies[i].Update(window, player.getPos(), enemies, dt);
				enemies[i].render(window);
			}
		}

		window.display();
	}
#pragma endregion

	return 0;
}
