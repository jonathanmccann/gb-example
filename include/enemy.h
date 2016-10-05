#ifndef PROJECT_ENEMY_H
#define PROJECT_ENEMY_H

typedef struct _Enemy {
	// Initial X coordinate starting point for enemy
	UBYTE xCoordinate;

	// Initial Y coordinate starting point for enemy
	UBYTE yCoordinate;

	// Set which style of movement the enemy will have
	UBYTE movementPathType;

	// Set the sprite number associated with the enemy
	int spriteNumber;
} Enemy;

void initializeEnemies();
void moveEnemySprites();
void testEnemyCollision();

#endif //PROJECT_ENEMY_H
