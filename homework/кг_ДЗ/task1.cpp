#include "library.h"
#include "line.h"
#include "task1.h"


int GetPixel(const Mat& img, int x, int y) {
    if (x >= 0 && x < img.cols && y >= 0 && y < img.rows) {
        Vec3b color = img.at<Vec3b>(y, x);
        return color[0] | (color[1] << 8) | (color[2] << 16);
    }
    return -1;
}


void floodFill8(Mat& img, int x, int y, int newColor, int oldColor) {
    static const int dx[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
    static const int dy[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };

    if (oldColor == newColor) return; // если цвет уже совпадает

    stack<pair<int, int>> stack;
    stack.push({ x, y }); // помещаем затравку в стек

    while (!stack.empty()) {
        pair<int, int> point = stack.top();
        stack.pop();
        x = point.first;
        y = point.second;

        SetPixel(img, x, y,
            newColor & 0xFF,
            (newColor >> 8) & 0xFF,
            (newColor >> 16) & 0xFF);

        for (int i = 0; i < 8; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < img.cols && ny >= 0 && ny < img.rows)
                if (GetPixel(img, nx, ny) == oldColor)
                    stack.push({ nx, ny });
        }
    }
}
