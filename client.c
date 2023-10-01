#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include <windows.h>
#include "ship.h"
#include "common.h"
#include "grid.h"

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

int enemy_matrix[10][10] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

int damage_matrix[10][10] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

// clang-format on

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

typedef struct gamestate_t {
    int isPrep;
    int isReady;
    int phase;
    int player_turn;
} gamestate_t;

HWND create_window();
LRESULT CALLBACK event_handler(HWND handle, unsigned event, WPARAM wparam, LPARAM lparam);

ship_t ships[6] = {0};
mouse_t mouse = {0};
gamestate_t gamestate = {0};


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
    }

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
            //R
            if (wparam == 0x52) {
                for (int i = 1; i < 6; i++) {
                    if (ships[i].dragged) {
                        ship_rotate(&ships[i]);
                        ship_move(&ships[i], mouse.delta.x, mouse.delta.y);
                    }

                };
            };
            break;
        };
        case WM_LBUTTONDOWN: {
            printf("Mouse clicked!\n");
            printf("%d %d\n", LOWORD(lparam), HIWORD(lparam));
            // Flip vertical coords because different origin points
            if (gamestate.phase == 0) {
                int ship_to_drag = ship_cursor_collides(ships, LOWORD(lparam), WINDOW_HEIGHT - (int) HIWORD(lparam));
                if (ship_to_drag != 0) ships[ship_to_drag].dragged = true;
            };
            break;
        };
        case WM_LBUTTONUP: {
            if (gamestate.phase == 0) {
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
            }
            break;
        };
        case WM_MOUSEMOVE: {
            mouse.previous.x = mouse.current.x;
            mouse.previous.y = mouse.current.y;
            mouse.current.x = (int)LOWORD(lparam);
            mouse.current.y = WINDOW_HEIGHT - (int)HIWORD(lparam);
            mouse.delta.x = mouse.current.x - mouse.previous.x;
            mouse.delta.y = mouse.current.y - mouse.previous.y;

            if (gamestate.phase == 0) {
                for (int i = 1; i < 6; i++) {
                    if (ships[i].dragged) { ship_move(&ships[i], mouse.delta.x, mouse.delta.y); }
                };
            };

            break;
        };
        case WM_CLOSE: {
            PostQuitMessage(0);
            break;
        };
        default: break;
    };
    return DefWindowProc(handle, event, wparam, lparam);
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
        mouse.delta.x = 0;
        mouse.delta.y = 0;
        // Process inputs
        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) is_active = false;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        };

        // Clear phase (to bg color)
        glClearColor(0.018f, 0.55f, 0.84f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //IN THE PREP SCREEN
        if (gamestate.phase == 0) {
            // Process scene
            for (int i = 1; i < 6; i++) {
                ships[i].color = WHITE;
                ships[i].is_in_collision = false;
            }
            for (int i = 1; i < 6; i++) {
                for (int j = i + 1; j < 6; j++) {
                    if (ship_ship_collides(&ships[i], &ships[j])) {
                        ships[i].color = RED;
                        ships[j].color = RED;
                        ships[i].is_in_collision = true;
                        ships[j].is_in_collision = true;
                    };
                };
            };

            // Draw scene elements
            grid_draw_grid();
            grid_draw_ships(ships);
            //DEBUG hitboxes
            for (int i = 1; i < 6; i++) hitbox_draw(&ships[i]);

            //ship_ship_collides(&ships[1], &ships[2]);
        //IN THE GAME PHASE
        } else if (gamestate.phase == 1) {
            //PLAYER'S TURN, DISPLAY ENEMY MATRIX
            if (gamestate.player_turn == 1) {
                grid_draw_grid();
                //grid_draw_enemy_hits(enemy_matrix);
                //grid_draw_cursor(GRAY);
            //ENEMY TURN, DISPLAY OWN MATRIX AND WAIT FOR HIT
            } else if (gamestate.player_turn == 0) {
                grid_draw_grid();
                grid_draw_ships(ships);
                //grid_draw_damage(damage_matrix);
                for (int i = 1; i < 6; i++) hitbox_draw(&ships[i]);
            }
        //END OF GAME PHASE
        } else if (gamestate.phase == 2) {

        }


        SwapBuffers(window_hdc);
    };

    DestroyWindow(window_handle);
    UnregisterClass("BS_MAIN", GetModuleHandle(NULL));
    return 0;
};