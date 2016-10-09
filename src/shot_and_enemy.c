#include <gb/gb.h>

#include "../include/global.h"
#include "../include/player.h"
#include "../include/sprite_and_background.h"

#define numberOfPlayerShots 9
#define numberOfShotsInGroup 3
#define numberOfEnemyShots 20

#define forwardShot 0
#define upAndDownShot 1
#define behindShot 2

#define shotXCoordinateLowerBoundary 0
#define shotXCoordinateUpperBoundary 168
#define shotYCoordinateLowerBoundary 0
#define shotYCoordinateUpperBoundary 160

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
	50,						// Shot counter
	1,						// Is on screen
	first_enemy   		    // Sprite number of the enemy
};

Enemy secondEnemy = {
	125,   		 			// Starting X coordinate
	20,    		 			// Staring Y coordinate
	movementPathStraight, 	// Movement path type
	0,						// Movement counter
	0,						// Change in movement
	50,						// Shot counter
	1,						// Is on screen
	second_enemy  			// Sprite number of the enemy
};

Enemy thirdEnemy = {
	75,   		  					// Starting X coordinate
	50,    		 					// Staring Y coordinate
	movementPathUpwardDiagonal, 	// Movement path type
	0,								// Movement counter
	0,								// Change in movement
	50,								// Shot counter
	1,								// Is on screen
	third_enemy  					// Sprite number of the enemy
};

Enemy fourthEnemy = {
	150,   		  					// Starting X coordinate
	10,    		 					// Staring Y coordinate
	movementPathDownwardDiagonal, 	// Movement path type
	0,								// Movement counter
	0,								// Change in movement
	50,								// Shot counter
	1,								// Is on screen
	fourth_enemy  					// Sprite number of the enemy
};

Enemy enemies[10];

UBYTE canShootLower, canShootUpper, i, rotatingShotDirection;

// Create a counter to keep track of which shot we should be firing
UBYTE shotCounter;

typedef struct {
	// Initial X coordinate starting point for the shot
	UBYTE xCoordinate;

	// Initial Y coordinate starting point for the shot
	UBYTE yCoordinate;

	// Velocity of the shot in the X direction
	INT8 xVelocity;

	// Velocity of the shot in the Y direction
	INT8 yVelocity;

	// Boolean to determine whether the enemy is moving up or down
	int isOnScreen;

	// Boolean to determine whether or not this shot can be fired. This is used
	// when one of the ship's guns has been destroyed
	int canBeShot;

	// Set the sprite number associated with the enemy
	int spriteNumber;
} Shot;

Shot playerShots[numberOfPlayerShots];
Shot enemyShots[numberOfEnemyShots];

void initializePlayerShots() {
	// Set up the shots to confirm that "isOnScreen" is false.

	// Incrementing within the loop to allow for differing velocities from
	// the inception of the shots.
	for (i = 0; i < numberOfPlayerShots; i++) {
		playerShots[i].xCoordinate = offScreen;
		playerShots[i].yCoordinate = offScreen;
		playerShots[i].xVelocity = 2;
		playerShots[i].yVelocity = 0;
		playerShots[i].spriteNumber = shot_sprite_starting_position + i;
		playerShots[i].isOnScreen = 0;
		playerShots[i].canBeShot = 1;

		move_sprite(
			playerShots[i].spriteNumber,
			playerShots[i].xCoordinate,
			playerShots[i].yCoordinate);

		i++;

		playerShots[i].xCoordinate = offScreen;
		playerShots[i].yCoordinate = offScreen;
		playerShots[i].xVelocity = 2;
		playerShots[i].yVelocity = -1;
		playerShots[i].spriteNumber = shot_sprite_starting_position + i;
		playerShots[i].isOnScreen = 0;
		playerShots[i].canBeShot = 1;

		move_sprite(
			playerShots[i].spriteNumber,
			playerShots[i].xCoordinate,
			playerShots[i].yCoordinate);

		i++;

		playerShots[i].xCoordinate = offScreen;
		playerShots[i].yCoordinate = offScreen;
		playerShots[i].xVelocity = 2;
		playerShots[i].yVelocity = 1;
		playerShots[i].spriteNumber = shot_sprite_starting_position + i;
		playerShots[i].isOnScreen = 0;
		playerShots[i].canBeShot = 1;

		move_sprite(
			playerShots[i].spriteNumber,
			playerShots[i].xCoordinate,
			playerShots[i].yCoordinate);
	}

	// Initialize the shotCounter to zero so we start from the beginning of the
	// 'shots' array.
	shotCounter = 0;

	// Set up the booleans to make sure the correct shot type is being disabled
	canShootLower = 1;
	canShootUpper = 1;

	rotatingShotDirection = forwardShot;
	i = 0;
}

