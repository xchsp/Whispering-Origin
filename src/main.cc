/*
 * ## Required dependencies ##
 *  - SDL2
 *  - SDL2 ttf
 *  - SDL2 image
 *
 * ## Installation ##
 *  Use the bundled install file to install the dependencies
 *  > sudo sh ./_install_dependencies.sh
 *
 *  or do it manually using:
 *  > sudo apt-get install libsdl2-dev
 *  > sudo apt-get install libsdl2-ttf-dev
 *  > sudo apt-get install libsdl2-image-dev
 *
 *
 * ## Tested on ##
 *  - Ubuntu 17.10.1
 *  - KDE Neon (Ubuntu with 16.04.1)
 */
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>

#include "config.h"
#include "classes/window.h"
#include "classes/renderer.h"
#include "classes/images.h"
#include "classes/text.h"
#include "classes/fps.h"
#include "classes/character.h"
using namespace std;


int constrain(int number, int min, int max) {
  if(number < min) number = min;
  if(number > max) number = max;

  return number;
}


int constrain_png_width(int number, Window& win, PNG& png) {
  return constrain(number, 0, (win.width() - png.width()));
}

int constrain_png_height(int number, Window& win, PNG& png) {
  return constrain(number, 0, (win.height() - png.height()));
}


string format_number(int number_to_format) {
  stringstream formatted_number;

  formatted_number.imbue(locale(""));
  formatted_number << fixed << number_to_format;

  return formatted_number.str();
}

string to_fixed(float number_to_fix) {
  stringstream fixed_number;
  fixed_number << fixed << setprecision(2) << number_to_fix;

  return fixed_number.str();
}






