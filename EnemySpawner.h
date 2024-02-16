#pragma once
#include "Enemy.h"

Clock Enemy::levelUpTimer;

class EnemySpawner
{
public:
	Enemy spawnEnemy() {
		int level = calculateEnemyLevel(Enemy::getLevelUpTimer());
		return Enemy(level);
	}

private:
	int period = 15;

	int calculateEnemyLevel(const sf::Clock& levelUpTimer) {
		int secondsPassed = levelUpTimer.getElapsedTime().asSeconds();
		return 1 + secondsPassed / period;
	}
};