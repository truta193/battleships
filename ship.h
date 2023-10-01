#ifndef BATTLESHIPS2_SHIP_H
#define BATTLESHIPS2_SHIP_H

#include "common.h"
#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include "grid.h"

typedef struct ship_t {
    int id;
    int grid[10][10];
    // 1 is horizontal 0 is vertical
    int orientation;
    color_t color;
    bool dragged;
    bool is_in_collision;
    struct hitbox {
        struct bl {
            int x;
            int y;
        } bl;
        struct dim {
            int w;
            int h;
        } dim;
    } hitbox;
    // Offset from grid_xy when being dragged
    struct offset {
        int x;
        int y;
    } offset;
    // Position of the static ship as seen in the grid matrix
    struct grid_xy {
        int x;
        int y;
    } grid_xy;
} ship_t;



// Returns ship id (1-6)
int ship_cursor_collides(ship_t *ships, int x, int y);
bool ship_ship_collides(ship_t *shipA, ship_t *shipB);
void ship_init(ship_t *ship, int grid[10][10]);
void ship_move(ship_t *ship, int dx, int dy);
void hitbox_draw(ship_t *ship);
//Slight drift and it bugs out when rotated outside the grid
void ship_rotate(ship_t *ship);
void grid_draw_ships(ship_t *ships);

#endif //BATTLESHIPS2_SHIP_H
