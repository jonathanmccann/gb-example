#include "player.h"

#ifndef PROJECT_SHOT_H
#define PROJECT_SHOT_H

void moveShots();
void moveShotsToPlayer(Player* player);
void removeLowerShot();
void removeUpperShot();
void updateRotatingShotDirection();
void updateRotatingShotsVelocity();

#endif