void movePlayerShotsToPlayer(Player *player) {
	// Set all of the shots to be where the player is
	// so it looks like the shot is coming from the ship

	// Test to see if the current group of shots is already on screen
	if (playerShots[shotCounter].isOnScreen || playerShots[shotCounter + 1].isOnScreen ||
		playerShots[shotCounter + 2].isOnScreen) {

		return;
	}

	// Only loop as far as the number of shots in a group allows
	for (i = 0; i < numberOfShotsInGroup; i++) {
		if (playerShots[shotCounter].canBeShot) {
			playerShots[shotCounter].xCoordinate = player->xCoordinate;
			playerShots[shotCounter].yCoordinate = player->yCoordinate;
			playerShots[shotCounter].isOnScreen = 1;

			move_sprite(
				playerShots[shotCounter].spriteNumber,
				playerShots[shotCounter].xCoordinate,
				playerShots[shotCounter].yCoordinate);
		}

		shotCounter++;
	}

	// Here, we aren't doing "numberOfPlayerShots - 1" since the loop above will
	// increment it before breaking out.
	if (shotCounter == numberOfPlayerShots) {
		shotCounter = 0;
	}
}

// If the lower shot has already been removed, then remove the upper shot.
// Look over very specifically the third, sixth, and ninth shots since those
// are the lower ones.
void removePlayerLowerShot() {
	if (!canShootLower) {
		removePlayerUpperShot();
	}
	else {
		int i;

		for (i = 2; i <= 8; i += 3) {
			playerShots[i].canBeShot = 0;
		}

		canShootLower = 0;
	}
}

// If the upper shot has already been removed, then remove the lower shot.
// Look over very specifically the second, fifth, and eighth shots since those
// are the upper ones.
void removePlayerUpperShot() {
	if (!canShootUpper) {
		removePlayerLowerShot();
	}
	else {
		int i;

		for (i = 1; i <= 7; i += 3) {
			playerShots[i].canBeShot = 0;
		}

		canShootUpper = 0;
	}
}

void updateRotatingShotDirection() {
	if (rotatingShotDirection == forwardShot) {
		rotatingShotDirection = upAndDownShot;
	}
	else if (rotatingShotDirection == upAndDownShot) {
		rotatingShotDirection = behindShot;
	}
	else {
		rotatingShotDirection = forwardShot;
	}
}

void updateRotatingShotsVelocity() {
    int xVelocity, yVelocity;

	if (rotatingShotDirection == forwardShot) {
		xVelocity = 2;
		yVelocity = 1;
	}
	else if (rotatingShotDirection == upAndDownShot) {
		xVelocity = 0;
		yVelocity = 2;
	}
	else {
		xVelocity = -2;
		yVelocity = 1;
	}

	// Start at index 1 because the shots 0, 3, 6, etc. are straight shots
	// and their velocity does not need to be updated.
	for (i = 1; i < numberOfPlayerShots; i++) {
		if (!playerShots[i].isOnScreen) {
			playerShots[i].xVelocity = xVelocity;
			playerShots[i].yVelocity = -yVelocity;
		}

		i++;

		if (!playerShots[i].isOnScreen) {
			playerShots[i].xVelocity = xVelocity;
			playerShots[i].yVelocity = yVelocity;
		}

		i++;
	}
}

void moveShots() {
	int shotMovedOffscreen = 0;

	for (i = 0; i < numberOfPlayerShots; i++) {
		if (playerShots[i].isOnScreen) {
			if ((playerShots[i].xCoordinate <= shotXCoordinateLowerBoundary) ||
				(playerShots[i].xCoordinate >= shotXCoordinateUpperBoundary) ||
				(playerShots[i].yCoordinate <= shotYCoordinateLowerBoundary) ||
				(playerShots[i].yCoordinate >= shotYCoordinateUpperBoundary)) {

				playerShots[i].xCoordinate = offScreen;
				playerShots[i].yCoordinate = offScreen;
				playerShots[i].isOnScreen = 0;

				shotMovedOffscreen = 1;
			}
			else {
				playerShots[i].xCoordinate += playerShots[i].xVelocity;
				playerShots[i].yCoordinate += playerShots[i].yVelocity;
			}

			move_sprite(playerShots[i].spriteNumber, playerShots[i].xCoordinate, playerShots[i].yCoordinate);
		}
	}

	// If any of the shots have moved offscreen, be sure to update the rotating
	// shots' velocities so their direction is correct upon the next fire.
	if (shotMovedOffscreen) {
		updateRotatingShotsVelocity();
	}

	for (i = 0; i < numberOfEnemyShots; i++) {
		if (enemyShots[i].isOnScreen) {
			if ((enemyShots[i].xCoordinate <= shotXCoordinateLowerBoundary) ||
				(enemyShots[i].xCoordinate >= shotXCoordinateUpperBoundary) ||
				(enemyShots[i].yCoordinate <= shotYCoordinateLowerBoundary) ||
				(enemyShots[i].yCoordinate >= shotYCoordinateUpperBoundary)) {

				enemyShots[i].xCoordinate = offScreen;
				enemyShots[i].yCoordinate = offScreen;
				enemyShots[i].isOnScreen = 0;
			}
			else {
				enemyShots[i].xCoordinate += enemyShots[i].xVelocity;
				enemyShots[i].yCoordinate += enemyShots[i].yVelocity;
			}

			move_sprite(enemyShots[i].spriteNumber, enemyShots[i].xCoordinate, enemyShots[i].yCoordinate);
		}
	}
}

