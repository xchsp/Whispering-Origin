#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "config.h"
#include <string>
#include <vector>
#include <map>
using namespace std;



const vector<grid_tile_type> grid_tile_types {
  COLLIDER, TREE, ORE, FISH
};

const TILE_DATA grid_tiles {
  { TREE_OAK, TREE },
  { ORE_COPPER, ORE },
  { FISH_SALMON, FISH }
};

const TILE_NAMES grid_tile_names {
  { TREE_OAK, "Oak Tree" },
  { ORE_COPPER, "Copper Ore" },
  { FISH_SALMON, "Salmon" }
};




// General
const int window_width = 1280;
const int window_height = 720;

const char* game_name = "Whispering Origin";
const char* icon_path = "images/icon.png";

bool debug_mode = true;

const int debug_info_x_start = 25;
const int debug_info_y_start = 105;
const int debug_info_y_padding = 2;

vector<int> xp_rates;


// UI
const string base_ui_image_path = "images/UI.png";
const string active_slot_image_path = "images/active_slot.png";


// Fonts
const bool anti_aliased_font = true;
const char* main_font_path = "fonts/Ubuntu_C.ttf";
map<string, TTF_Font*> fonts;


// Colors
const SDL_Color color_game_background = { 21, 108, 153, 1 };
const SDL_Color color_white = { 255, 255, 255, 1 };


// World
const string world_data_path = "maps/main.map";
const string world_texture_path = "images/maps/main_texture.png";
const int world_velocity = 250;
int grid_size = 32;


// Entity images
const string character_image_path = "images/char.png";
const int character_grid_size = 64;

const string tree_image_path = "images/trees.png";
const int tree_grid_size = 160;

const string ore_image_path = "images/ores.png";
const int ore_grid_size = 32;

const string monster_image_path = "images/beetle5.PNG";
// const int monster_grid_size = ?;


// Player
const int max_level = 20;
const uint32_t max_money = 2000000;

const int time_to_display_level_up_message = 3500;
