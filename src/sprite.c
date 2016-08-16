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
	int x = 50;

	// Set the X coordinate boundaries so the sprite does not go off the screen
	int xCoordinateLowerBoundary = 8;
	int xCoordinateUpperBoundary = 160;

	// Initial Y coordinate starting point
	int y = 32;

	// Set the Y coordinate boundaries so the sprite does not go off the screen
	int yCoordinateLowerBoundary = 16;
	int yCoordinateUpperBoundary = 140;

	// Set the offset of height between the head and the body of the sprite
	int offset = 8;

	// Keep track of the joypad button pressed
	int key;

	// Load the three parts of the man into the sprite data
	set_sprite_data(0, 3, man);

	// Set the first sprite tile to be the head
	set_sprite_tile(0, 0);

	// Set the second sprite tile to be the first style of body
	set_sprite_tile(1, 1);

	// Move the sprite on to the screen so we can see it
	move_sprite(0, 50, 32);
	move_sprite(1, 50, 40);

	SHOW_SPRITES;

	while (!0) {
		key = joypad();

		// '&' is used in conjunction with if statments in case
		// Both right and up (or another combination) is being
		// pushed so that the sprite can move diagonally
		if (key & J_RIGHT) {
			// Set the body of the sprite to the first style
			set_sprite_tile(1, 1);

			x++;

			// Check to see if the X coordinate is greater than the upper boundary
			// If so, then do not let the sprite move beyond it
			if (x > xCoordinateUpperBoundary) {
				x = xCoordinateUpperBoundary;
			}

			// Move both the head and body of the sprite to the new X location
			move_sprite(0, x, y);
			move_sprite(1, x, y + 8);
		}

		if (key & J_LEFT) {
			set_sprite_tile(1, 2);

			x--;

			if (x < xCoordinateLowerBoundary) {
				x = xCoordinateLowerBoundary;
			}

			move_sprite(0, x, y);
			move_sprite(1, x, y + 8);
		}

		if (key & J_UP) {
			y--;

			if (y < yCoordinateLowerBoundary) {
				y = yCoordinateLowerBoundary;
			}

			move_sprite(0, x, y);
			move_sprite(1, x, y + 8);
		}

		if (key & J_DOWN) {
			y++;

			if (y > yCoordinateUpperBoundary) {
				y = yCoordinateUpperBoundary;
			}

			move_sprite(0, x, y);
			move_sprite(1, x, y + 8);
		}

		delay(15);
	}
}
