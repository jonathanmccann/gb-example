#include <gb/gb.h>

#include "../include/global.h"
#include "../include/player.h"
#include "../include/shot.h"
#include "../include/sprite_and_background.h"

#define playerXCoordinateLowerBoundary 8
#define playerXCoordinateUpperBoundary 152
#define playerYCoordinateLowerBoundary 16
#define playerYCoordinateUpperBoundary 150

// Set the offset of width between the left and right halves of the ship
int offset = 8;

// Set the off screen coordinates to hide the sprite
int offScreen = 0;

// Create an iterator for use across all functions
UBYTE i, keyADown;

// Create a timer to keep track of when the ship can shoot next
int timeToShoot = 0;

// Store how far we have scrolled on the X axis
UBYTE scrollX;

UBYTE hitVerticalLine = 0;

Player player;

void initializePlayer() {
	// Move the sprite on to the screen so we can see it
	player.xCoordinate = 50;
	player.yCoordinate = 125;
	player.leftSpriteNumber = left_half_ship;
	player.rightSpriteNumber = right_half_ship;

	move_sprite(player.leftSpriteNumber, player.xCoordinate, player.yCoordinate);
	move_sprite(player.rightSpriteNumber, player.xCoordinate + offset, player.yCoordinate);

	scrollX = 0;
}

void movePlayer() {
	// Move both the left and right halves of the ship to its new location
	// as set in 'updatePlayerAndShot'
	move_sprite(left_half_ship, player.xCoordinate, player.yCoordinate);
	move_sprite(right_half_ship, player.xCoordinate + offset, player.yCoordinate);
}

void testBackgroundCollision() {
	UBYTE tile;

	// Get the tile directly to the right of the ship. The offsets will need to
	// be tweaked to get the correct collision as well as checking more parts
	// of the ship.
	get_bkg_tiles((player.xCoordinate + 7 + scrollX) / 8, (player.yCoordinate - 8) / 8, 1, 1, &tile);

	// If the tile is 1 that means it has hit tile 1 from 'tiles.c'. In this
	// case, it is the vertical line tile. For testing purposes, simply flip
	// the ships sprites for feedback on whether or not the collision detection
	// is working properly.
	if (tile == 1) {
		if (hitVerticalLine == 0) {
			set_sprite_prop(left_half_ship, S_FLIPX);
			set_sprite_prop(right_half_ship, S_FLIPX);

			hitVerticalLine = 1;
		}
		else {
			set_sprite_prop(left_half_ship, 0);
			set_sprite_prop(right_half_ship, 0);

			hitVerticalLine = 0;
		}
	}

	// Keep track of where we are in terms of the background scrolling. Since
	// the map has a width of 32 and 32 * 8 = 256, a UBYTE works perfectly as it
	// will overflow back to 0 once the upper limit is reached. If the map
	// becomes wider, 'scrollX' will need to be able to handle a larger digit.
	scrollX += backgroundXScrollRate;
}

void testPlayerAndEnemyCollision(Enemy* enemy) {
	if (player.yCoordinate > enemy->yCoordinate - 8) {
		if (player.yCoordinate < enemy->yCoordinate + 8) {
			if (player.xCoordinate > enemy->xCoordinate - 16) {
				if (player.xCoordinate < enemy->xCoordinate + 8) {
					enemy->xCoordinate = offScreen;
					enemy->yCoordinate = offScreen;

					// Move the enemy sprite off screen
					move_sprite(enemy->spriteNumber, offScreen, offScreen);

					// Designate that the player has died to break out of the
					// main loop in main.c
					isPlayerDead = 1;
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
		if (player.xCoordinate > playerXCoordinateUpperBoundary) {
			player.xCoordinate = playerXCoordinateUpperBoundary;
		}

		movePlayer();
	}

	if (key & J_LEFT) {
		player.xCoordinate--;

		if (player.xCoordinate < playerXCoordinateLowerBoundary) {
			player.xCoordinate = playerXCoordinateLowerBoundary;
		}

		movePlayer();
	}

	if (key & J_UP) {
		player.yCoordinate--;

		if (player.yCoordinate < playerYCoordinateLowerBoundary) {
			player.yCoordinate = playerYCoordinateLowerBoundary;
		}

		movePlayer();
	}

	if (key & J_DOWN) {
		player.yCoordinate++;

		if (player.yCoordinate > playerYCoordinateUpperBoundary) {
			player.yCoordinate = playerYCoordinateUpperBoundary;
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
			moveShotsToPlayer(&player);

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
