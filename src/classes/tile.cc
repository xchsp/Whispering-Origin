#include <sstream>
#include "tile.h"
#include "functions.h"

using namespace std;


Tile::Tile(Game& game, const grid_tile_data& id, int row, int col) {
  _game = &game;
  _tile_grid_size = grid_size;

  _id = id;
  if(_id != -1) {
    _type = get_tile_type(_id);
    _tile_name = get_tile_name(_id);
  } else {
    _type = COLLIDER;
    _tile_name = "Collider";
  }


  _row = row;
  _col = col;
  _row_and_col = combine_row_and_col(_row, _col);



  string tile_image_path = "";

  if(_type == TREE) {
    _tile_grid_size = tree_grid_size;
    tile_image_path = tree_image_path;

    _resources = 5;
    _resources_left = _resources;
  } else if(_type == ORE) {
    _tile_grid_size = ore_grid_size;
    tile_image_path = ore_image_path;

    _resources = 3;
    _resources_left = _resources;
  }
  //else if(_type == FISH) _tile_grid_size = fish_grid_size;


  _progress_text = new Text(*_game, fonts["main_18"], color_white, (format_number(_resources_left) + "/" + format_number(_resources)), 0, 0);
  _progress_text->align_center_y();

  SDL_Color bar_color = { 21, 108, 153, 255 };
  _progress_bar = new Progress(*_game, bar_color, 68, 16, 0, 0);
  _progress_bar->progress((int) _resource_percentage);


  _x_start = (_col * grid_size);
  _x_end = (_x_start + grid_size);
  _y_start = (_row * grid_size);
  _y_end = (_y_start + grid_size);


  if(tile_image_path != "") {
    _original_x = _x_start - (_tile_grid_size / 2) + (grid_size / 2);
    _original_y = _y_start - (_tile_grid_size - grid_size);

    _tile_image = new Image(*_game, tile_image_path, _original_x, _original_y);

    _tile_image->container_width(_tile_grid_size);
    _tile_image->container_height(_tile_grid_size);
    _tile_image->image_width(_tile_grid_size);
    _tile_image->image_height(_tile_grid_size);

    // 0 value is temporary
    _tile_image->image_x(0);
    _tile_image->image_y(0);
  } else {
    _tile_image = NULL;
  }
}



int Tile::width() {
  if(_tile_image != NULL) {
    return _tile_image->width();
  } else {
    return 0;
  }
}
int Tile::height() {
  if(_tile_image != NULL) {
    return _tile_image->height();
  } else {
    return 0;
  }
}



int Tile::x_start() { return _x_start; }
int Tile::x_end() { return _x_end; }

int Tile::y_start() { return _y_start; }
int Tile::y_end() { return _y_end; }



int Tile::original_x() { return _original_x; }
int Tile::original_y() { return _original_y; }


int Tile::x() {
  if(_tile_image != NULL) {
    return _tile_image->x();
  } else {
    return 0;
  }
}
int Tile::y() {
  if(_tile_image != NULL) {
    return _tile_image->y();
  } else {
    return 0;
  }
}

void Tile::x(int new_x) {
  if(_tile_image != NULL) {
    _tile_image->x(new_x);
  }
}
void Tile::y(int new_y) {
  if(_tile_image != NULL) {
    _tile_image->y(new_y);
  }
}


int Tile::row() { return _row; }
int Tile::col() { return _col; }

void Tile::row(int new_row) {
  _row = new_row;
  _row_and_col = combine_row_and_col(_row, _col);

  _y_start = (_row * grid_size);
  _y_end = (_y_start + grid_size);
}

void Tile::col(int new_col) {
  _col = new_col;
  _row_and_col = combine_row_and_col(_row, _col);

  _x_start = (_col * grid_size);
  _x_end = (_x_start + grid_size);
}

string Tile::row_and_col() { return _row_and_col; }


grid_tile_data Tile::id() { return _id; }
grid_tile_type Tile::type() { return _type; }
string Tile::tile_name() { return _tile_name; }


void Tile::action() {
  if((_game->fps()->ticks() - _last_action_at) >= 1000) {
    if(_resources_left > 0) {
      _last_action_at = _game->fps()->ticks();
      _resource_percentage = constrain((_resource_percentage - (100.0 / 3.0)), 0, 100);

      if(_resource_percentage <= 0) {
        _resource_percentage = 100.0;
        _resources_left--;
        _progress_text->update((format_number(_resources_left) + "/" + format_number(_resources)));

        if(_type == TREE) _game->player()->skill("Woodcutting")->increase_xp(5);
        else if(_type == ORE) _game->player()->skill("Mining")->increase_xp(5);
        //else if(_type == FISH) _game->player()->skill("Fishing")->increase_xp(5);

        _game->player()->add_item(grid_tile_drops.at(_id));

        if(_resources_left == 0) {
          if(_depleted_at == 0) {
            _depleted_at = _game->fps()->ticks();
            _tile_image->image_x(_tile_grid_size);
          }
        }
      }

      _progress_bar->progress((int) _resource_percentage);

    }
  }
}

bool Tile::action_on_cooldown() {
  if((_game->fps()->ticks() - _last_action_at) >= 1000) {
    return false;
  } else {
    return true;
  }
}


void Tile::show_progress() {
  _progress_bar->x(_tile_image->x() + ((_tile_image->width() / 2) - (_progress_bar->width() / 2)));
  _progress_text->x(_progress_bar->x() + ((_progress_bar->width() / 2) - (_progress_text->width() / 2)));

  if(_type == TREE) {
    if(_depleted_at > 0) {
      _progress_bar->y(_tile_image->y() + (_tile_image->height() - 60));
    } else {
      _progress_bar->y(_tile_image->y());
    }

    _progress_text->y(_progress_bar->y() - 25);
  } else if(_type == ORE) {
    _progress_bar->y(_tile_image->y() - 30);
    _progress_text->y(_progress_bar->y() - 25);
  }

  _progress_text->render();
  _progress_bar->render();
}


void Tile::update() {
  if(_depleted_at > 0) {
    if(_game->fps()->ticks() - _depleted_at >= 5000) {
      _depleted_at = 0;
      _tile_image->image_x(0);
      _resources_left = _resources;
      _progress_text->update((format_number(_resources_left) + "/" + format_number(_resources)));
      _progress_bar->progress((int) _resource_percentage);
    } else {
      _progress_text->update(format_number(((5000 - (_game->fps()->ticks() - _depleted_at)) / 1000) + 1) + "s");
      _progress_bar->progress(0);
    }
  }
}

void Tile::render() {
  if(_tile_image != NULL) {
    _tile_image->render();
  }
}