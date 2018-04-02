/*
 * ## Required dependencies ##
 *  - SDL2
 *
 * ## Installation ##
 *  Use the bundled install file to install the dependencies
 *  > sudo sh ./_install_dependencies.sh
 *
 *  or do it manually using:
 *  > sudo apt-get install libsdl2-dev
 *
 *
 * ## Tested on ##
 *  - Ubuntu 17.10.1
 */
#include <SDL2/SDL.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
using namespace std;


enum grid_type {
  PLAYER = 20522057,
  COLLIDER = 245245245,
  GRASS = 6517051, // unused
  WATER = 21108153 // unused
};
enum item_type {
  TREE = 7617580,
  ORE = 25119245,
  FISH = 33150243
};

enum tree_id {
  TREE_OAK = 6381181
};
enum ore_id {
  ORE_COPPER = 6381181
};
enum fish_id {
  FISH_SALMON = 6381181
};


bool get_RGB(SDL_Color& color, SDL_Surface* surface, int x, int y) {
  if(surface->format->BitsPerPixel == 32) {
    uint32_t* pixels = (uint32_t*) surface->pixels;
    uint32_t pixel = pixels[(y * surface->w) + x];
    
    SDL_GetRGB(pixel, surface->format, &(color.r), &(color.g), &(color.b));
    return true;
  } else {
    return false;
  }
}

int concat_RGB(SDL_Color& color) {
  stringstream concat;
  
  concat << (unsigned) color.r
         << (unsigned) color.g
         << (unsigned) color.b;
  
  return stoi(concat.str());
}



bool is_item(int type) {
  if(type == TREE) return true;
  else if(type == ORE) return true;
  else if(type == FISH) return true;
  else return false;
}



int to_grid_row(int y, int grid_size) {
  return floor(y / grid_size);
}

int to_grid_col(int x, int grid_size) {
  return floor(x / grid_size);
}


int to_grid_x(int col, int grid_size) {
  return (col * grid_size);
}

int to_grid_y(int row, int grid_size) {
  return (row * grid_size);
}



int main() {
  const string map_to_use = "images/maps/main_data.bmp";
  const string save_to = "maps/main.map";
  ofstream map_file;
  
  int current_x = 0;
  int current_y = 0;
  
  bool parsing = true;
  
  
  SDL_Surface* surface = SDL_LoadBMP(map_to_use.c_str());
  
  if(surface == NULL) {
    cout << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
    SDL_Quit();
  } else {
    cout << "Extracting map data from image..." << endl << endl;
    
    map_file.open(save_to.c_str());
    const int map_width = surface->w;
    const int map_height = surface->h;
    const int grid_size = 32;
    
    
    map_file << map_width << "x" << map_height
             << ":" << grid_size << endl;
    
    
    
    SDL_Color current_color = { 255, 255, 255, 1};
    bool player_pos_found = false;
    int player_pos_x = floor(map_width / 2);
    int player_pos_y = floor(map_height / 2);
    
    grid_type current_grid_type;
    item_type current_item_type;
    
    int coliders_found = 0;
    int trees_found = 0;
    int ores_found = 0;
    int fish_found = 0;
    
    stringstream player;
    player << "P:"; // Player
    
    stringstream NPCs;
    NPCs << "N:"; // NPCs (unused ATM)
    
    stringstream monsters;
    monsters << "M:"; // Monsters (unused ATM)
    
    
    stringstream colliders;
    colliders << "C:"; // Colliders
    
    stringstream trees;
    trees << "T:"; // Trees
    
    stringstream ores;
    ores << "O:"; // Ores
    
    stringstream fish;
    fish << "F:"; // Fish
    
    
    
    while(parsing) {
      if(get_RGB(current_color, surface, current_x, current_y)) {
        current_grid_type = (grid_type) concat_RGB(current_color);
        
        if(current_grid_type == COLLIDER) {
          if(get_RGB(current_color, surface, (current_x + 1), (current_y + 1))) {
            if(is_item(concat_RGB(current_color))) {
              current_item_type = (item_type) concat_RGB(current_color);
              
              if(get_RGB(current_color, surface, (current_x + 2), (current_y + 1))) {
                if(current_item_type == TREE) {
                  if(trees_found > 0) trees << "|";
                  trees << to_grid_row(current_y, grid_size) << "x" << to_grid_col(current_x, grid_size)
                        << ":" << ((tree_id) concat_RGB(current_color));
                  
                  trees_found++;
                } else if(current_item_type == ORE) {
                  if(ores_found > 0) ores << "|";
                  ores << to_grid_row(current_y, grid_size) << "x" << to_grid_col(current_x, grid_size)
                        << ":" << ((ore_id) concat_RGB(current_color));
                  
                  ores_found++;
                } else if(current_item_type == FISH) {
                  if(fish_found > 0) fish << "|";
                  fish << to_grid_row(current_y, grid_size) << "x" << to_grid_col(current_x, grid_size)
                        << ":" << ((fish_id) concat_RGB(current_color));
                  
                  fish_found++;
                }
              }
            } else {
              if(coliders_found > 0) colliders << "|";
              colliders << to_grid_row(current_y, grid_size) << "x" << to_grid_col(current_x, grid_size);
              coliders_found++;
            }
          }
        } else if(current_grid_type == PLAYER) {
          player_pos_x = current_x;
          player_pos_y = current_y;
          player_pos_found = true;
        }
        
        
        current_x += grid_size;
        if(current_x >= map_width) {
          if((current_y + grid_size) < map_height) {
            current_x = 0;
            current_y += grid_size;
          } else {
            parsing = false;
          }
        }
      } else {
        parsing = false;
        cout << "Failed to grab RGB!" << endl;
      }
    }
    
    
    map_file << endl;
    if(player_pos_found) {
      player << to_grid_row(player_pos_y, grid_size) << "x" << to_grid_col(player_pos_x, grid_size);
    } else {
      // add check to see if the default (center) pos is on ground
    }
    
    
    
    player << endl;
    NPCs << endl;
    monsters << endl << endl;
    
    colliders << endl;
    trees << endl;
    ores << endl;
    fish << endl;
    
    
    map_file << player.str();
    map_file << NPCs.str();
    map_file << monsters.str();
    
    map_file << colliders.str();
    map_file << trees.str();
    map_file << ores.str();
    map_file << fish.str();
    
    
    
    cout << "Colliders found: " << coliders_found << endl
         << "Trees found: " << trees_found << endl
         << "Ores found: " << ores_found << endl
         << "Fish found: " << fish_found << endl
         << endl << endl;
    
    cout << ">> Map width: " << map_width << ", Map height: " << map_height << " <<" << endl << endl;
    SDL_FreeSurface(surface);
    map_file.close();
  }
}
