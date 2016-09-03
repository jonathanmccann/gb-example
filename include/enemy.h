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

Enemy firstEnemy = {
	100,  			// Starting X coordinate
	100,  			// Staring Y coordinate
	TRUE, 			// Is the enemy moving up
	50,   			// Y coordinate lower boundary
	125,  			// Y coordinate upper boundary
	first_enemy     // Sprite number of the enemy
};

Enemy secondEnemy = {
	125,   		  // Starting X coordinate
	20,    		  // Staring Y coordinate
	FALSE, 		  // Is the enemy moving up
	20,    		  // Y coordinate lower boundary
	125,   		  // Y coordinate upper boundary
	second_enemy  // Sprite number of the enemy
};

Enemy enemies[numberOfEnemies];

void initializeEnemies();
void moveEnemySprites();

#endif //PROJECT_ENEMY_H
