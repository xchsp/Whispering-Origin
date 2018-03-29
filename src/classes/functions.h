#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>

#include "window.h"
#include "images.h"

using namespace std;


extern int constrain(int, int, int);

extern int constrain_png_width(int, Window&, PNG&);
extern int constrain_png_height(int, Window&, PNG&);


extern string format_number(int);
extern string to_fixed(float);


#endif