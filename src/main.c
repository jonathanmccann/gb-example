#include <gb/gb.h>

#include <stdio.h>

#include "../include/global.h"
#include "../include/player.h"
#include "../include/shot_and_enemy.h"
#include "../include/sprite_and_background.h"

UBYTE scrollBackgroundCounter;

void main();

void initializeGame() {
	// Initialize the background and sprites before
	// trying to access and move them in 'player.c'
	// and 'shot.c'
	initializeBackground();
	initializeSprites();

	// Call the initialize enemy and player functions
	initializeEnemies();
	initializePlayer();
	initializePlayerShots();

	// Show the background and sprites that were previously
	// initialized
	initializeDisplay();

	scrollBackgroundCounter = 0;
}

void gameLoop() {
	initializeGame();

	while (1) {
		// Trigger the background to be updated
		if (scrollBackgroundCounter) {
			scrollBackground();

			scrollBackgroundCounter = 0;
		}
		else {
			scrollBackgroundCounter = 1;
		}

		// Pass the input from the joypad to player.c to update
		// the player and shot's location
		updatePlayerAndShots(joypad());

		// Move the enemy sprites as they are defined in enemy.c
		moveEnemySprites();

		// After all movement has completed, test to see if any of the shots
		// collide with any of the enemies or if any of the enemies collide with
		// the player
		testEnemyCollision();

		// Test to see if the player has collided with any of the solid
		// background sprites
		testBackgroundCollision();

		if (playerHitCounter == 3) {
			break;
		}

		// Use this to force the game to run at 60
		// frames per second and allow the background
		// to scroll more smoothly
		wait_vbl_done();
	}
}

void gameOver() {
	clearScreen();

	// This print statement display incorrectly currently. It will be fixed with
	// a splash 'Game Over' image in the future.
	printf("Game Over");

	waitpad(J_START);

	// Although it calls the 'main' function, the function 'displayStartScreen'
	// does not wait for another start button press, most likely due to how
	// quickly the Gameboy scans for input.
	main();
}

void displayStartScreen() {
	printf("Press start to play");

	waitpad(J_START);
}

void main() {
	displayStartScreen();

	gameLoop();

	gameOver();
}
