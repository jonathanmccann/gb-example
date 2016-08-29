#include <gb.h>

#define numberOfShots 3

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

// Initial X and Y coordinate starting points for the three shots
int straightShotXCoordinate = 0;
int straightShotYCoordinate = 0;

// Set the off screen coordinates to hide the sprite
int offScreen = 0;

typedef struct {
	// Initial X coordinate starting point for enemy
	int xCoordinate;

	// Initial Y coordinate starting point for enemy
	int yCoordinate;

	// Boolean to determine whether the enemy is moving up or down
	BOOLEAN isOnScreen;

	// Set the sprite number associated with the enemy
	int spriteNumber;
} Shot;

Shot shots[numberOfShots];

Shot *shotPointers[numberOfShots];

void initializePlayer() {
	// Move the sprite on to the screen so we can see it
	move_sprite(left_half_ship, playerXCoordinate, playerYCoordinate);
	move_sprite(right_half_ship, playerXCoordinate + offset, playerYCoordinate);
}

void initializeShots() {
	/*
	Set up the shots to confirm that "isOnScreen"
	is false. Then, use another array as a pointer
	to the "shots" array to avoid multiplication
	and strange results when accessing the original
	array. See http://zalods.blogspot.com.es/2016/07/game-boy-development-tips-and-tricks-ii.html
	for more details.
	*/

	shots[0].xCoordinate = offScreen;
	shots[0].yCoordinate = offScreen;
	shots[0].spriteNumber = straight_shot;
	shots[0].isOnScreen = FALSE;

	shotPointers[0] = &shots[0];

	shots[1].xCoordinate = offScreen;
	shots[1].yCoordinate = offScreen;
	shots[1].spriteNumber = upper_shot;
	shots[1].isOnScreen = FALSE;

	shotPointers[1] = &shots[1];

	shots[2].xCoordinate = offScreen;
	shots[2].yCoordinate = offScreen;
	shots[2].spriteNumber = lower_shot;
	shots[2].isOnScreen = FALSE;

	shotPointers[2] = &shots[2];
}

void moveShotsToPlayer() {
	// Set all of the shots to be where the player is
	// so it looks like the shot is coming from the ship

	shots[0].xCoordinate = playerXCoordinate;
	shots[0].yCoordinate = playerYCoordinate;
	shots[0].isOnScreen = TRUE;

	move_sprite(straight_shot, playerXCoordinate, playerYCoordinate);

	shots[1].xCoordinate = playerXCoordinate;
	shots[1].yCoordinate = playerYCoordinate;
	shots[1].isOnScreen = TRUE;

	move_sprite(upper_shot, playerXCoordinate, playerYCoordinate);

	shots[2].xCoordinate = playerXCoordinate;
	shots[2].yCoordinate = playerYCoordinate;
	shots[2].isOnScreen = TRUE;

	move_sprite(lower_shot, playerXCoordinate, playerYCoordinate);
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
	if (shotPointers[0]->isOnScreen) {
		if (shotPointers[0]->xCoordinate >= xCoordinateUpperBoundary) {
			shotPointers[0]->xCoordinate = offScreen;
			shotPointers[0]->yCoordinate = offScreen;
			shotPointers[0]->isOnScreen = FALSE;
		}
		else {
			shotPointers[0]->xCoordinate += 2;
		}

		move_sprite(shotPointers[0]->spriteNumber, shotPointers[0]->xCoordinate, shotPointers[0]->yCoordinate);
	}

	// Move the second shot in an upward angle (decrease the Y coordinate)
	if (shotPointers[1]->isOnScreen) {
		if ((shotPointers[1]->xCoordinate >= xCoordinateUpperBoundary) ||
			(shotPointers[1]->yCoordinate <= yCoordinateLowerBoundary)) {

			shotPointers[1]->xCoordinate = offScreen;
			shotPointers[1]->yCoordinate = offScreen;
			shotPointers[1]->isOnScreen = FALSE;
		}
		else {
			shotPointers[1]->xCoordinate += 2;
			shotPointers[1]->yCoordinate -= 1;
		}

		move_sprite(shotPointers[1]->spriteNumber, shotPointers[1]->xCoordinate, shotPointers[1]->yCoordinate);
	}

	// Move the third shot in an downward angle (increase the Y coordinate)
	if (shotPointers[2]->isOnScreen) {
		if ((shotPointers[2]->xCoordinate >= xCoordinateUpperBoundary) ||
			(shotPointers[2]->yCoordinate >= yCoordinateUpperBoundary)) {

			shotPointers[2]->xCoordinate = offScreen;
			shotPointers[2]->yCoordinate = offScreen;
			shotPointers[2]->isOnScreen = FALSE;
			move_sprite(shotPointers[2]->spriteNumber, shotPointers[2]->xCoordinate, shotPointers[2]->yCoordinate);
		}
		else {
			shotPointers[2]->xCoordinate += 2;
			shotPointers[2]->yCoordinate += 1;
			move_sprite(shotPointers[2]->spriteNumber, shotPointers[2]->xCoordinate, shotPointers[2]->yCoordinate);
		}
	}
}

// Collision detection between the player's shot and the enemy
// 'enemy' is the array defined in 'enemy.c'
void testShotAndEnemyCollision() {
	for (i = 0; i < numberOfEnemies; i++) {
		if (straightShotYCoordinate > enemies[i].yCoordinate - 8) {
			if (straightShotYCoordinate < enemies[i].yCoordinate + 8) {
				if (straightShotXCoordinate > enemies[i].xCoordinate - 8) {
					if (straightShotXCoordinate < enemies[i].xCoordinate + 8) {
						enemies[i].xCoordinate = offScreen;
						enemies[i].yCoordinate = offScreen;

						move_sprite(enemies[i].spriteNumber, offScreen, offScreen);
						//move_sprite(10, offScreen, offScreen);
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

	if ((key & J_A) && !shotPointers[0]->isOnScreen &&
		!shotPointers[1]->isOnScreen && !shotPointers[2]->isOnScreen) {

		moveShotsToPlayer();
	}

	if (shotPointers[0]->isOnScreen || shotPointers[1]->isOnScreen ||
		shotPointers[2]->isOnScreen) {

		moveShots();
	}
}
