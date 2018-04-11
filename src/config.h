/*
 * The ultimate config file for main.cc
 * char*  => used for options passed directly to SDL functions
 * string => used for options passed to class constructors
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
using namespace std;


// General
extern const int window_width; // define the window's width created by SDL
extern const int window_height; // define the window's height created by SDL

extern const char* game_name; // define the title of the game (will display in the window's title bar)
extern const char* icon_path; // define where the icon image is stored (icon will show in the taskbar) [PNG]

extern bool debug_mode; // define if debug mode should be on by default


// Fonts
extern const char* main_font_path; // define where the main font is stored [TTF]

extern TTF_Font* main_font_16;
extern TTF_Font* main_font_20;
extern TTF_Font* main_font_32;


// Colors
extern const SDL_Color color_game_background;
extern const SDL_Color color_white;


// World
extern const string world_data_path; // define where the main world's data is stored [MAP]
extern const string world_texture_path; // define where the main world's texture image is stored [PNG]
extern const int world_velocity; // defines how fast the world moves around


// Entity images
extern const string character_image_path; // define where the player's character image is stored [PNG]
extern const int character_grid_size; // define how big one character image is (used for sprites)

extern const string tree_image_path; // define where the TEMP tree image is stored [PNG]
extern const int tree_grid_size; // define how big one tree image is (used for sprites)

extern const string ore_image_path;
extern const int ore_grid_size;

extern const string monster_image_path; // define where TEMP monster image is stored [PNG]
extern const int monster_grid_size;


// Player
extern const int max_level; // define how far the player can level
extern const uint32_t max_money; // define how much money the player can have

extern const int time_to_display_level_up_message; // define how long the level up message should be shown (in milliseconds)



#endif
