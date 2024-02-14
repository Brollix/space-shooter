#pragma once
#include "Enemy.h"

class EnemySpawner
{
public:
	Enemy spawnEnemy() {
		// Calculate the level based on elapsed time
		int level = calculateEnemyLevel(Enemy::getLevelUpTimer());
		return Enemy(level);
	}

private:
	// Function to calculate enemy level based on elapsed time
	int calculateEnemyLevel(const sf::Clock& levelUpTimer) {
		// For example, spawn enemies at level 1 initially, then increase every 60 seconds
		int secondsPassed = levelUpTimer.getElapsedTime().asSeconds();
		return 1 + secondsPassed / 5; // Increase level every 60 seconds
	}
};