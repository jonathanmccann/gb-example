#include <gb.h>

#define numberOfEnemies 2

// Create an iterator for loops
int i;

typedef struct _Enemy {
	// Initial X coordinate starting point for enemy
	int xCoordinate;

	// Initial Y coordinate starting point for enemy
	int yCoordinate;

	// Boolean to determine whether the enemy is moving up or down
	BOOLEAN isMovingUp;

	// Set the Y coordinate upper and lower bounds for the enemy sprite
	int yCoordinateLowerBoundary;
	int yCoordinateUpperBoundary;

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

void insertEnemyIntoEnemyArray(Enemy *enemy, Enemy *enemyToInsert) {
	enemy->xCoordinate = enemyToInsert->xCoordinate;
	enemy->yCoordinate = enemyToInsert->yCoordinate;
	enemy->isMovingUp = enemyToInsert->isMovingUp;
	enemy->yCoordinateLowerBoundary = enemyToInsert->yCoordinateLowerBoundary;
	enemy->yCoordinateUpperBoundary = enemyToInsert->yCoordinateUpperBoundary;
	enemy->spriteNumber = enemyToInsert->spriteNumber;
}

void initializeEnemies() {
	// Pass references to the enemy array and the enemy itself
	// to work around a limitation in SDCC where structs cannot
	// be assigned directly to an array
	insertEnemyIntoEnemyArray(&enemies[0], &firstEnemy);
	insertEnemyIntoEnemyArray(&enemies[1], &secondEnemy);

	// Move the enemy sprites on to the screen so we can see it
	move_sprite(enemies[0].spriteNumber, enemies[0].xCoordinate, enemies[0].yCoordinate);
	move_sprite(enemies[1].spriteNumber, enemies[1].xCoordinate, enemies[1].yCoordinate);
}

void moveEnemySprites() {
	for (i = 0; i < numberOfEnemies; i++) {
		// Move the enemy sprite head in a straight line pattern
		if (enemies[i].isMovingUp) {
			enemies[i].yCoordinate--;

			if (enemies[i].yCoordinate <= enemies[i].yCoordinateLowerBoundary) {
				enemies[i].isMovingUp = 0;
			}
		}
		else {
			enemies[i].yCoordinate++;

			if (enemies[i].yCoordinate >= enemies[i].yCoordinateUpperBoundary) {
				enemies[i].isMovingUp = 1;
			}
		}

		move_sprite(enemies[i].spriteNumber, enemies[i].xCoordinate, enemies[i].yCoordinate);
	}
}