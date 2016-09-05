#include <gb/gb.h>

#include "../include/player.h"
#include "../include/sprite_and_background.h"

#define numberOfRotatingShots 2
#define numberOfStraightShots 1

#define forwardShot 0
#define upAndDownShot 1
#define behindShot 2

// Set the X coordinate boundaries so the sprite does not go off the screen
UBYTE xCoordinateLowerBoundary = 8;
UBYTE xCoordinateUpperBoundary = 152;

// Set the Y coordinate boundaries so the sprite does not go off the screen
UBYTE yCoordinateLowerBoundary = 16;
UBYTE yCoordinateUpperBoundary = 150;

// Set the offset of width between the left and right halves of the ship
int offset = 8;

// Set the off screen coordinates to hide the sprite
int offScreen = 0;

// Create an iterator for use across all functions
UBYTE i, keyADown, rotatingShotDirection;

typedef struct {
	UBYTE xCoordinate;
	UBYTE yCoordinate;
	int leftSpriteNumber;
	int rightSpriteNumber;
} Player;

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

	// Set the sprite number associated with the enemy
	int spriteNumber;
} Shot;

Player player;

Shot rotatingShots[numberOfRotatingShots];
Shot straightShots[numberOfStraightShots];

void initializePlayer() {
	// Move the sprite on to the screen so we can see it
	player.xCoordinate = 50;
	player.yCoordinate = 32;
	player.leftSpriteNumber = left_half_ship;
	player.rightSpriteNumber = right_half_ship;

	move_sprite(player.leftSpriteNumber, player.xCoordinate, player.yCoordinate);
	move_sprite(player.rightSpriteNumber, player.xCoordinate + offset, player.yCoordinate);
}

void initializeShots() {
	// Set up the shots to confirm that "isOnScreen" is false.

	for (i = 0; i < numberOfStraightShots; i++) {
		straightShots[i].xCoordinate = offScreen;
		straightShots[i].yCoordinate = offScreen;
		straightShots[i].xVelocity = 2;
		straightShots[i].yVelocity = 0;
		straightShots[i].spriteNumber = straight_shot_sprite_starting_position + i;
		straightShots[i].isOnScreen = 0;
	}

	for (i = 0; i < numberOfRotatingShots; i++) {
		rotatingShots[i].xCoordinate = offScreen;
		rotatingShots[i].yCoordinate = offScreen;
		rotatingShots[i].xVelocity = 2;
		rotatingShots[i].yVelocity = -1;
		rotatingShots[i].spriteNumber = rotating_shot_sprite_starting_position + i;
		rotatingShots[i].isOnScreen = 0;

		i++;

		rotatingShots[i].xCoordinate = offScreen;
		rotatingShots[i].yCoordinate = offScreen;
		rotatingShots[i].xVelocity = 2;
		rotatingShots[i].yVelocity = 1;
		rotatingShots[i].spriteNumber = rotating_shot_sprite_starting_position + i;
		rotatingShots[i].isOnScreen = 0;
	}
}

void moveShotsToPlayer() {
	// Set all of the shots to be where the player is
	// so it looks like the shot is coming from the ship

	for (i = 0; i < numberOfStraightShots; i++) {
		straightShots[i].xCoordinate = player.xCoordinate;
		straightShots[i].yCoordinate = player.yCoordinate;
		straightShots[i].isOnScreen = 1;

		move_sprite(straightShots[i].spriteNumber, straightShots[i].xCoordinate, straightShots[i].yCoordinate);
	}

	for (i = 0; i < numberOfRotatingShots; i++) {
		rotatingShots[i].xCoordinate = player.xCoordinate;
		rotatingShots[i].yCoordinate = player.yCoordinate;
		rotatingShots[i].isOnScreen = 1;

		move_sprite(rotatingShots[i].spriteNumber, rotatingShots[i].xCoordinate, rotatingShots[i].yCoordinate);
	}
}

void movePlayer() {
	// Move both the left and right halves of the ship to its new location
	// as set in 'updatePlayerAndShot'
	move_sprite(left_half_ship, player.xCoordinate, player.yCoordinate);
	move_sprite(right_half_ship, player.xCoordinate + offset, player.yCoordinate);
}

void moveRotatingShots() {
	for (i = 0; i < numberOfRotatingShots; i++) {
		if (rotatingShots[i].isOnScreen) {
			if ((rotatingShots[i].xCoordinate <= xCoordinateLowerBoundary) ||
				(rotatingShots[i].xCoordinate >= xCoordinateUpperBoundary) ||
				(rotatingShots[i].yCoordinate <= yCoordinateLowerBoundary) ||
				(rotatingShots[i].yCoordinate >= yCoordinateUpperBoundary)) {

				rotatingShots[i].xCoordinate = offScreen;
				rotatingShots[i].yCoordinate = offScreen;
				rotatingShots[i].isOnScreen = 0;
			}
			else {
				rotatingShots[i].xCoordinate += rotatingShots[i].xVelocity;
				rotatingShots[i].yCoordinate += rotatingShots[i].yVelocity;
			}

			move_sprite(rotatingShots[i].spriteNumber, rotatingShots[i].xCoordinate, rotatingShots[i].yCoordinate);
		}
	}
}

