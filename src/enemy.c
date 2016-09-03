#include <gb/gb.h>

#include "../include/enemy.h"
#include "../include/player.h"

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

void initializeEnemies() {
	enemies[0].xCoordinate = firstEnemy.xCoordinate;
	enemies[0].yCoordinate = firstEnemy.yCoordinate;
	enemies[0].isMovingUp = firstEnemy.isMovingUp;
	enemies[0].yCoordinateLowerBoundary = firstEnemy.yCoordinateLowerBoundary;
	enemies[0].yCoordinateUpperBoundary = firstEnemy.yCoordinateUpperBoundary;
	enemies[0].spriteNumber = firstEnemy.spriteNumber;

	enemies[1].xCoordinate = secondEnemy.xCoordinate;
	enemies[1].yCoordinate = secondEnemy.yCoordinate;
	enemies[1].isMovingUp = secondEnemy.isMovingUp;
	enemies[1].yCoordinateLowerBoundary = secondEnemy.yCoordinateLowerBoundary;
	enemies[1].yCoordinateUpperBoundary = secondEnemy.yCoordinateUpperBoundary;
	enemies[1].spriteNumber = secondEnemy.spriteNumber;

	// Move the enemy sprites on to the screen so we can see it
	move_sprite(enemies[0].spriteNumber, enemies[0].xCoordinate, enemies[0].yCoordinate);
	move_sprite(enemies[1].spriteNumber, enemies[1].xCoordinate, enemies[1].yCoordinate);
}

void moveEnemySprites() {
	int i;

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

/*
 * The enemies are passed as elements to 'player.c'
 * because in 'player.c' the struct elements in the
 * array 'enemies' cannot be read outside of this
 * file.
 */
void testEnemyCollision() {
	int i;

	for (i = 0; i < numberOfEnemies; i++) {
		testShotAndEnemyCollision(&enemies[i]);
	}
}