int main() {
  bool debug_mode = true;
  
  const int window_width = 1280;
  const int window_height = 720;
  
  //const string map = "images/maps/main_texture.png";
  
  
  FPS fps;
  uint32_t frames = 0;


  if(SDL_Init(SDL_INIT_VIDEO) != 0) {
    cout << "SDL_Init Error: " << SDL_GetError() << endl;
    return 1;
  }

  if(TTF_Init() != 0) {
    cout << "TTF_Init Error: " << SDL_GetError() << endl;
    return 1;
  }

  int imgFlags = IMG_INIT_PNG;
  if(!(IMG_Init(imgFlags) & imgFlags)) {
    cout << "IMG_Init Error: " << SDL_GetError() << endl;
    return 1;
  }


  SDL_DisplayMode DM;
  SDL_GetCurrentDisplayMode(0, &DM);
  int display_width = DM.w;
  int display_height = DM.h;

  const int vel = 250;


  cout << "Display Size: " << display_width << "x" << display_height << endl;
  cout << "Window Size: " << window_width << "x" << window_height << endl << endl;


  TTF_Font* debug_font = TTF_OpenFont(font, 20);
  TTF_Font* Ubuntu_font = TTF_OpenFont(font, 20);
  SDL_Color background_color = { 240, 240, 240 };
  SDL_Color text_color = { 255, 255, 255 };




  Window win(game_name, ((display_width / 2) - (window_width / 2)), ((display_height / 2) - (window_height / 2)), window_width, window_height);
  Renderer ren(win, background_color);

  SDL_Surface* icon = IMG_Load(icon_location);
  SDL_SetWindowIcon(win.get(), icon);


  PNG demo_1080p_map(win, ren, map, -50, -50);
  demo_1080p_map.set_y((demo_1080p_map.get_y() + (32 * 2)));

  //BMP test_char(win, ren, "images/test_char_2.bmp", -50, -50); // -50 => centered, -100 => bottom aligned / right aligned
  PNG test_char(win, ren, char_location, -50, -50); // -50 => centered, -100 => bottom aligned / right aligned


  vector<PNG*> trees;
  const int tree_count = 5;

  for(int i = 0; i < tree_count; i++) {
    trees.push_back(new PNG(win, ren, tree_location, -50, -50));
  }

  trees[0]->set_x(constrain_png_width(545, win, *trees[0]));
  trees[0]->set_y(constrain_png_height(125, win, *trees[0]));

  trees[1]->set_x(constrain_png_width(929, win, *trees[1]));
  trees[1]->set_y(constrain_png_height(157, win, *trees[1]));

  trees[2]->set_x(constrain_png_width(801, win, *trees[2]));
  trees[2]->set_y(constrain_png_height(189, win, *trees[2]));

  trees[3]->set_x(constrain_png_width(321, win, *trees[3]));
  trees[3]->set_y(constrain_png_height(285, win, *trees[3]));

  trees[4]->set_x(constrain_png_width(865, win, *trees[4]));
  trees[4]->set_y(constrain_png_height(317, win, *trees[4]));

/*
  trees[0]->set_x(constrain_png_width(936, win, *trees[0]));
  trees[0]->set_y(constrain_png_height(25, win, *trees[0]));

  trees[1]->set_x(constrain_png_width(1093, win, *trees[1]));
  trees[1]->set_y(constrain_png_height(36, win, *trees[1]));

  trees[2]->set_x(constrain_png_width(986, win, *trees[2]));
  trees[2]->set_y(constrain_png_height(200, win, *trees[2]));

  trees[3]->set_x(constrain_png_width(1161, win, *trees[3]));
  trees[3]->set_y(constrain_png_height(168, win, *trees[3]));

  trees[4]->set_x(constrain_png_width(1103, win, *trees[4]));
  trees[4]->set_y(constrain_png_height(342, win, *trees[4]));
  */

  // Character test start
  Character test_boi(win, ren);

  Text hello_text(win, ren, Ubuntu_font, text_color, "<text will be updated ;) >", -50, -100); // -50 => centered, -100 => bottom aligned / right aligned
  hello_text.set_y((hello_text.get_y() - 25));

  Text debug_ticks(win, ren, debug_font, text_color, "Ticks: 0", 25, 25);
  Text debug_frame_count(win, ren, debug_font, text_color, "Frames: 0", 25, 50);
  Text debug_fps(win, ren, debug_font, text_color, "FPS: 0", 25, 75);
  Text debug_delta_time(win, ren, debug_font, text_color, "Delta Time: 0", 25, 100);
  // Character-specific debug
  Text debug_health(win, ren, debug_font, text_color, "Health: ", 25, 150);
  Text debug_level(win, ren, debug_font, text_color, "Level: ", 25, 175);
  Text debug_exp(win, ren, debug_font, text_color, "Exp: ", 25, 200);
  Text debug_posx(win, ren, debug_font, text_color, "Pos X: ", 25, 225);
  Text debug_posy(win, ren, debug_font, text_color, "Pos Y: ", 25, 250);









  SDL_Event event;
  bool running = true;

  bool up_is_down = false;
  bool down_is_down = false;
  bool left_is_down = false;
  bool right_is_down = false;



  while(running) {
    while(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) running = false;
      else if(event.type == SDL_KEYDOWN) {
        if(event.key.keysym.sym == SDLK_UP) up_is_down = true;
        if(event.key.keysym.sym == SDLK_w) up_is_down = true;

        if(event.key.keysym.sym == SDLK_DOWN) down_is_down = true;
        if(event.key.keysym.sym == SDLK_s) down_is_down = true;

        if(event.key.keysym.sym == SDLK_LEFT) left_is_down = true;
        if(event.key.keysym.sym == SDLK_a) left_is_down = true;

        if(event.key.keysym.sym == SDLK_RIGHT) right_is_down = true;
        if(event.key.keysym.sym == SDLK_d) right_is_down = true;
        
        if(event.key.keysym.sym == SDLK_F4) debug_mode = !debug_mode;
      } else if(event.type == SDL_KEYUP) {
        if(event.key.keysym.sym == SDLK_UP) up_is_down = false;
        if(event.key.keysym.sym == SDLK_w) up_is_down = false;

        if(event.key.keysym.sym == SDLK_DOWN) down_is_down = false;
        if(event.key.keysym.sym == SDLK_s) down_is_down = false;

        if(event.key.keysym.sym == SDLK_LEFT) left_is_down = false;
        if(event.key.keysym.sym == SDLK_a) left_is_down = false;

        if(event.key.keysym.sym == SDLK_RIGHT) right_is_down = false;
        if(event.key.keysym.sym == SDLK_d) right_is_down = false;
      } else {
        //if(event.type != 1024 && event.type != 512) cout << ">> Event: " << event.type << endl;
      }
    }
    
    fps.update();

    
    float temp_vel = (vel * fps.delta_time());
    //float temp_vel = (vel * 0.015000);
    // down and right seem a bit slower than up and left?
    if(up_is_down) {
      test_char.set_y(constrain((test_char.get_y() - temp_vel), 0, (window_height - test_char.height())));
    }
    if(down_is_down) {
      test_char.set_y(constrain((test_char.get_y() + temp_vel), 0, (window_height - test_char.height())));
    }

    if(left_is_down) {
      test_char.set_x(constrain((test_char.get_x() - temp_vel), 0, (window_width - test_char.width())));
    }
    if(right_is_down) {
      test_char.set_x(constrain((test_char.get_x() + temp_vel), 0, (window_width - test_char.width())));
    }


    ren.clear();
    demo_1080p_map.render();

    hello_text.render();
    test_char.render();

    for(int i = 0; i < tree_count; i++) {
      trees[i]->render();
    }
    
    
    if(fps.ticks() > 1000) {
      hello_text.update("Use arrow keys or WASD to move \"the character\" (up, down, left, right), F4 to toggle debug menu"); // will not mess up alignment ;)
    }
    
    
    
    // BEGIN, DEBUG TEXT UPDATING & RENDERING
    debug_frame_count.update("Frames: " + format_number(frames++));
    debug_ticks.update("Ticks: " + format_number(fps.ticks()));
    
    if((fps.ticks() % 500) < 250) {
      debug_fps.update("FPS: " + to_fixed(fps.get()));
    }
    
    debug_delta_time.update("Delta Time: " + to_string(fps.delta_time()));
    
    // Char-specific text-update
    debug_health.update("Health: " + format_number(test_boi.health()));
    debug_exp.update("Exp: " + format_number(test_boi.char_exp()) + "/" 
            + format_number(test_boi.exp_rate()));
    debug_level.update("Level: " + format_number(test_boi.level()) + "/" 
            + format_number(test_boi.levels()));
    debug_posx.update("Pos X: " + format_number(test_char.get_x()));
    debug_posy.update("Pos Y: " + format_number(test_char.get_y()));
    
    if(debug_mode) {
      debug_frame_count.render();
      debug_ticks.render();
      debug_fps.render();
      debug_delta_time.render();

      debug_health.render();
      debug_exp.render();
      debug_level.render();
      debug_posx.render();
      debug_posy.render();
    }
    // END, DEBUG TEXT UPDATING & RENDERING

    
    ren.update(); // update the screen
    // SDL_Delay(1000); // <= sometimes used for debuggning issues
  }


  TTF_CloseFont(debug_font);
  TTF_CloseFont(Ubuntu_font);

  TTF_Quit();
  SDL_Quit();
  cout << ">> Bye Bye! <<" << endl << endl;
}
