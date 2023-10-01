#include "grid.h"

void grid_draw_grid() {
    float x1, x2, y1, y2;
    x1 = norm_ix(10);
    x2 = norm_ix(510);
    for (int i = 1; i <= 11; i++) {
        float y1 = norm_iy(i * 50);
        float y2 = norm_iy(i * 50);
        glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    };

    y1 = norm_iy(50);
    y2 = norm_iy(550);
    for (int i = 0; i <= 10; i++) {
        float x1 = norm_ix(10 + i * 50);
        float x2 = norm_ix(10 + i * 50);
        glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    };
};

void grid_cell_fill(int a, int b, int px_offset, int py_offset, color_t color) {
    glBegin(GL_QUADS);
    switch (color) {
        case WHITE: {
            glColor3f(1.0f, 1.0f, 1.0f);
            break;
        };
        case RED: {
            glColor3f(1.0f, 0.0f, 0.0f);
            break;
        };
        case GREEN: {
            glColor3f(0.0f, 1.0f, 0.0f);
            break;
        };
        case BLACK: {
            glColor3f(0.0f, 0.0f, 0.0f);
            break;
        };
    }
    glVertex2f(norm_ix(15 + b * 50 + px_offset), norm_iy(WINDOW_HEIGHT - 55 - a * 50 + py_offset));
    glVertex2f(norm_ix(55 + b * 50 + px_offset), norm_iy(WINDOW_HEIGHT - 55 - a * 50 + py_offset));
    glVertex2f(norm_ix(55 + b * 50 + px_offset), norm_iy(WINDOW_HEIGHT - 95 - a * 50 + py_offset));
    glVertex2f(norm_ix(15 + b * 50 + px_offset), norm_iy(WINDOW_HEIGHT - 95 - a * 50 + py_offset));
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();
};

void grid_cell_center(int a, int b, int *x, int *y) {
    *x = 10 + (b + 1) * 50 - 25;
    *y = WINDOW_HEIGHT - 50 - (a + 1) * 50 + 25;
};

void grid_cell_bl(int a, int b, int *x, int *y) {
    *x = 10 + b * 50;
    *y = WINDOW_HEIGHT - 50 - a * 50;
};

void grid_cell_nearest(int x, int y, int *na, int *nb) {
    int min_i, min_j;
    y = WINDOW_HEIGHT - y;
    float min_d = 100000.0f;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            int cx, cy;
            grid_cell_center(i, j, &cx, &cy);
            float d = sqrtf((float)((x - cx) * (x - cx) + (y - cy) * (y - cy)));
            if (d < min_d) {
                min_d = d;
                min_i = i;
                min_j = j;
            };
        };
    };

    *na = min_i;
    *nb = min_j;
    int cx, cy;
    grid_cell_center(min_i, min_j, &cx, &cy);
    glBegin(GL_POINTS);
    glVertex2f(norm_ix(cx), norm_iy(cy));
    glEnd();
};