#include <gb.h>

// Initial X coordinate starting point
int playerXCoordinate = 50;

// Set the X coordinate boundaries so the sprite does not go off the screen
int xCoordinateLowerBoundary = 8;
int xCoordinateUpperBoundary = 160;

// Initial Y coordinate starting point
int playerYCoordinate = 32;

// Set the Y coordinate boundaries so the sprite does not go off the screen
int yCoordinateLowerBoundary = 16;
int yCoordinateUpperBoundary = 140;

// Set the offset of height between the head and the body of the sprite
int offset = 8;

// Boolean to determine whether or not the player can shoot
int playerCanShoot = 1;

// Initial X and Y coordinate starting points for the shot
int shotXCoordinate = 0;
int shotYCoordinate = 0;

// Set the off screen coordinates to hide the sprite
int offScreen = 200;

void initializePlayer() {
	// Move the sprite on to the screen so we can see it
	move_sprite(0, playerXCoordinate, playerYCoordinate);
	move_sprite(1, playerXCoordinate + offset, playerYCoordinate);
}

void testShotAndEnemyCollision(Enemy *enemy) {
	// Collision detection between the player's shot and the enemy
	if (shotYCoordinate > enemy->yCoordinate - 8) {
		if (shotYCoordinate < enemy->yCoordinate + 8) {
			if (shotXCoordinate > enemy->xCoordinate - 8) {
				if (shotXCoordinate < enemy->xCoordinate + 8) {
					playerCanShoot = 1;

					enemy->xCoordinate = offScreen;
					enemy->yCoordinate = offScreen;

					move_sprite(enemy->spriteNumber, offScreen, offScreen);
					move_sprite(4, offScreen, offScreen);
				}
			}
		}
	}
}

void updatePlayerAndShot(int key) {
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

		// Move both the head and body of the sprite to the new X location
		move_sprite(0, playerXCoordinate, playerYCoordinate);
		move_sprite(1, playerXCoordinate + offset, playerYCoordinate);
	}

	if (key & J_LEFT) {
		playerXCoordinate--;

		if (playerXCoordinate < xCoordinateLowerBoundary) {
			playerXCoordinate = xCoordinateLowerBoundary;
		}

		move_sprite(0, playerXCoordinate, playerYCoordinate);
		move_sprite(1, playerXCoordinate + offset, playerYCoordinate);
	}

	if (key & J_UP) {
		playerYCoordinate--;

		if (playerYCoordinate < yCoordinateLowerBoundary) {
			playerYCoordinate = yCoordinateLowerBoundary;
		}

		move_sprite(0, playerXCoordinate, playerYCoordinate);
		move_sprite(1, playerXCoordinate + offset, playerYCoordinate);
	}

	if (key & J_DOWN) {
		playerYCoordinate++;

		if (playerYCoordinate > yCoordinateUpperBoundary) {
			playerYCoordinate = yCoordinateUpperBoundary;
		}

		move_sprite(0, playerXCoordinate, playerYCoordinate);
		move_sprite(1, playerXCoordinate + offset, playerYCoordinate);
	}

	if ((key & J_A) && playerCanShoot) {
		playerCanShoot = 0;

		shotXCoordinate = playerXCoordinate;
		shotYCoordinate = playerYCoordinate;

		move_sprite(4, shotXCoordinate, shotYCoordinate);
	}

	if (!playerCanShoot) {
		shotXCoordinate += 2;

		if (shotXCoordinate >= xCoordinateUpperBoundary) {
			move_sprite(4, offScreen, offScreen);

			playerCanShoot = 1;
		}
		else {
			move_sprite(4, shotXCoordinate, shotYCoordinate);
		}
	}
}