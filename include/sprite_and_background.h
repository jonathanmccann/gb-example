#ifndef PROJECT_SPRITE_AND_BACKGROUND_H
#define PROJECT_SPRITE_AND_BACKGROUND_H

// Define how quickly and in which direct the background scrolls
#define backgroundXScrollRate 1
#define backgroundYScrollRate 0

// Define the sprite numbers for use in other files
#define left_half_ship 0
#define right_half_ship 1
#define first_enemy 2
#define second_enemy 3
#define first_straight_shot 4
#define first_upper_shot 5
#define first_lower_shot 6
#define second_straight_shot 7
#define second_upper_shot 8
#define second_lower_shot 9
#define third_straight_shot 10
#define third_upper_shot 11
#define third_lower_shot 12

// Define the starting index of the shot sprites
// for use inside of the loop in 'player.c'
#define shot_sprite_starting_position 4

// Create external references to functions found in
// 'sprite_and_background.c'
void initializeBackground();
void initializeDisplay();
void initializeSprites();
void scrollBackground();

#endif //PROJECT_SPRITE_AND_BACKGROUND_H
