#include "library.h"
#include "set_pixel.h"
#include "task4.h"


// function to check whether a point is inside a polygon according to the even-odd rule
bool isInsideEvenOdd(const vector<Point>& points, const Point& p) {
    int count = 0;
    int n = points.size();

    for (int i = 0; i < n; i++) {
        Point p1 = points[i];
        Point p2 = points[(i + 1) % n];

        if ((p1.y > p.y) != (p2.y > p.y)) {
            double xIntersection = (double)(p2.x - p1.x) * (p.y - p1.y) / (p2.y - p1.y) + p1.x;
            if (p.x < xIntersection) {
                count++;
            }
        }
    }
    return (count % 2) == 1;
}

// function to check whether a point is inside a polygon according to the non-zero-winding rule
bool isInsideNonZeroWinding(const vector<Point>& points, const Point& p) {
    int windingNumber = 0;
    int n = points.size();

    for (int i = 0; i < n; i++) {
        Point p1 = points[i];
        Point p2 = points[(i + 1) % n];

        if (p1.y <= p.y) {
            if (p2.y > p.y && (p2.x - p1.x) * (p.y - p1.y) - (p2.y - p1.y) * (p.x - p1.x) > 0) {
                windingNumber++;
            }
        }
        else {
            if (p2.y <= p.y && (p2.x - p1.x) * (p.y - p1.y) - (p2.y - p1.y) * (p.x - p1.x) < 0) {
                windingNumber--;
            }
        }
    }
    return windingNumber != 0;
}

// filling the polygon
void FillPolygon(Mat& img, const vector<Point>& points, int color, bool useEvenOddRule) {
    int minX = img.cols, maxX = 0, minY = img.rows, maxY = 0;
    for (const Point& p : points) {
        minX = min(minX, p.x);
        maxX = max(maxX, p.x);
        minY = min(minY, p.y);
        maxY = max(maxY, p.y);
    }

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            Point p(x, y);
            bool inside = useEvenOddRule ? isInsideEvenOdd(points, p) : isInsideNonZeroWinding(points, p);
            if (inside) {
                int r = color & 0xFF;
                int g = (color >> 8) & 0xFF;
                int b = (color >> 16) & 0xFF;
                SetPixel(img, x, y, r, g, b);
            }
        }
    }
}