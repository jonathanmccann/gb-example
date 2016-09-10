#include <gb/gb.h>

#include "../include/enemy.h"
#include "../include/player.h"
#include "../include/sprite_and_background.h"

#define numberOfShots 9
#define numberOfShotsInGroup 3

#define forwardShot 0
#define upAndDownShot 1
#define behindShot 2

#define shotXCoordinateLowerBoundary 0
#define shotXCoordinateUpperBoundary 168
#define shotYCoordinateLowerBoundary 0
#define shotYCoordinateUpperBoundary 160

UBYTE i, rotatingShotDirection;

int offScreen = 0;

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

	// Set the sprite number associated with the enemy
	int spriteNumber;
} Shot;

Shot shots[numberOfShots];

void initializeShots() {
	// Set up the shots to confirm that "isOnScreen" is false.

	// Incrementing within the loop to allow for differing velocities from
	// the inception of the shots.
	for (i = 0; i < numberOfShots; i++) {
		shots[i].xCoordinate = offScreen;
		shots[i].yCoordinate = offScreen;
		shots[i].xVelocity = 2;
		shots[i].yVelocity = 0;
		shots[i].spriteNumber = shot_sprite_starting_position + i;
		shots[i].isOnScreen = 0;

		i++;

		shots[i].xCoordinate = offScreen;
		shots[i].yCoordinate = offScreen;
		shots[i].xVelocity = 2;
		shots[i].yVelocity = -1;
		shots[i].spriteNumber = shot_sprite_starting_position + i;
		shots[i].isOnScreen = 0;

		i++;

		shots[i].xCoordinate = offScreen;
		shots[i].yCoordinate = offScreen;
		shots[i].xVelocity = 2;
		shots[i].yVelocity = 1;
		shots[i].spriteNumber = shot_sprite_starting_position + i;
		shots[i].isOnScreen = 0;
	}

	// Initialize the shotCounter to zero so we start from the beginning of the
	// 'shots' array.
	shotCounter = 0;
}

void moveShotsToPlayer(Player* player) {
	// Set all of the shots to be where the player is
	// so it looks like the shot is coming from the ship

	// Test to see if the current group of shots is already on screen
	if (shots[shotCounter].isOnScreen || shots[shotCounter + 1].isOnScreen ||
		shots[shotCounter + 2].isOnScreen) {

		return;
	}

	// Only loop as far as the number of shots in a group allows
	for (i = 0; i < numberOfShotsInGroup; i++) {
		shots[shotCounter].xCoordinate = player->xCoordinate;
		shots[shotCounter].yCoordinate = player->yCoordinate;
		shots[shotCounter].isOnScreen = 1;

		move_sprite(
			shots[shotCounter].spriteNumber,
			shots[shotCounter].xCoordinate,
			shots[shotCounter].yCoordinate);

		shotCounter++;
	}

	// Here, we aren't doing "numberOfShots - 1" since the loop above will
	// increment it before breaking out.
	if (shotCounter == numberOfShots) {
		shotCounter = 0;
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

	// Start at index 1 because the shots 0, 3, 6, etc. are straight shots
	// and their velocity does not need to be updated.
	for (i = 1; i < numberOfShots; i++) {
		if (!shots[i].isOnScreen) {
			shots[i].xVelocity = xVelocity;
			shots[i].yVelocity = yVelocity;
		}

		i++;

		if (!shots[i].isOnScreen) {
			shots[i].xVelocity = xVelocity;
			shots[i].yVelocity = -yVelocity;
		}

		i++;
	}
}

void moveShots() {
	int shotMovedOffscreen = 0;

	for (i = 0; i < numberOfShots; i++) {
		if (shots[i].isOnScreen) {
			if ((shots[i].xCoordinate <= shotXCoordinateLowerBoundary) ||
				(shots[i].xCoordinate >= shotXCoordinateUpperBoundary) ||
				(shots[i].yCoordinate <= shotYCoordinateLowerBoundary) ||
				(shots[i].yCoordinate >= shotYCoordinateUpperBoundary)) {

				shots[i].xCoordinate = offScreen;
				shots[i].yCoordinate = offScreen;
				shots[i].isOnScreen = 0;

				shotMovedOffscreen = 1;
			}
			else {
				shots[i].xCoordinate += shots[i].xVelocity;
				shots[i].yCoordinate += shots[i].yVelocity;
			}

			move_sprite(shots[i].spriteNumber, shots[i].xCoordinate, shots[i].yCoordinate);
		}
	}

	// If any of the shots have moved offscreen, be sure to update the rotating
	// shots' velocities so their direction is correct upon the next fire.
	if (shotMovedOffscreen) {
		updateRotatingShotsVelocity();
	}
}

// Collision detection between the player's shots and the enemy
void testShotAndEnemyCollision(Enemy* enemy) {
	int i;

	for (i = 0; i < numberOfShots; i++) {
		if (shots[i].yCoordinate > enemy->yCoordinate - 8) {
			if (shots[i].yCoordinate < enemy->yCoordinate + 8) {
				if (shots[i].xCoordinate > enemy->xCoordinate - 8) {
					if (shots[i].xCoordinate < enemy->xCoordinate + 8) {
						enemy->xCoordinate = offScreen;
						enemy->yCoordinate = offScreen;

						// Move the enemy sprite off screen
						move_sprite(enemy->spriteNumber, offScreen, offScreen);

						// Move the shot sprite off screen
						move_sprite(shots[i].spriteNumber, offScreen, offScreen);

						shots[i].isOnScreen = 0;
					}
				}
			}
		}
	}
}
