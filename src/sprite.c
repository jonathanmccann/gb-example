#include <gb.h>

#include <tiles.c>
#include <background_map_tiles.c>
#include <ship.c>

#define numberOfEnemies 2

// Create an iterator for loops
int i;

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

	// Set the sprite number associated with the enemy
	int spriteNumber;
} Enemy;

Enemy firstEnemy = {
	100,  // Starting X coordinate
	100,  // Staring Y coordinate
	TRUE, // Is the enemy moving up
	50,   // Y coordinate lower boundary
	125,  // Y coordinate upper boundary
	2     // Sprite number of the enemy
};

Enemy secondEnemy = {
	125,   // Starting X coordinate
	20,    // Staring Y coordinate
	FALSE, // Is the enemy moving up
	20,    // Y coordinate lower boundary
	125,   // Y coordinate upper boundary
	3      // Sprite number of the enemy
};

Enemy enemy[numberOfEnemies];

#include <player.c>

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

void insert(Enemy *enemy, Enemy *enemyToInsert) {
	enemy->xCoordinate = enemyToInsert->xCoordinate;
	enemy->yCoordinate = enemyToInsert->yCoordinate;
	enemy->isMovingUp = enemyToInsert->isMovingUp;
	enemy->yCoordinateLowerBoundary = enemyToInsert->yCoordinateLowerBoundary;
	enemy->yCoordinateUpperBoundary = enemyToInsert->yCoordinateUpperBoundary;
	enemy->spriteNumber = enemyToInsert->spriteNumber;
}

void initializeEnemies() {
	// Pass references to the enemy array and the enemy itself
	// to work around a limitation in SDCC where structs cannot
	// be assigned directly to an array
	insert(&enemy[0], &firstEnemy);
	insert(&enemy[1], &secondEnemy);
}

void moveEnemySprite(Enemy *enemy) {
	// Move the enemy sprite head in a straight line pattern
	if (enemy->isMovingUp) {
		enemy->yCoordinate--;

		if (enemy->yCoordinate <= enemy->yCoordinateLowerBoundary) {
			enemy->isMovingUp = 0;
		}
	}
	else {
		enemy->yCoordinate++;

		if (enemy->yCoordinate >= enemy->yCoordinateUpperBoundary) {
			enemy->isMovingUp = 1;
		}
	}

	move_sprite(enemy->spriteNumber, enemy->xCoordinate, enemy->yCoordinate);
}

void main() {
	// Set the scroll rate of the background in pixels
	int backgroundXScrollRate = 1;
	int backgroundYScrollRate = 1;

	// Call the initialize enemy function
	initializeEnemies();

	// Load the two parts of the ship into the sprite data
	set_sprite_data(0, 2, ship);

	// Load the three parts of the man into the sprite data
	// Starting at '2' due to the two sprites already added
	// by the 'ship' data
	set_sprite_data(2, 3, man);

	// Set the first sprite tile to be the left half of the ship
	set_sprite_tile(0, 0);

	// Set the second sprite tile to be the right half of the ship
	set_sprite_tile(1, 1);

	// Set the third sprite tile to be the head for use as the first enemy
	set_sprite_tile(2, 2);

	// Set the fourth sprite tile to be the head for use as the second enemy
	set_sprite_tile(3, 2);

	// Set the fifth sprite tile to be the head for use as a player's shot
	set_sprite_tile(4, 2);

	initializePlayer();

	// Move the sprite on to the screen so we can see it
	move_sprite(2, 100, 100);
	move_sprite(3, 20, 125);

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

		updatePlayerAndShot(joypad());

		for (i = 0; i < numberOfEnemies; i++) {
			moveEnemySprite(&enemy[i]);
		}

		for (i = 0; i < numberOfEnemies; i++) {
			testShotAndEnemyCollision(&enemy[i]);
		}

		// Use this to force the game to run at 60
		// frames per second and allow the background
		// to scroll more smoothly
		wait_vbl_done();
	}
}
