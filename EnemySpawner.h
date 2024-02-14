#pragma once
#include "Enemy.h"

class EnemySpawner
{
public:
	Enemy spawnEnemy(float randX, float randY) {
		// Calculate the level based on elapsed time
		int level = calculateEnemyLevel();
		return Enemy(level);
	}

private:
	// Function to calculate enemy level based on elapsed time
	int calculateEnemyLevel() {
		// For example, spawn enemies at level 1 initially, then increase every 60 seconds
		int secondsPassed = Enemy::getLevelUpTimer().getElapsedTime().asSeconds();
		return 1 + secondsPassed / 60; // Increase level every 60 seconds
	}
};