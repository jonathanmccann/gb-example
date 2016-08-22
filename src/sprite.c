#include <gb.h>

#include <tiles.c>
#include <background_map_tiles.c>
#include <ship.c>

#define numberOfEnemies 2

typedef struct _Enemy {
	// Initial X coordinate starting point for enemy
	int xCoordinate;

	// Initial Y coordinate starting point for enemy
	int yCoordinate;

	// Boolean to determine whether the enemy is moving up or down
	BOOLEAN isMovingUp;

	// Set the Y coordinate upper and lower bounds for the enemy sprite
	int yCoordinateLowerBoundary;
	int yCoordinateUpperBoundary;
} Enemy;

Enemy enemy[numberOfEnemies];

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

void initializeEnemy() {
	int i;

	for (i = 0; i < numberOfEnemies; i++) {
		// Initial X coordinate starting point for enemy
		enemy[i].xCoordinate = 100;

		// Initial Y coordinate starting point for enemy
		enemy[i].yCoordinate = 100;

		// Boolean to determine whether the enemy is moving up or down
		enemy[i].isMovingUp = TRUE;

		// Set the Y coordinate upper and lower bounds for the enemy sprite
		enemy[i].yCoordinateLowerBoundary = 50;
		enemy[i].yCoordinateUpperBoundary = 125;
	}
}

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

	// Boolean to determine whether or not the player can shoot
	int playerCanShoot = 1;

	// Initial X and Y coordinate starting points for the shot
	int shotXCoordinate = 0;
	int shotYCoordinate = 0;

	// Set the off screen coordinates to hide the sprite
	int offScreen = 200;

	// Keep track of the joypad button pressed
	int key;

	// Set the scroll rate of the background in pixels
	int backgroundXScrollRate = 1;
	int backgroundYScrollRate = 1;

	// Call the initialize enemy function
	initializeEnemy();

	// Load the two parts of the ship into the sprite data
	set_sprite_data(0, 2, ship);

	// Load the three parts of the man into the sprite data
	//set_sprite_date(3, 3, man);

	// Set the first sprite tile to be the left half of the ship
	set_sprite_tile(0, 0);

	// Set the second sprite tile to be the right half of the ship
	set_sprite_tile(1, 1);

	// Set the third sprite tile to be the head for use as an enemy
	set_sprite_tile(2, 3);

	// Set the fourth sprite tile to be the head for use as a player's shot
	set_sprite_tile(3, 3);

	// Move the sprite on to the screen so we can see it
	move_sprite(0, 50, 32);
	move_sprite(1, 58, 32);
	move_sprite(2, 100, 100);

	// Set the background data
	// Since there are two tiles
	set_bkg_data(0, 2, tiles);

	// Set the background tiles. These tiles in background_map_tiles
	// refer to 8x8 tiles created in tiles.c / tiles
	set_bkg_tiles(0, 0, 20, 18, background_map_tiles);

	SHOW_BKG;
	SHOW_SPRITES;

	while (!0) {
		// Scroll the background as defined by the scroll rate
		scroll_bkg(backgroundXScrollRate, backgroundYScrollRate);

		key = joypad();

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

			move_sprite(3, shotXCoordinate, shotYCoordinate);
		}

		// Move the enemy sprite head in a straight line pattern
		if (enemy[0].isMovingUp) {
			enemy[0].yCoordinate--;

			if (enemy[0].yCoordinate <= enemy[0].yCoordinateLowerBoundary) {
				enemy[0].isMovingUp = 0;
			}
		}
		else {
			enemy[0].yCoordinate++;

			if (enemy[0].yCoordinate >= enemy[0].yCoordinateUpperBoundary) {
				enemy[0].isMovingUp = 1;
			}
		}

		move_sprite(2, enemy[0].xCoordinate, enemy[0].yCoordinate);

		if (!playerCanShoot) {
			shotXCoordinate += 2;

			if (shotXCoordinate >= xCoordinateUpperBoundary) {
				move_sprite(3, offScreen, offScreen);

				playerCanShoot = 1;
			}
			else {
				move_sprite(3, shotXCoordinate, shotYCoordinate);
			}
		}

		// Collision detection between the player's shot and the enemy
		if (shotYCoordinate > enemy[0].yCoordinate - 8) {
			if (shotYCoordinate < enemy[0].yCoordinate + 8) {
				if (shotXCoordinate > enemy[0].xCoordinate - 8) {
					if (shotXCoordinate < enemy[0].xCoordinate + 8) {
						playerCanShoot = 1;

						enemy[0].xCoordinate = offScreen;
						enemy[0].yCoordinate = offScreen;

						move_sprite(2, offScreen, offScreen);
						move_sprite(3, offScreen, offScreen);
					}
				}
			}
		}

		delay(15);

		// Use this to force the game to run at 60
		// frames per second and allow the background
		// to scroll more smoothly
		wait_vbl_done();
	}
}
