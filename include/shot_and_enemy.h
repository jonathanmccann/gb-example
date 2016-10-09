#include "player.h"

#ifndef PROJECT_SHOT_H
#define PROJECT_SHOT_H

void initializeEnemies();
void moveEnemySprites();
void moveShots();
void movePlayerShotsToPlayer(Player *player);
void removePlayerLowerShot();
void removePlayerUpperShot();
void testEnemyCollision();
void updateRotatingShotDirection();
void updateRotatingShotsVelocity();

#endif
