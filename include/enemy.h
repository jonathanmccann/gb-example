#ifndef PROJECT_ENEMY_H
#define PROJECT_ENEMY_H

typedef struct _Enemy {
	// Initial X coordinate starting point for enemy
	UBYTE xCoordinate;

	// Initial Y coordinate starting point for enemy
	UBYTE yCoordinate;

	// Set which style of movement the enemy will have
	UBYTE movementPathType;

	// Determine whether or not the enemy is on screen
	UBYTE isOnScreen;

	// Set the sprite number associated with the enemy
	int spriteNumber;
} Enemy;

void initializeEnemies();
void moveEnemySprites();
void testEnemyCollision();

#endif //PROJECT_ENEMY_H
