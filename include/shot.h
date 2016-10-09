#include "player.h"

#ifndef PROJECT_SHOT_H
#define PROJECT_SHOT_H

void moveShots();
void movePlayerShotsToPlayer(Player *player);
void removePlayerLowerShot();
void removePlayerUpperShot();
void updateRotatingShotDirection();
void updateRotatingShotsVelocity();

#endif
