#include "library.h"
#include "task3.h"


// determining the angle between 3 points
double getAngle(Point p1, Point p2, Point p3) {
    double dx1 = p2.x - p1.x;
    double dy1 = p2.y - p1.y;
    double dx2 = p3.x - p2.x;
    double dy2 = p3.y - p2.y;
    double angle = atan2(dy2, dx2) - atan2(dy1, dx1);

    if (angle > M_PI) angle -= 2 * M_PI;
    if (angle < -M_PI) angle += 2 * M_PI;

    return angle * 180 / M_PI;
}

// determining the convexity of a polygon
bool isConvexPolygon(vector<Point> points) {
    int n = points.size();
    if (n < 3) {
        return false;
    }

    bool isConvex = true;
    double prev_angle = 0;

    for (int i = 0; i < n; i++) {
        Point p1 = points[i];
        Point p2 = points[(i + 1) % n];
        Point p3 = points[(i + 2) % n];
        double angle = getAngle(p1, p2, p3);

        if (i > 0) {
            if ((angle > 0) != (prev_angle > 0)) {
                isConvex = false;
                break;
            }
        }
        prev_angle = angle;
    }

    return isConvex;
}

// checking the intersection of segments
bool doIntersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
    double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    if (denom == 0) {
        return false;
    }
    else {
        // the formulas are taken from here: https://en.wikipedia.org/wiki/Line-line_intersection
        double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
        double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denom;
        return (t >= 0 && t <= 1 && u >= 0 && u <= 1);
    }
}

// defining the polygon type
bool isSimplePolygon(vector<Point> points) {
    int n = points.size();

    for (int i = 0; i < n - 3; i++) {
        if (doIntersect(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y,
            points[i + 2].x, points[i + 2].y, points[i + 3].x, points[i + 3].y)) {
            return false;
        }
    }
    return true;
}