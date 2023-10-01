#include "common.h"

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
