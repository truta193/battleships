#ifndef BATTLESHIPS2_GRID_H
#define BATTLESHIPS2_GRID_H

#include "common.h"
#include "GL/gl.h"
#include <math.h>

void grid_draw_grid();
void grid_cell_fill(int a, int b, int px_offset, int py_offset, color_t color);
void grid_cell_center(int a, int b, int *x, int *y);
void grid_cell_bl(int a, int b, int *x, int *y);
void grid_cell_nearest(int x, int y, int *na, int *nb);

#endif //BATTLESHIPS2_GRID_H
