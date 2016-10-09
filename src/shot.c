#include <gb/gb.h>

#include "../include/enemy.h"
#include "../include/global.h"
#include "../include/player.h"
#include "../include/sprite_and_background.h"

#define numberOfPlayerShots 9
#define numberOfShotsInGroup 3

#define forwardShot 0
#define upAndDownShot 1
#define behindShot 2

#define shotXCoordinateLowerBoundary 0
#define shotXCoordinateUpperBoundary 168
#define shotYCoordinateLowerBoundary 0
#define shotYCoordinateUpperBoundary 160

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
