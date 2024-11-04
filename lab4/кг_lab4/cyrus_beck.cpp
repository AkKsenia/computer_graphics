#include "library.h"
#include "line.h"
#include "polygon.h"
#include "convexity_and_orientation.h"
#include "cyrus_beck.h"


// the Cyrus-Beck algorithm
void CyrusBeckClipLine(Mat& img, Point& p1, Point& p2, const vector<Point>& points) {
    // checking for the convexity of the polygon
    if (!isConvexPolygon(points)) {
        cerr << "Polygon is not convex!" << endl;
        return;
    }
    // checking for the correct orientation of the polygon
    if (!isClockwise(points)) {
        cerr << "Polygon is not correctly oriented!" << endl;
        return;
    }

    DrawPolygon(img, points, 0x000000);
    Line(img, p1.x, p1.y, p2.x, p2.y, 0x000000);

    int n = points.size();
    double t1 = 0, t2 = 1, t;
    double sx = p2.x - p1.x, sy = p2.y - p1.y;
    double nx, ny, denom, num;

    for (int i = 0; i < n; i++) {
        nx = points[(i + 1) % n].y - points[i].y;
        ny = points[i].x - points[(i + 1) % n].x;
        denom = nx * sx + ny * sy;
        num = nx * (p1.x - points[i].x) + ny * (p1.y - points[i].y);

        if (denom != 0) {
            t = -num / denom;
            if (denom > 0) {
                if (t > t1) t1 = t;
            }
            else {
                if (t < t2) t2 = t;
            }
        }
        if (t1 > t2) return;
    }

    if (t1 <= t2) {
        Point p1_new = p1 + t1 * (p2 - p1);
        Point p2_new = p1 + t2 * (p2 - p1);

        Line(img, p1_new.x, p1_new.y, p2_new.x, p2_new.y, 0xC27BA0);

        p1 = p1_new;
        p2 = p2_new;
    }
}