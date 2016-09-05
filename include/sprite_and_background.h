#ifndef PROJECT_SPRITE_AND_BACKGROUND_H
#define PROJECT_SPRITE_AND_BACKGROUND_H

// Define how quickly and in which direct the background scrolls
#define backgroundXScrollRate 1
#define backgroundYScrollRate 1

// Define the sprite numbers for use in other files
#define left_half_ship 0
#define right_half_ship 1
#define first_enemy 2
#define second_enemy 3
#define straight_shot 4
#define upper_shot 5
#define lower_shot 6

// Define the starting index of the shot sprites
// for use inside of the loop in 'player.c'
#define straight_shot_sprite_starting_position 4
#define rotating_shot_sprite_starting_position 5

// Create external references to functions found in
// 'sprite_and_background.c'
void initializeBackground();
void initializeDisplay();
void initializeSprites();
void updateBackground();

#endif //PROJECT_SPRITE_AND_BACKGROUND_H