// Collision detection between the player's shots and the enemy
void testShotAndEnemyCollision(Enemy* enemy) {
	int i;

	for (i = 0; i < numberOfPlayerShots; i++) {
		if (playerShots[i].yCoordinate > enemy->yCoordinate - 8) {
			if (playerShots[i].yCoordinate < enemy->yCoordinate + 8) {
				if (playerShots[i].xCoordinate > enemy->xCoordinate - 8) {
					if (playerShots[i].xCoordinate < enemy->xCoordinate + 8) {
						enemy->xCoordinate = offScreen;
						enemy->yCoordinate = offScreen;
						enemy->isOnScreen = 0;

						// Move the enemy sprite off screen
						move_sprite(enemy->spriteNumber, offScreen, offScreen);

						// Move the shot sprite off screen
						move_sprite(playerShots[i].spriteNumber, offScreen, offScreen);

						playerShots[i].isOnScreen = 0;
					}
				}
			}
		}
	}
}

void initializeEnemies() {
	enemies[0].xCoordinate = firstEnemy.xCoordinate;
	enemies[0].yCoordinate = firstEnemy.yCoordinate;
	enemies[0].movementPathType = firstEnemy.movementPathType;
	enemies[0].movementCounter = firstEnemy.movementCounter;
	enemies[0].changeInMovement = firstEnemy.changeInMovement;
	enemies[0].shotCounter = firstEnemy.shotCounter;
	enemies[0].isOnScreen = firstEnemy.isOnScreen;
	enemies[0].spriteNumber = firstEnemy.spriteNumber;

	enemies[1].xCoordinate = secondEnemy.xCoordinate;
	enemies[1].yCoordinate = secondEnemy.yCoordinate;
	enemies[1].movementPathType = secondEnemy.movementPathType;
	enemies[1].movementCounter = secondEnemy.movementCounter;
	enemies[1].changeInMovement = secondEnemy.changeInMovement;
	enemies[1].shotCounter = secondEnemy.shotCounter;
	enemies[1].isOnScreen = secondEnemy.isOnScreen;
	enemies[1].spriteNumber = secondEnemy.spriteNumber;

	enemies[2].xCoordinate = thirdEnemy.xCoordinate;
	enemies[2].yCoordinate = thirdEnemy.yCoordinate;
	enemies[2].movementPathType = thirdEnemy.movementPathType;
	enemies[2].movementCounter = thirdEnemy.movementCounter;
	enemies[2].changeInMovement = thirdEnemy.changeInMovement;
	enemies[2].shotCounter = thirdEnemy.shotCounter;
	enemies[2].isOnScreen = thirdEnemy.isOnScreen;
	enemies[2].spriteNumber = thirdEnemy.spriteNumber;

	enemies[3].xCoordinate = fourthEnemy.xCoordinate;
	enemies[3].yCoordinate = fourthEnemy.yCoordinate;
	enemies[3].movementPathType = fourthEnemy.movementPathType;
	enemies[3].movementCounter = fourthEnemy.movementCounter;
	enemies[3].changeInMovement = fourthEnemy.changeInMovement;
	enemies[3].shotCounter = fourthEnemy.shotCounter;
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
	int i, spriteNumber;

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
				}
			}
			else if (enemies[i].movementPathType == movementPathStraightDownwardDiagonal) {
				if (enemies[i].changeInMovement < enemies[i].movementCounter) {
					enemies[i].xCoordinate--;
					enemies[i].yCoordinate++;
				}
				else {
					enemies[i].xCoordinate--;
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
				}
			}
			else if (enemies[i].movementPathType == movementPathStop) {
				if (enemies[i].changeInMovement > enemies[i].movementCounter) {
					enemies[i].xCoordinate--;
				}
			}
			else if (enemies[i].movementPathType == movementPathStraightAndReturn) {
				if (enemies[i].changeInMovement < enemies[i].movementCounter) {
					enemies[i].xCoordinate++;
				}
				else {
					enemies[i].xCoordinate--;
				}
			}

			enemies[i].movementCounter++;

			if (enemies[i].shotCounter == enemies[i].movementCounter) {
				for (i = 0; i < numberOfEnemyShots; i++) {
					if (!enemyShots[i].isOnScreen) {
						enemyShots[i].xCoordinate = enemies[i].xCoordinate;
						enemyShots[i].yCoordinate = enemies[i].yCoordinate;
						enemyShots[i].xVelocity = -2;
						enemyShots[i].isOnScreen = 1;
						enemyShots[i].yVelocity = 1;

						spriteNumber = i + first_enemy_shot;

						enemyShots[i].spriteNumber = spriteNumber;

						break;
					}
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
