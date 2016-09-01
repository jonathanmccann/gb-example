#include <gb/gb.h>

#include <sprite_and_background.c>

// 'enemy.c' must be included before 'player.c'
// since 'player.c' references variables defined in
// 'enemy.c'
#include <enemy.c>
#include <player.c>

void main() {
	// Initialize the background and sprites before
	// trying to access and move them in 'player.c'
	// and 'enemy.c'
	initializeBackground();
	initializeSprites();

	// Call the initialize enemy and player functions
	initializeEnemies();
	initializePlayer();
	initializeShots();

	// Show the background and sprites that were previously
	// initialized
	initializeDisplay();

	while (!0) {
		// Trigger the background to be updated
		updateBackground();

		// Pass the input from the joypad to player.c to update
		// the player and shot's location
		updatePlayerAndShots(joypad());

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
