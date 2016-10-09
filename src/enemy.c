#include <gb/gb.h>

#include <stdio.h>

#include "../include/enemy.h"
#include "../include/player.h"
#include "../include/shot.h"
#include "../include/sprite_and_background.h"

#define movementPathStraight 1						// Move in a straight line
#define movementPathUpwardDiagonal 2				// Move in a straight diagonal upward
#define movementPathDownwardDiagonal 3				// Move in a straight diagonal downward
#define movementPathStraightUpwardDiagonal 4		// Move in a straight line then move diagonal upward
#define movementPathStraightDownwardDiagonal 5		// Move in a straight line then move diagonal downward
#define movementPathZigZag 6						// Move in an alternating upward and downward diagonal
#define movementPathStop 7							// Move in a straight line and then stop moving
#define movementPathStraightAndReturn 8				// Move in a straight line then u-turn back

UBYTE numberOfEnemies;

Enemy firstEnemy = {
	175,  					// Starting X coordinate
	125,  					// Staring Y coordinate
	movementPathStraight,	// Movement path type
	0,						// Movement counter
	50,						// Change in movement
	1,						// Is on screen
	first_enemy   		    // Sprite number of the enemy
};

Enemy secondEnemy = {
	125,   		 			// Starting X coordinate
	20,    		 			// Staring Y coordinate
	movementPathStraight, 	// Movement path type
	0,						// Movement counter
	0,						// Change in movement
	1,						// Is on screen
	second_enemy  			// Sprite number of the enemy
};

Enemy thirdEnemy = {
	75,   		  					// Starting X coordinate
	50,    		 					// Staring Y coordinate
	movementPathUpwardDiagonal, 	// Movement path type
	0,								// Movement counter
	0,								// Change in movement
	1,								// Is on screen
	third_enemy  					// Sprite number of the enemy
};

Enemy fourthEnemy = {
	150,   		  					// Starting X coordinate
	10,    		 					// Staring Y coordinate
	movementPathDownwardDiagonal, 	// Movement path type
	0,								// Movement counter
	0,								// Change in movement
	1,								// Is on screen
	fourth_enemy  					// Sprite number of the enemy
};

Enemy enemies[10];

void initializeEnemies() {
	enemies[0].xCoordinate = firstEnemy.xCoordinate;
	enemies[0].yCoordinate = firstEnemy.yCoordinate;
	enemies[0].movementPathType = firstEnemy.movementPathType;
	enemies[0].movementCounter = firstEnemy.movementCounter;
	enemies[0].changeInMovement = firstEnemy.changeInMovement;
	enemies[0].isOnScreen = firstEnemy.isOnScreen;
	enemies[0].spriteNumber = firstEnemy.spriteNumber;

	enemies[1].xCoordinate = secondEnemy.xCoordinate;
	enemies[1].yCoordinate = secondEnemy.yCoordinate;
	enemies[1].movementPathType = secondEnemy.movementPathType;
	enemies[1].movementCounter = secondEnemy.movementCounter;
	enemies[1].changeInMovement = secondEnemy.changeInMovement;
	enemies[1].isOnScreen = secondEnemy.isOnScreen;
	enemies[1].spriteNumber = secondEnemy.spriteNumber;

	enemies[2].xCoordinate = thirdEnemy.xCoordinate;
	enemies[2].yCoordinate = thirdEnemy.yCoordinate;
	enemies[2].movementPathType = thirdEnemy.movementPathType;
	enemies[2].movementCounter = thirdEnemy.movementCounter;
	enemies[2].changeInMovement = thirdEnemy.changeInMovement;
	enemies[2].isOnScreen = thirdEnemy.isOnScreen;
	enemies[2].spriteNumber = thirdEnemy.spriteNumber;

	enemies[3].xCoordinate = fourthEnemy.xCoordinate;
	enemies[3].yCoordinate = fourthEnemy.yCoordinate;
	enemies[3].movementPathType = fourthEnemy.movementPathType;
	enemies[3].movementCounter = fourthEnemy.movementCounter;
	enemies[3].changeInMovement = fourthEnemy.changeInMovement;
	enemies[3].isOnScreen = fourthEnemy.isOnScreen;
	enemies[3].spriteNumber = fourthEnemy.spriteNumber;

	// Move the enemy sprites on to the screen so we can see it
	move_sprite(enemies[0].spriteNumber, enemies[0].xCoordinate, enemies[0].yCoordinate);
	move_sprite(enemies[1].spriteNumber, enemies[1].xCoordinate, enemies[1].yCoordinate);
	move_sprite(enemies[2].spriteNumber, enemies[2].xCoordinate, enemies[2].yCoordinate);
	move_sprite(enemies[3].spriteNumber, enemies[3].xCoordinate, enemies[3].yCoordinate);

	numberOfEnemies = 4;
}

void moveEnemySprites() {
	int i;

	for (i = 0; i < numberOfEnemies; i++) {
		if (enemies[i].isOnScreen) {
			if (enemies[i].movementPathType == movementPathStraight) {
				enemies[i].xCoordinate--;
			}
			else if (enemies[i].movementPathType == movementPathUpwardDiagonal) {
				enemies[i].xCoordinate--;
				enemies[i].yCoordinate--;
			}
			else if (enemies[i].movementPathType == movementPathDownwardDiagonal) {
				enemies[i].xCoordinate--;
				enemies[i].yCoordinate++;
			}
			else if (enemies[i].movementPathType == movementPathStraightUpwardDiagonal) {
				if (enemies[i].changeInMovement < enemies[i].movementCounter) {
					enemies[i].xCoordinate--;
					enemies[i].yCoordinate--;
				}
				else {
					enemies[i].xCoordinate--;

					enemies[i].movementCounter++;
				}
			}
			else if (enemies[i].movementPathType == movementPathStraightDownwardDiagonal) {
				if (enemies[i].changeInMovement < enemies[i].movementCounter) {
					enemies[i].xCoordinate--;
					enemies[i].yCoordinate++;
				}
				else {
					enemies[i].xCoordinate--;

					enemies[i].movementCounter++;
				}
			}
			else if (enemies[i].movementPathType == movementPathZigZag) {
				if (enemies[i].changeInMovement < enemies[i].movementCounter) {
					enemies[i].xCoordinate--;
					enemies[i].yCoordinate++;
				}
				else {
					enemies[i].xCoordinate--;
					enemies[i].yCoordinate--;

					enemies[i].movementCounter++;
				}
			}
			else if (enemies[i].movementPathType == movementPathStop) {
				if (enemies[i].changeInMovement > enemies[i].movementCounter) {
					enemies[i].xCoordinate--;

					enemies[i].movementCounter++;
				}
			}
			else if (enemies[i].movementPathType == movementPathStraightAndReturn) {
				if (enemies[i].changeInMovement < enemies[i].movementCounter) {
					enemies[i].xCoordinate++;
				}
				else {
					enemies[i].xCoordinate--;

					enemies[i].movementCounter++;
				}
			}

			move_sprite(enemies[i].spriteNumber, enemies[i].xCoordinate, enemies[i].yCoordinate);
		}
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
		testPlayerAndEnemyCollision(&enemies[i]);
	}
}
