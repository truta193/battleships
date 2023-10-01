#ifndef BATTLESHIPS2_COMMON_H
#define BATTLESHIPS2_COMMON_H
#include "stdbool.h"

enum { VERTICAL, HORIZONTAL };
typedef enum { WHITE, BLACK, RED, GREEN, GRAY } color_t;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define HALF_PI 1.570796f

float norm_fx(float x);
float norm_fy(float y);
float norm_ix(int x);
float norm_iy(int y);


#endif //BATTLESHIPS2_COMMON_H
