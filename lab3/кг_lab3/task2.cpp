#include "library.h"
#include "task2.h"
#include "task1.h"


// displaying the polygon
void DrawPolygon(Mat& img, vector<Point> points, int color) {
    for (int i = 0; i < points.size(); i++) {
        Point p1 = points[i];
        Point p2 = points[(i + 1) % points.size()];
        Line(img, p1.x, p1.y, p2.x, p2.y, color);
    }
}