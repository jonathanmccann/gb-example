#include <gb/gb.h>

#include <stdio.h>

#include "../include/enemy.h"
#include "../include/player.h"
#include "../include/shot.h"
#include "../include/sprite_and_background.h"

void initializeGame() {
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
}

void gameLoop() {
	initializeGame();

	while (1) {
		// Trigger the background to be updated
		scrollBackground();

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

		// Use this to force the game to run at 60
		// frames per second and allow the background
		// to scroll more smoothly
		wait_vbl_done();
	}
}

void displayStartScreen() {
	printf("Press start to play");

	waitpad(J_START);
}

void main() {
	displayStartScreen();

	gameLoop();
}
