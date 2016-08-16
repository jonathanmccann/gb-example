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
	int x;

	// Load the three parts of the man into the sprite data
	set_sprite_data(0, 3, man);

	// Set the first sprite tile to be the head
	set_sprite_tile(0, 0);

	// Set the second sprite tile to be the first style of body
	set_sprite_tile(1, 1);

	SHOW_SPRITES;

	while (!0) {
		// Move the sprite head and body to the right from X pixel 50 to 100
		for(x = 50; x < 100; x++) {
			move_sprite(0, x, 32);
			move_sprite(1, x, 40);
			delay(15);
		}

		// Move the sprite head and body to the left from X pixel 100 to 50
		for(x = 100; x > 50; x--) {
			move_sprite(0, x, 32);
			move_sprite(1, x, 40);
			delay(15);
		}
	}
}
