#include <gb.h>

#include <tiles.c>
#include <background_map_tiles.c>
#include <ship.c>

// 'enemy.c' must be included before 'player.c'
// since 'player.c' references variables defined in
// 'enemy.c'
#include <enemy.c>
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

void main() {
	// Set the scroll rate of the background in pixels
	int backgroundXScrollRate = 1;
	int backgroundYScrollRate = 1;

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

	// Call the initialize enemy and player functions
	initializeEnemies();
	initializePlayer();

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

		// Pass the input from the joypad to player.c to update
		// the player and shot's location
		updatePlayerAndShot(joypad());

		// Move the enemy sprites as they are defined in enemy.c
		moveEnemySprites();

		// After all movement has completed, test to see if the shot
		// collides with any of the enemies in player.c
		testShotAndEnemyCollision();

		// Use this to force the game to run at 60
		// frames per second and allow the background
		// to scroll more smoothly
		wait_vbl_done();
	}
}
