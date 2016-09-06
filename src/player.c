#include <gb/gb.h>

#include "../include/player.h"
#include "../include/sprite_and_background.h"

#define numberOfShots 9
#define numberOfShotsInGroup 3

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

// Create a timer to keep track of when the ship can shoot next
UBYTE timeToShoot;

// Create a counter to keep track of which shot we should be firing
UBYTE shotCounter;

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

Shot shots[numberOfShots];

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

	// Incrementing within the loop to allow for differing velocities from
	// the inception of the shots.
	for (i = 0; i != numberOfShots; i++) {
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

	// Allow the player to shoot immediately
	timeToShoot = 0;
}

void moveShotsToPlayer() {
	// Set all of the shots to be where the player is
	// so it looks like the shot is coming from the ship

	// Test to see if the current group of shots is already on screen
	if (shots[shotCounter].isOnScreen || shots[shotCounter + 1].isOnScreen ||
		shots[shotCounter + 2].isOnScreen) {

		return;
	}

	// Only loop as far as the number of shots in a group allows
	for (i = 0; i != numberOfShotsInGroup; i++) {
		shots[shotCounter].xCoordinate = player.xCoordinate;
		shots[shotCounter].yCoordinate = player.yCoordinate;
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

void movePlayer() {
	// Move both the left and right halves of the ship to its new location
	// as set in 'updatePlayerAndShot'
	move_sprite(left_half_ship, player.xCoordinate, player.yCoordinate);
	move_sprite(right_half_ship, player.xCoordinate + offset, player.yCoordinate);
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
	for (i = 1; i != numberOfShots; i++) {
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

	for (i = 0; i != numberOfShots; i++) {
		if (shots[i].isOnScreen) {
			if ((shots[i].xCoordinate <= xCoordinateLowerBoundary) ||
				(shots[i].xCoordinate >= xCoordinateUpperBoundary) ||
				(shots[i].yCoordinate <= yCoordinateLowerBoundary) ||
				(shots[i].yCoordinate >= yCoordinateUpperBoundary)) {

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

	for (i = 0; i != numberOfShots; i++) {
		if (shots[i].yCoordinate > enemy->yCoordinate - 8) {
			if (shots[i].yCoordinate < enemy->yCoordinate + 8) {
				if (shots[i].xCoordinate > enemy->xCoordinate - 8) {
					if (shots[i].xCoordinate < enemy->xCoordinate + 8) {
						enemy->xCoordinate = offScreen;
						enemy->yCoordinate = offScreen;

						move_sprite(enemy->spriteNumber, offScreen, offScreen);
					}
				}
			}
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

	if (key & J_B) {
		if (timeToShoot == 0) {
			moveShotsToPlayer();

			// The main loop will run at 60 frames per second. Setting
			// 'timeToShoot' will have the ship fire every 15 frames or
			// 1/4th of a second.
			timeToShoot = 15;
		}
	}

	// Decrease the timeToShoot counter irregardless of input in case the player
	// is not holding down the fire button.
	if (timeToShoot != 0) {
		timeToShoot--;
	}

	// Move the shots irregardless of input since some shots might already be
	// on the screen and require movement.
	moveShots();
}
