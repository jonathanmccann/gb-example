#include <gb/gb.h>

#include "../include/global.h"
#include "../include/sprite_and_background.h"

#include <tiles.c>
#include <background_map_tiles.c>
#include <ship.c>

#define backgroundMapWidthToDraw 21
#define pixelsPerTile 8
#define tilesPerScreen 32

UBYTE columnYCoordinate, columnXCoordinate;

BYTE pixelCounter;

UWORD tileCounter;

BYTE mapCounter;

int updateMapCounter = 0;

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

void clearScreen() {
	UBYTE x, y;

	// Reset the X scroll value so that the correct tiles are cleared
	SCX_REG = 0;

	DISPLAY_OFF;
	HIDE_SPRITES;
	HIDE_BKG;

	// Reset the tiles currently being display to blank
	for(x = 0; x != 20; x++)
	{
		for(y = 0; y != 18; y++)
		{
			set_bkg_tiles(x, y, 1, 1, 0x04);
		}
	}

	SHOW_BKG;
	DISPLAY_ON;
}

void initializeBackground() {
	// Reset the X scroll value so that the screen begins drawing in the correct
	// place
	SCX_REG = 0;

	// Set up all global variables so that when the game restarts, they are
	// initialized correctly
	columnYCoordinate = 0;
	columnXCoordinate = 0;
	pixelCounter = 0;
	tileCounter = 0;
	tileScrollX = 0;
	mapCounter = 0;

	// Set the background data
	// Since there are two tiles
	set_bkg_data(0, tilesSize, tiles);

	/*
	 * Starting in the upper left hand corner, draw a single row of tiles
	 * (22 tiles wide, 1 tile tall). Do this 18 times to fill the height of the
	 * screen. To start at the correct tile in the map, keep a tileCounter and add
	 * the entire width of the map to the tileCounter. This will progress the
	 * tileCounter to the beginning of the next tile row.
	 */
	for (columnYCoordinate = 0; columnYCoordinate != backgroundMapTilesHeight; columnYCoordinate++) {
		set_bkg_tiles(
			0, columnYCoordinate, backgroundMapWidthToDraw, 1,
			&(backgroundMapTiles[mapCounter] + tileCounter));

		tileCounter = tileCounter + backgroundMapTilesWidth;
	}
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

	// Set the fifth sprite tile to be the head for use as the third enemy
	set_sprite_tile(third_enemy, 2);

	// Set the sixth sprite tile to be the head for use as the fourth enemy
	set_sprite_tile(fourth_enemy, 2);

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

	// Set the 20 enemy shot sprites as the head
	set_sprite_tile(15, 2);
	set_sprite_tile(16, 2);
	set_sprite_tile(17, 2);
	set_sprite_tile(18, 2);
	set_sprite_tile(19, 2);
	set_sprite_tile(20, 2);
}

void updateBackground() {
	// Specify the column that needs to be redrawn
	tileCounter = tileScrollX + backgroundMapWidthToDraw;

	// If the tileCounter is at the last tile in the map switch to the next map array
	if (tileCounter == 255) {
		if (updateMapCounter == 1) {
			updateMapCounter = 2;
		}
		else {
			updateMapCounter = 1;
		}
	}

	// Get the starting X coordinate of the column that needs to be redrawn
	// Since the map can only be 32X32, use mod 32 to normalize the tileCounter
	columnXCoordinate = tileCounter % tilesPerScreen;

	// Normalize the tileCounter so that it does not overflow the background map
	tileCounter = tileCounter % backgroundMapTilesWidth;

	/*
	 * Starting at the column specified in 'columnXCoordinate', redraw that
	 * column tile by tile 18 (height of the map) times to redraw the entire
	 * column. Again, use tileCounter to move forward in the background map to
	 * next row of tiles.
	 */
	for (columnYCoordinate = 0; columnYCoordinate != backgroundMapTilesHeight; columnYCoordinate++) {
		set_bkg_tiles(
			columnXCoordinate, columnYCoordinate, 1, 1,
			&(backgroundMapTiles[mapCounter] + tileCounter));

		tileCounter = tileCounter + backgroundMapTilesWidth;
	}

	// Update the counter after the drawing occurs above so that we draw the last column of the current map
	if (updateMapCounter == 1) {
		mapCounter = 1;
	}
	else if (updateMapCounter == 2) {
		mapCounter = 0;
	}
}

void scrollBackground() {
	// Scroll the background as defined by the scroll rate
	scroll_bkg(backgroundXScrollRate, backgroundYScrollRate);

	// Keep track of where we are in terms of the background scrolling. Since
	// the map has a width of 32 and 32 * 8 = 256, a UBYTE works perfectly as it
	// will overflow back to 0 once the upper limit is reached. If the map
	// becomes wider, 'pixelScrollX' will need to be able to handle a larger digit.
	pixelScrollX += backgroundXScrollRate;

	// Every pixel scrolled, increase the pixel tileCounter
	pixelCounter++;

	// If the player has scrolled 8 pixels (1 tile), then we need to update the
	// background tiles
	if (pixelCounter == pixelsPerTile) {
		// Keep track of where we are in relation to how many tiles we have moved
		tileScrollX++;

		// Reset the pixel counter
		pixelCounter = 0;

		updateBackground();
	}
}