void moveStraightShots() {
	for (i = 0; i < numberOfStraightShots; i++) {
		if (straightShots[i].isOnScreen) {
			if ((straightShots[i].xCoordinate <= xCoordinateLowerBoundary) ||
				(straightShots[i].xCoordinate >= xCoordinateUpperBoundary) ||
				(straightShots[i].yCoordinate <= yCoordinateLowerBoundary) ||
				(straightShots[i].yCoordinate >= yCoordinateUpperBoundary)) {

				straightShots[i].xCoordinate = offScreen;
				straightShots[i].yCoordinate = offScreen;
				straightShots[i].isOnScreen = 0;
			}
			else {
				straightShots[i].xCoordinate += straightShots[i].xVelocity;
			}

			move_sprite(straightShots[i].spriteNumber, straightShots[i].xCoordinate, straightShots[i].yCoordinate);
		}
	}
}

// Collision detection between the player's shots and the enemy
void testShotAndEnemyCollision(Enemy* enemy) {
	int i;

	for (i = 0; i < numberOfStraightShots; i++) {
		if (straightShots[i].yCoordinate > enemy->yCoordinate - 8) {
			if (straightShots[i].yCoordinate < enemy->yCoordinate + 8) {
				if (straightShots[i].xCoordinate > enemy->xCoordinate - 8) {
					if (straightShots[i].xCoordinate < enemy->xCoordinate + 8) {
						enemy->xCoordinate = offScreen;
						enemy->yCoordinate = offScreen;

						move_sprite(enemy->spriteNumber, offScreen, offScreen);
					}
				}
			}
		}
	}

	for (i = 0; i < numberOfRotatingShots; i++) {
		if (rotatingShots[i].yCoordinate > enemy->yCoordinate - 8) {
			if (rotatingShots[i].yCoordinate < enemy->yCoordinate + 8) {
				if (rotatingShots[i].xCoordinate > enemy->xCoordinate - 8) {
					if (rotatingShots[i].xCoordinate < enemy->xCoordinate + 8) {
						enemy->xCoordinate = offScreen;
						enemy->yCoordinate = offScreen;

						move_sprite(enemy->spriteNumber, offScreen, offScreen);
					}
				}
			}
		}
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
		yVelocity = -1;
	}
	else if (rotatingShotDirection == upAndDownShot) {
		xVelocity = 0;
		yVelocity = 2;
	}
	else {
		xVelocity = -2;
		yVelocity = -1;
	}

	for (i = 0; i < numberOfRotatingShots; i++) {
		if (!rotatingShots[i].isOnScreen) {
			rotatingShots[i].xVelocity = xVelocity;
			rotatingShots[i].yVelocity = yVelocity;
		}

		i++;

		if (!rotatingShots[i].isOnScreen) {
			rotatingShots[i].xVelocity = xVelocity;
			rotatingShots[i].yVelocity = -yVelocity;
		}
	}
}

void updatePlayerAndShots(int key) {
	// '&' is used in conjunction with if statments in case
	// Both right and up (or another combination) is being
	// pushed so that the sprite can move diagonally
	if (key & J_RIGHT) {
		player.xCoordinate++;

		// Check to see if the X coordinate is greater than the upper boundary
		// If so, then do not let the sprite move beyond it
		if (player.xCoordinate > xCoordinateUpperBoundary) {
			player.xCoordinate = xCoordinateUpperBoundary;
		}

		movePlayer();
	}

	if (key & J_LEFT) {
		player.xCoordinate--;

		if (player.xCoordinate < xCoordinateLowerBoundary) {
			player.xCoordinate = xCoordinateLowerBoundary;
		}

		movePlayer();
	}

	if (key & J_UP) {
		player.yCoordinate--;

		if (player.yCoordinate < yCoordinateLowerBoundary) {
			player.yCoordinate = yCoordinateLowerBoundary;
		}

		movePlayer();
	}

	if (key & J_DOWN) {
		player.yCoordinate++;

		if (player.yCoordinate > yCoordinateUpperBoundary) {
			player.yCoordinate = yCoordinateUpperBoundary;
		}

		movePlayer();
	}

	if (key & J_A) {
		keyADown = 1;
	}
	else if (keyADown) {
		keyADown = 0;

		updateRotatingShotDirection();
		updateRotatingShotsVelocity();
	}

	if ((key & J_B) && !straightShots[0].isOnScreen &&
		!rotatingShots[0].isOnScreen && !rotatingShots[1].isOnScreen) {

		moveShotsToPlayer();
	}

	if (straightShots[0].isOnScreen || rotatingShots[0].isOnScreen ||
		rotatingShots[1].isOnScreen) {

		moveStraightShots();
		moveRotatingShots();
	}
}
