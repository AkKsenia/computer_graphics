#include "library.h"
#include "line.h"


void SetPixel(Mat& img, int x, int y, int r, int g, int b) {
    if (x >= 0 && x < img.cols && y >= 0 && y < img.rows)
        img.at<Vec3b>(y, x) = Vec3b(r, g, b);
}


void Line(Mat& img, int x1, int y1, int x2, int y2, int color) {
    int x = x1, y = y1;
    int dx = x2 - x1, dy = y2 - y1;
    int e, i;
    int ix = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
    if (ix == -1) dx = -dx;
    int iy = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;
    if (iy == -1) dy = -dy;

    auto setPixel = [&](int x, int y) {
        int r = color & 0xFF;
        int g = (color >> 8) & 0xFF;
        int b = (color >> 16) & 0xFF;
        SetPixel(img, x, y, r, g, b);
    };

    if (dx >= dy) {
        e = 2 * dy - dx;
        for (i = 0; i <= dx; i++) {
            setPixel(x, y);
            if (e >= (iy >= 0 ? 0 : 1)) {
                y += iy;
                e -= 2 * dx;
            }
            x += ix;
            e += dy * 2;
        }
    }
    else {
        e = 2 * dx - dy;
        for (i = 0; i <= dy; i++) {
            setPixel(x, y);
            if (e >= (ix >= 0 ? 0 : 1)) {
                x += ix;
                e -= 2 * dy;
            }
            y += iy;
            e += dx * 2;
        }
    }
}