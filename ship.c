#include "ship.h"

int ship_cursor_collides(ship_t *ships, int x, int y) {
    for (int i = 1; i < 6; i++) {
        if (x >= ships[i].hitbox.bl.x && x <= ships[i].hitbox.bl.x + ships[i].hitbox.dim.w &&
            y >= ships[i].hitbox.bl.y && y <= ships[i].hitbox.bl.y + ships[i].hitbox.dim.h) {
            return i;
        };
    };
    return 0;
};

bool ship_ship_collides(ship_t *shipA, ship_t *shipB) {
    int ax = shipA->hitbox.bl.x;
    int ay = shipA->hitbox.bl.y;
    int aw = shipA->hitbox.dim.w;
    int ah = shipA->hitbox.dim.h;
    int bx = shipB->hitbox.bl.x;
    int by = shipB->hitbox.bl.y;
    int bw = shipB->hitbox.dim.w;
    int bh = shipB->hitbox.dim.h;

    if (ax < bx + bw && ax + aw > bx && ay < by + bh && ah + ay > by) {
        return true;
    } else {
        return false;
    }
};

void ship_init(ship_t *ship, int grid[10][10]) {
    memcpy(ship->grid, grid, sizeof(int) * 10 * 10);
    // 20 is greater than the max distance possible (10 * 10 * sqrt(2))
    float min_dist = 20.0f;
    int ship_size = 0;
    int is_vertical = false;
    /*
     * Goes through the provided grid and checks if the ship is vertical or horizontal
     * Will get the indices of the cell that's closer to the top left corner
     */
    int min_i = 0, min_j = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (ship->grid[i][j] != 0) {
                ship_size++;
                //
                if (ship->grid[i][j] == ship->grid[i + 1][j] || ship->grid[i][j] == ship->grid[i - 1][j])
                    is_vertical = true;
                float d = sqrtf((float)(i * i + j * j));
                if (d < min_dist) {
                    min_dist = d;
                    min_j = j;
                    min_i = i;
                };
            };
        };
    };

    //Get grid cell phase coordinates and set up ship info
    int tl_x, tl_y;
    grid_cell_bl(min_i, min_j, &tl_x, &tl_y);
    printf("is vertical %d\n", is_vertical);
    if (is_vertical) {
        ship->hitbox.dim.w = 50;
        ship->hitbox.dim.h = 50 * ship_size;
        ship->hitbox.bl.x = tl_x;
        ship->hitbox.bl.y = tl_y - 50 * ship_size;
        ship->orientation = VERTICAL;
    } else {
        ship->hitbox.dim.w = 50 * ship_size;
        ship->hitbox.dim.h = 50;
        ship->hitbox.bl.x = tl_x;
        ship->hitbox.bl.y = tl_y - 50;
        ship->orientation = HORIZONTAL;
    };

    ship->grid_xy.x = ship->hitbox.bl.x;
    ship->grid_xy.y = ship->hitbox.bl.y;

};

void ship_move(ship_t *ship, int dx, int dy) {
    ship->hitbox.bl.x += dx;
    ship->hitbox.bl.y += dy;
    ship->offset.x = ship->hitbox.bl.x - ship->grid_xy.x;
    ship->offset.y = ship->hitbox.bl.y - ship->grid_xy.y;
};

void hitbox_draw(ship_t *ship) {
    int x = ship->hitbox.bl.x;
    int y = ship->hitbox.bl.y;
    int w = ship->hitbox.dim.w;
    int h = ship->hitbox.dim.h;
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(norm_ix(x), norm_iy(y));
    glVertex2f(norm_ix(x + w), norm_iy(y));
    glEnd();

    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(norm_ix(x), norm_iy(y));
    glVertex2f(norm_ix(x), norm_iy(y + h));
    glEnd();

    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(norm_ix(x + w), norm_iy(y));
    glVertex2f(norm_ix(x + w), norm_iy(y + h));
    glEnd();

    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(norm_ix(x), norm_iy(y + h));
    glVertex2f(norm_ix(x + w), norm_iy(y + h));
    glEnd();
};

//BUG: Conversion from float to integer implies rounding, so there's ghost movement when rotating ship; very noticeable when holding R
void ship_rotate(ship_t *ship) {
    int w = ship->hitbox.dim.w;
    int h = ship->hitbox.dim.h;
    int x = ship->hitbox.bl.x;
    int y = ship->hitbox.bl.y;
    int len = w > h ? ship->hitbox.dim.w / 50 : ship->hitbox.dim.h / 50;

    // Get offset to center of the hitbox
    float cx = (float)(x + x + w) / 2.0f;
    float cy = (float)(y + y + h) / 2.0f;
    printf("%d %d %.2f %.2f\n", x, y, cx, cy);

    //Get the center phase coordinates
    float nx = (float)x - cx;
    float ny = (float)y + h - cy;

    // Half pi for a 90 degree rotation
    float temp_x = nx * cosf(HALF_PI) - ny * sinf(HALF_PI);
    float temp_y = nx * sinf(HALF_PI) + ny * cosf(HALF_PI);;


    printf("%.2f %.2f %.2f %.2f\n", nx, ny, temp_x, temp_y);

    ship->hitbox.bl.x = (int)(temp_x + cx);
    ship->hitbox.bl.y = (int)(temp_y + cy);
    ship->hitbox.dim.w = h;
    ship->hitbox.dim.h = w;
    ship->orientation = !ship->orientation;

    //Pass the center of the bottom-left cell to find which cell to snap to in the grid
    int a, b;
    grid_cell_nearest(ship->hitbox.bl.x + 25, WINDOW_HEIGHT - ship->hitbox.bl.y - 25, &a, &b);

    x = 0;
    y = 0;
    grid_cell_bl(a + 1, b, &x, &y);
    ship->grid_xy.x = x;
    ship->grid_xy.y = y;

    // Recalculate grid
    //TODO
    //Rotating outside the grid to horizontal orientation causes the ship to bug its visuals due to how filling the hitbox
    // with cells behaves (wants to fill on a row past the 10th cell, so it wraps around because the matrix in an array
    // in memory, need a workaround for this here
    memset(ship->grid, 0, sizeof(int) * 10 * 10);
    if (ship->orientation == VERTICAL) {
        for (int j = 0; j < len; j++) ship->grid[a - j][b] = 1;
    } else {
        for (int j = 0; j < len; j++) ship->grid[a][b + j] = 1;
    };

    //ship_move(ship, mouse.delta.x, mouse.delta.y);
};

void grid_draw_ships(ship_t *ships) {
    for (int i = 1; i < 6; i++) {
        for (int ii = 0; ii < 10; ii++) {
            for (int jj = 0; jj < 10; jj++) {
                if (ships[i].grid[ii][jj] != 0)
                    grid_cell_fill(ii, jj, ships[i].offset.x, ships[i].offset.y, ships[i].color);
            };
        };
    };
};
