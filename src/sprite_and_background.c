#include <gb/gb.h>

#include "../include/sprite_and_background.h"

#include <tiles.c>
#include <background_map_tiles.c>
#include <ship.c>

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

void initializeBackground() {
	// Set the background data
	// Since there are two tiles
	set_bkg_data(0, tilesSize, tiles);

	// Set the background tiles. These tiles in background_map_tiles
	// refer to 8x8 tiles created in tiles.c / tiles
	set_bkg_tiles(0, 0, backgroundMapTilesWidth, backgroundMapTilesHeight, background_map_tiles);
}

void initializeDisplay() {
	SHOW_BKG;
	SHOW_SPRITES;
}

void initializeSprites() {
	// Load the two parts of the ship into the sprite data
	set_sprite_data(0, shipSize, ship);

	// Load the three parts of the man into the sprite data
	// Starting at '2' due to the two sprites already added
	// by the 'ship' data
	set_sprite_data(2, 3, man);

	// Set the first sprite tile to be the left half of the ship
	set_sprite_tile(left_half_ship, 0);

	// Set the second sprite tile to be the right half of the ship
	set_sprite_tile(right_half_ship, 1);

	// Set the third sprite tile to be the head for use as the first enemy
	set_sprite_tile(first_enemy, 2);

	// Set the fourth sprite tile to be the head for use as the second enemy
	set_sprite_tile(second_enemy, 2);

	// Set the six sprite tiles for the six various shots as the head
	set_sprite_tile(first_straight_shot, 2);
	set_sprite_tile(first_upper_shot, 2);
	set_sprite_tile(first_lower_shot, 2);
	set_sprite_tile(second_straight_shot, 2);
	set_sprite_tile(second_upper_shot, 2);
	set_sprite_tile(second_lower_shot, 2);
	set_sprite_tile(third_straight_shot, 2);
	set_sprite_tile(third_upper_shot, 2);
	set_sprite_tile(third_lower_shot, 2);
}

void updateBackground() {
	// Scroll the background as defined by the scroll rate
	scroll_bkg(backgroundXScrollRate, backgroundYScrollRate);
}
