#include <gb/gb.h>

#include "../include/player.h"
#include "../include/sprite_and_background.h"

#define numberOfShots 3

// Initial X coordinate starting point
UBYTE playerXCoordinate = 50;

// Set the X coordinate boundaries so the sprite does not go off the screen
UBYTE xCoordinateLowerBoundary = 8;
UBYTE xCoordinateUpperBoundary = 160;

// Initial Y coordinate starting point
UBYTE playerYCoordinate = 32;

// Set the Y coordinate boundaries so the sprite does not go off the screen
UBYTE yCoordinateLowerBoundary = 16;
UBYTE yCoordinateUpperBoundary = 140;

// Set the offset of width between the left and right halves of the ship
int offset = 8;

// Set the off screen coordinates to hide the sprite
int offScreen = 0;

// Create an iterator for use across all functions
UBYTE i;

typedef struct {
	// Initial X coordinate starting point for the shot
	UBYTE xCoordinate;

	// Initial Y coordinate starting point for the shot
	UBYTE yCoordinate;

	// Boolean to determine whether the enemy is moving up or down
	UBYTE isOnScreen;

	// Set the sprite number associated with the enemy
	int spriteNumber;
} Shot;

Shot shots[numberOfShots];

void initializePlayer() {
	// Move the sprite on to the screen so we can see it
	move_sprite(left_half_ship, playerXCoordinate, playerYCoordinate);
	move_sprite(right_half_ship, playerXCoordinate + offset, playerYCoordinate);
}

void initializeShots() {
	// Set up the shots to confirm that "isOnScreen" is false.

	for (i = 0; i <= numberOfShots; i++) {
		shots[i].xCoordinate = offScreen;
		shots[i].yCoordinate = offScreen;
		shots[i].spriteNumber = shot_sprite_starting_position + i;
		shots[i].isOnScreen = 0;
	}
}

void moveShotsToPlayer() {
	// Set all of the shots to be where the player is
	// so it looks like the shot is coming from the ship

	for (i = 0; i <= numberOfShots; i++) {
		shots[i].xCoordinate = playerXCoordinate;
		shots[i].yCoordinate = playerYCoordinate;
		shots[i].isOnScreen = 1;

		move_sprite(shots[i].spriteNumber, shots[i].xCoordinate, shots[i].yCoordinate);
	}
}

void movePlayer() {
	// Move both the left and right halves of the ship to its new location
	// as set in 'updatePlayerAndShot'
	move_sprite(left_half_ship, playerXCoordinate, playerYCoordinate);
	move_sprite(right_half_ship, playerXCoordinate + offset, playerYCoordinate);
}

void moveShots() {
	// Move the first shot in a straight line.
	// If it is off screen, then reset the shot.
	if (shots[0].isOnScreen) {
		if (shots[0].xCoordinate >= xCoordinateUpperBoundary) {
			shots[0].xCoordinate = offScreen;
			shots[0].yCoordinate = offScreen;
			shots[0].isOnScreen = 0;
		}
		else {
			shots[0].xCoordinate += 2;
		}

		move_sprite(shots[0].spriteNumber, shots[0].xCoordinate, shots[0].yCoordinate);
	}

	// Move the second shot in an upward angle (decrease the Y coordinate)
	if (shots[1].isOnScreen) {
		if ((shots[1].xCoordinate >= xCoordinateUpperBoundary) ||
			(shots[1].yCoordinate <= yCoordinateLowerBoundary)) {

			shots[1].xCoordinate = offScreen;
			shots[1].yCoordinate = offScreen;
			shots[1].isOnScreen = 0;
		}
		else {
			shots[1].xCoordinate += 2;
			shots[1].yCoordinate -= 1;
		}

		move_sprite(shots[1].spriteNumber, shots[1].xCoordinate, shots[1].yCoordinate);
	}

	// Move the third shot in an downward angle (increase the Y coordinate)
	if (shots[2].isOnScreen) {
		if ((shots[2].xCoordinate >= xCoordinateUpperBoundary) ||
			(shots[2].yCoordinate >= yCoordinateUpperBoundary)) {

			shots[2].xCoordinate = offScreen;
			shots[2].yCoordinate = offScreen;
			shots[2].isOnScreen = 0;
		}
		else {
			shots[2].xCoordinate += 2;
			shots[2].yCoordinate += 1;
		}

		move_sprite(shots[2].spriteNumber, shots[2].xCoordinate, shots[2].yCoordinate);
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
		playerXCoordinate++;

		// Check to see if the X coordinate is greater than the upper boundary
		// If so, then do not let the sprite move beyond it
		if (playerXCoordinate > xCoordinateUpperBoundary) {
			playerXCoordinate = xCoordinateUpperBoundary;
		}

		movePlayer();
	}

	if (key & J_LEFT) {
		playerXCoordinate--;

		if (playerXCoordinate < xCoordinateLowerBoundary) {
			playerXCoordinate = xCoordinateLowerBoundary;
		}

		movePlayer();
	}

	if (key & J_UP) {
		playerYCoordinate--;

		if (playerYCoordinate < yCoordinateLowerBoundary) {
			playerYCoordinate = yCoordinateLowerBoundary;
		}

		movePlayer();
	}

	if (key & J_DOWN) {
		playerYCoordinate++;

		if (playerYCoordinate > yCoordinateUpperBoundary) {
			playerYCoordinate = yCoordinateUpperBoundary;
		}

		movePlayer();
	}

	if ((key & J_A) && !shots[0].isOnScreen &&
		!shots[1].isOnScreen && !shots[2].isOnScreen) {

		moveShotsToPlayer();
	}

	if (shots[0].isOnScreen || shots[1].isOnScreen ||
		shots[2].isOnScreen) {

		moveShots();
	}
}
