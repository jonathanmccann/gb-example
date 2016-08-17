#include <gb.h>

unsigned char man[] =
{
	// Head
	0x00,0x2B,0x00,0x7E,0x7E,0x42,0xFF,0xA5,
	0xFF,0x81,0xFF,0x99,0x7E,0x42,0x3C,0x3C,
	// Body style 1
	0x52,0x52,0x52,0x52,0x3C,0x3C,0x08,0x08,
	0x08,0x08,0x14,0x14,0x24,0x24,0x42,0x42,
	// Body style 2
	0x08,0x08,0x49,0x49,0x3E,0x3E,0x10,0x10,
	0x10,0x10,0x28,0x28,0x24,0x24,0x42,0x42
};

void main() {
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

	// Initial X coordinate starting point for enemy
	int enemyXCoordinate = 100;

	// Initial Y coordinate starting point for enemy
	int enemyYCoordinate = 100;

	// Boolean to determine whether the enemy is moving up or down
	int enemyMovingUp = 1;

	// Keep track of the joypad button pressed
	int key;

	// Load the three parts of the man into the sprite data
	set_sprite_data(0, 3, man);

	// Set the first sprite tile to be the head
	set_sprite_tile(0, 0);

	// Set the second sprite tile to be the first style of body
	set_sprite_tile(1, 1);

	// Set the third sprite tile to be the head for use as an enemy
	set_sprite_tile(2, 0);

	// Move the sprite on to the screen so we can see it
	move_sprite(0, 50, 32);
	move_sprite(1, 50, 40);
	move_sprite(2, 100, 100);

	SHOW_SPRITES;

	while (!0) {
		key = joypad();

		// '&' is used in conjunction with if statments in case
		// Both right and up (or another combination) is being
		// pushed so that the sprite can move diagonally
		if (key & J_RIGHT) {
			// Set the body of the sprite to the first style
			set_sprite_tile(1, 1);

			playerXCoordinate++;

			// Check to see if the X coordinate is greater than the upper boundary
			// If so, then do not let the sprite move beyond it
			if (playerXCoordinate > xCoordinateUpperBoundary) {
				playerXCoordinate = xCoordinateUpperBoundary;
			}

			// Move both the head and body of the sprite to the new X location
			move_sprite(0, playerXCoordinate, playerYCoordinate);
			move_sprite(1, playerXCoordinate, playerYCoordinate + 8);
		}

		if (key & J_LEFT) {
			set_sprite_tile(1, 2);

			playerXCoordinate--;

			if (playerXCoordinate < xCoordinateLowerBoundary) {
				playerXCoordinate = xCoordinateLowerBoundary;
			}

			move_sprite(0, playerXCoordinate, playerYCoordinate);
			move_sprite(1, playerXCoordinate, playerYCoordinate + 8);
		}

		if (key & J_UP) {
			playerYCoordinate--;

			if (playerYCoordinate < yCoordinateLowerBoundary) {
				playerYCoordinate = yCoordinateLowerBoundary;
			}

			move_sprite(0, playerXCoordinate, playerYCoordinate);
			move_sprite(1, playerXCoordinate, playerYCoordinate + 8);
		}

		if (key & J_DOWN) {
			playerYCoordinate++;

			if (playerYCoordinate > yCoordinateUpperBoundary) {
				playerYCoordinate = yCoordinateUpperBoundary;
			}

			move_sprite(0, playerXCoordinate, playerYCoordinate);
			move_sprite(1, playerXCoordinate, playerYCoordinate + 8);
		}

		// Move the enemy sprite head in a square pattern
		if (enemyMovingUp) {
			enemyYCoordinate--;

			if (enemyYCoordinate <= 50) {
				enemyMovingUp = 0;
			}
		}
		else {
			enemyYCoordinate++;

			if (enemyYCoordinate >= 125) {
				enemyMovingUp = 1;
			}
		}

		move_sprite(2, enemyXCoordinate, enemyYCoordinate);

		delay(15);
	}
}
