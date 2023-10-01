#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include <windows.h>
#include "stdbool.h"
enum { VERTICAL, HORIZONTAL };

typedef enum { WHITE, BLACK, RED, GREEN } color_t;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define HALF_PI 1.570796f

// clang-format off
const int matrix1[10][10] = {
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

const int matrix2[10][10] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

const int matrix3[10][10] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

const int matrix4[10][10] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

const int matrix5[10][10] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
};

// clang-format on

typedef struct ship_t {
    int id;
    int grid[10][10];
    // 1 is horizontal 0 is vertical
    int orientation;
    color_t color;
    bool dragged;
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

typedef struct mouse_t {
    struct current {
        int x;
        int y;
    } current;
    struct previous {
        int x;
        int y;
    } previous;
    struct delta {
        int x;
        int y;
    } delta;
} mouse_t;

ship_t ships[6] = {0};

mouse_t mouse = {0};

// typedef BOOL (__stdcall* PFNGLSWAPINTERVALEXTPROC) (int interval);

// Window functions
HWND create_window();
LRESULT CALLBACK event_handler(HWND handle, unsigned event, WPARAM wparam, LPARAM lparam);

// Conversions from pixels to percentages
float norm_fx(float x);
float norm_fy(float y);
float norm_ix(int x);
float norm_iy(int y);

void grid_draw_grid();
void grid_draw_ships();
void grid_cell_fill(int a, int b, int px_offset, int py_offset, color_t color);
void grid_cell_center(int a, int b, int *x, int *y);
void grid_cell_bl(int a, int b, int *x, int *y);
void grid_cell_nearest(int x, int y, int *na, int *nb);
// Returns ship id (1-6)
int ship_cursor_collides(int x, int y);
bool ship_ship_collides(ship_t *shipA, ship_t *shipB);
void ship_init(ship_t *ship, int grid[10][10]);
void ship_move(ship_t *ship, int dx, int dy);
void hitbox_draw(ship_t *ship);
void ship_rotate(ship_t *ship);

HWND create_window() {
    // Register window clas
    WNDCLASS window_class = {0};
    window_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.hInstance = GetModuleHandle(NULL);
    window_class.lpfnWndProc = event_handler;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;
    window_class.lpszMenuName = NULL;
    window_class.hbrBackground = NULL;
    window_class.hbrBackground = CreateSolidBrush(RGB(255, 0, 0));
    window_class.lpszClassName = "BS_MAIN";
    if (!RegisterClass(&window_class)) {
        printf("[FAIL] Failed to register window class!\n");
        return 0;
    };

    // Window rectangle
    DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME;
    RECT window_rect = {.left = 0, .right = (long)WINDOW_WIDTH, .top = 0, .bottom = (long)WINDOW_HEIGHT};
    AdjustWindowRectEx(&window_rect, dwStyle, false, dwExStyle);
    int rect_width = window_rect.right - window_rect.left;
    int rect_height = window_rect.bottom - window_rect.top;

    // Create and show window
    HWND window_handle = (HWND)CreateWindowEx(dwExStyle, "BS_MAIN", "Battleships", dwStyle, 100, 100, rect_width,
                                              rect_height, NULL, NULL, GetModuleHandle(NULL), NULL);
    if (window_handle == 0) {
        printf("Failed to create window!\n");
        return 0;
    };
    ShowWindow(window_handle, 1);
    return window_handle;
};

LRESULT CALLBACK event_handler(HWND handle, unsigned event, WPARAM wparam, LPARAM lparam) {
    switch (event) {
        case WM_KEYDOWN: {
            if (wparam == 0x52) {
                for (int i = 1; i < 6; i++) {
                    if (ships[i].dragged) ship_rotate(&ships[i]);
                };
            };
            break;
        };
        case WM_LBUTTONDOWN: {
            printf("Mouse clicked!\n");
            printf("%d %d\n", LOWORD(lparam), HIWORD(lparam));
            // Flip vertical coords because different origin points
            int ship_to_drag = ship_cursor_collides(LOWORD(lparam), WINDOW_HEIGHT - (int)HIWORD(lparam));
            if (ship_to_drag != 0) ships[ship_to_drag].dragged = true;
            break;
        };
        case WM_LBUTTONUP: {
            for (int i = 1; i < 6; i++) {
                if (ships[i].dragged) {
                    ships[i].dragged = 0;
                    ships[i].offset.x = 0;
                    ships[i].offset.y = 0;
                    int a, b;
                    grid_cell_nearest(ships[i].hitbox.bl.x + 25, WINDOW_HEIGHT - ships[i].hitbox.bl.y - 25, &a, &b);

                    int x, y;
                    grid_cell_bl(a + 1, b, &x, &y);
                    ships[i].hitbox.bl.x = x;
                    ships[i].hitbox.bl.y = y;
                    ships[i].grid_xy.x = x;
                    ships[i].grid_xy.y = y;

                    // Recalculate grid
                    // Bottom and left overflow should not have issues due to how the nearest cell is calculated
                    int len = 0;
                    memset(ships[i].grid, 0, sizeof(int) * 10 * 10);
                    if (ships[i].orientation == VERTICAL) {
                        len = ships[i].hitbox.dim.h / 50;
                        while (a - len + 1 < 0) {
                            a++;
                            ships[i].hitbox.bl.y -= 50;
                            ships[i].grid_xy.y -= 50;
                        };

                        for (int j = 0; j < len; j++) ships[i].grid[a - j][b] = 1;
                    } else {
                        len = ships[i].hitbox.dim.w / 50;
                        while (b + len - 1 > 9) {
                            b--;
                            ships[i].hitbox.bl.x -= 50;
                            ships[i].grid_xy.x -= 50;
                        };

                        for (int j = 0; j < len; j++) ships[i].grid[a][b + j] = 1;
                    };

                    for (int k = 0; k < 10; k++) {
                        for (int j = 0; j < 10; j++) printf("%d ", ships[i].grid[k][j]);
                        printf("\n");
                    }
                };
            };
            break;
        };
        case WM_MOUSEMOVE: {
            mouse.previous.x = mouse.current.x;
            mouse.previous.y = mouse.current.y;
            mouse.current.x = (int)LOWORD(lparam);
            mouse.current.y = WINDOW_HEIGHT - (int)HIWORD(lparam);
            mouse.delta.x = mouse.current.x - mouse.previous.x;
            mouse.delta.y = mouse.current.y - mouse.previous.y;

            for (int i = 1; i < 6; i++) {
                if (ships[i].dragged) { ship_move(&ships[i], mouse.delta.x, mouse.delta.y); }
            };

            break;
        };
        case WM_CLOSE: {
            PostQuitMessage(0);
            break;
        };
    };
    return DefWindowProc(handle, event, wparam, lparam);
};

float norm_fx(float x) {
    return 2.0f * (x / ((float)WINDOW_WIDTH)) - 1.0f;
};

float norm_fy(float y) {
    return 2.0f * (y / ((float)WINDOW_HEIGHT)) - 1.0f;
};

float norm_ix(int x) {
    return 2.0f * (((float)x) / ((float)WINDOW_WIDTH)) - 1.0f;
};

float norm_iy(int y) {
    return 2.0f * (((float)y) / ((float)WINDOW_HEIGHT)) - 1.0f;
};

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

void grid_draw_ships() {
    for (int i = 1; i < 6; i++) {
        for (int ii = 0; ii < 10; ii++) {
            for (int jj = 0; jj < 10; jj++) {
                if (ships[i].grid[ii][jj] != 0)
                    grid_cell_fill(ii, jj, ships[i].offset.x, ships[i].offset.y, ships[i].color);
            };
        };
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

int ship_cursor_collides(int x, int y) {
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
        printf("collsion\n");
        return true;
    } else {
        printf("no collision\n");
        return false;
    }
};

void ship_init(ship_t *ship, int grid[10][10]) {
    //  max would be 9 * sqrt(2), but 20 to be safe
    memcpy(ship->grid, grid, sizeof(int) * 10 * 10);
    float min_dist = 20.0f;
    int ship_size = 0;
    int is_vertical = false;
    // Will get the most top-left cell
    int min_i = 0, min_j = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (ship->grid[i][j] != 0) {
                ship_size++;
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

void ship_rotate(ship_t *ship) {
    int w = ship->hitbox.dim.w;
    int h = ship->hitbox.dim.h;
    int x = ship->hitbox.bl.x;
    int y = ship->hitbox.bl.y;
    int len = w > h ? ship->hitbox.dim.w / 50 : ship->hitbox.dim.h / 50;

    float cx = (float)(x + x + w) / 2.0f;
    float cy = (float)(y + y + h) / 2.0f;
    printf("%d %d %.2f %.2f\n", x, y, cx, cy);

    float nx = (float)x - cx;
    float ny = (float)y + h - cy;

    float temp_x;
    float temp_y;
    // pi/2
    temp_x = nx * cosf(HALF_PI) - ny * sinf(HALF_PI);
    temp_y = nx * sinf(HALF_PI) + ny * cosf(HALF_PI);

    printf("%.2f %.2f %.2f %.2f\n", nx, ny, temp_x, temp_y);

    ship->hitbox.bl.x = (int)(temp_x + cx);
    ship->hitbox.bl.y = (int)(temp_y + cy);
    ship->hitbox.dim.w = h;
    ship->hitbox.dim.h = w;
    ship->orientation = !ship->orientation;

    int a, b;
    grid_cell_nearest(ship->hitbox.bl.x + 25, WINDOW_HEIGHT - ship->hitbox.bl.y - 25, &a, &b);

    x = 0;
    y = 0;
    grid_cell_bl(a + 1, b, &x, &y);
    ship->grid_xy.x = x;
    ship->grid_xy.y = y;

    // Recalculate grid
    memset(ship->grid, 0, sizeof(int) * 10 * 10);
    if (ship->orientation == VERTICAL) {
        for (int j = 0; j < len; j++) ship->grid[a - j][b] = 1;
    } else {
        for (int j = 0; j < len; j++) ship->grid[a][b + j] = 1;
    };

    ship_move(ship, mouse.delta.x, mouse.delta.y);
};

int main() {
    HWND window_handle = create_window();
    HDC window_hdc = GetDC(window_handle);

    // OpenGL context
    PIXELFORMATDESCRIPTOR pixel_format_desc = {sizeof(PIXELFORMATDESCRIPTOR),
                                               1,
                                               PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
                                               PFD_TYPE_RGBA,
                                               32,
                                               0,
                                               0,
                                               0,
                                               0,
                                               0,
                                               0,
                                               0,
                                               0,
                                               0,
                                               0,
                                               0,
                                               0,
                                               0,
                                               24,
                                               8,
                                               0,
                                               PFD_MAIN_PLANE,
                                               0,
                                               0,
                                               0,
                                               0};

    int suggested_pf_index = ChoosePixelFormat(window_hdc, &pixel_format_desc);
    PIXELFORMATDESCRIPTOR suggested_pf;
    DescribePixelFormat(window_hdc, suggested_pf_index, sizeof(suggested_pf), &suggested_pf);
    SetPixelFormat(window_hdc, suggested_pf_index, &suggested_pf);

    HGLRC window_context = wglCreateContext(window_hdc);
    wglMakeCurrent(window_hdc, window_context);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    memcpy(ships[1].grid, matrix1, sizeof(int) * 10 * 10);
    memcpy(ships[2].grid, matrix2, sizeof(int) * 10 * 10);
    memcpy(ships[3].grid, matrix3, sizeof(int) * 10 * 10);
    memcpy(ships[4].grid, matrix4, sizeof(int) * 10 * 10);
    memcpy(ships[5].grid, matrix5, sizeof(int) * 10 * 10);
    for (int i = 0; i < 6; i++) {
        ships[i].id = i;
        ship_init(&ships[i], ships[i].grid);
    };

    int is_active = true;
    MSG msg;

    while (is_active) {
        // Process inputs
        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) is_active = false;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        };

        // Clear screen (to bg color)
        glClearColor(0.018f, 0.55f, 0.84f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Process scene
        for (int i = 1; i < 6; i++) ships[i].color = WHITE;
        for (int i = 1; i < 6; i++) {
            for (int j = i + 1; j < 6; j++) {
                if (ship_ship_collides(&ships[i], &ships[j])) {
                    ships[i].color = RED;
                    ships[j].color = RED;
                };
            };
        };

        // Draw scene elements
        grid_draw_grid();
        grid_draw_ships();
        for (int i = 1; i < 6; i++) hitbox_draw(&ships[i]);

        ship_ship_collides(&ships[1], &ships[2]);

        SwapBuffers(window_hdc);

        continue;
    };

    DestroyWindow(window_handle);
    UnregisterClass("BS_MAIN", GetModuleHandle(NULL));
    return 0;
};