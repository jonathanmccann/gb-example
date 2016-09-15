#include "enemy.h"

#ifndef PROJECT_PLAYER_H
#define PROJECT_PLAYER_H

typedef struct {
	UBYTE xCoordinate;
	UBYTE yCoordinate;
	int leftSpriteNumber;
	int rightSpriteNumber;
} Player;

void initializePlayer();
void initializeShots();
void updatePlayerAndShots(int key);
void testBackgroundCollision();
void testPlayerAndEnemyCollision(Enemy* enemy);
void testShotAndEnemyCollision(Enemy* enemy);

#endif
