#ifndef PROJECT_ENEMY_H
#define PROJECT_ENEMY_H

typedef struct _Enemy {
	// Initial X coordinate starting point for enemy
	UBYTE xCoordinate;

	// Initial Y coordinate starting point for enemy
	UBYTE yCoordinate;

	// Set which style of movement the enemy will have
	UBYTE movementPathType;

	// Count how many pixels the enemy has used for use with 'changeInMovement'
	UBYTE movementCounter;

	// Set when the enemy will have a change in movement
	UBYTE changeInMovement;

	// Count how many pixels the enemy has moved before registering a shot
	UBYTE shotCounter;

	// Determine whether or not the enemy is on screen
	UBYTE isOnScreen;

	// Set the sprite number associated with the enemy
	int spriteNumber;
} Enemy;

#endif //PROJECT_ENEMY_H
