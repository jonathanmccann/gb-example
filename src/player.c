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

// Set the offset of width between the left and right halves of the ship
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
	move_sprite(left_half_ship, playerXCoordinate, playerYCoordinate);
	move_sprite(right_half_ship, playerXCoordinate + offset, playerYCoordinate);
}

// Collision detection between the player's shot and the enemy
// 'enemy' is the array defined in 'enemy.c'
void testShotAndEnemyCollision() {
	for (i = 0; i < numberOfEnemies; i++) {
		if (shotYCoordinate > enemies[i].yCoordinate - 8) {
			if (shotYCoordinate < enemies[i].yCoordinate + 8) {
				if (shotXCoordinate > enemies[i].xCoordinate - 8) {
					if (shotXCoordinate < enemies[i].xCoordinate + 8) {
						playerCanShoot = 1;

						enemies[i].xCoordinate = offScreen;
						enemies[i].yCoordinate = offScreen;

						move_sprite(enemies[i].spriteNumber, offScreen, offScreen);
						move_sprite(player_shot, offScreen, offScreen);
					}
				}
			}
		}
	}
}

void movePlayer() {
	// Move both the left and right halves of the ship to its new location
	// as set in 'updatePlayerAndShot'
	move_sprite(left_half_ship, playerXCoordinate, playerYCoordinate);
	move_sprite(right_half_ship, playerXCoordinate + offset, playerYCoordinate);
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

	if ((key & J_A) && playerCanShoot) {
		playerCanShoot = 0;

		shotXCoordinate = playerXCoordinate;
		shotYCoordinate = playerYCoordinate;

		move_sprite(player_shot, shotXCoordinate, shotYCoordinate);
	}

	if (!playerCanShoot) {
		shotXCoordinate += 2;

		if (shotXCoordinate >= xCoordinateUpperBoundary) {
			move_sprite(player_shot, offScreen, offScreen);

			playerCanShoot = 1;
		}
		else {
			move_sprite(player_shot, shotXCoordinate, shotYCoordinate);
		}
	}
}