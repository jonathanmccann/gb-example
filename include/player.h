#include "enemy.h"

#ifndef PROJECT_PLAYER_H
#define PROJECT_PLAYER_H

void initializePlayer();
void initializeShots();
void updatePlayerAndShots(int key);
void testBackgroundCollision();
void testShotAndEnemyCollision(Enemy* enemy);

#endif
