#include "sprite_and_background.h"

#ifndef PROJECT_ENEMY_H
#define PROJECT_ENEMY_H

#define numberOfEnemies 2

typedef struct _Enemy {
	// Initial X coordinate starting point for enemy
	UBYTE xCoordinate;

	// Initial Y coordinate starting point for enemy
	UBYTE yCoordinate;

	// Boolean to determine whether the enemy is moving up or down
	BOOLEAN isMovingUp;

	// Set the Y coordinate upper and lower bounds for the enemy sprite
	UBYTE yCoordinateLowerBoundary;
	UBYTE yCoordinateUpperBoundary;

	// Set the sprite number associated with the enemy
	int spriteNumber;
} Enemy;

Enemy enemies[numberOfEnemies];

void initializeEnemies();
void moveEnemySprites();

#endif //PROJECT_ENEMY_H
