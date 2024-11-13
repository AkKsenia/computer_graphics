#include "library.h"
#include "line.h"
#include "task2.h"


Point CatmullRomPoint(Point P0, Point P1, Point P2, Point P3, double t) {
    return 0.5 * (-t * pow(1 - t, 2) * P0 + (2 - 5 * pow(t, 2) + 3 * pow(t, 3)) * P1 + t * (1 + 4 * t - 3 * pow(t, 2)) * P2 - pow(t, 2) * (1 - t) * P3);
}


double Dist(Point P0, Point P1) {
    return abs(P0.x - P1.x) + abs(P0.y - P1.y);
}


double MaxDist(Point P0, Point P1, Point P2, Point P3) {
    double d1 = Dist(P0 - 2 * P1 + P2, Point(0, 0));
    double d2 = Dist(P1 - 2 * P2 + P3, Point(0, 0));
    return max(d1, d2);
}


void CatmullRom(Mat& img, Point& P0, Point& P1, Point& P2, Point& P3) {
    double H = MaxDist(P0, P1, P2, P3);
    int N = 1 + static_cast<int>(sqrt(3 * H));

    Point prevPoint = CatmullRomPoint(P0, P1, P2, P3, 0.0);
    for (int i = 1; i <= N; ++i) {
        double t = static_cast<double>(i) / N;
        Point point = CatmullRomPoint(P0, P1, P2, P3, t);
        Line(img, prevPoint.x, prevPoint.y, point.x, point.y, 0xB4A7D6);
        prevPoint = point;
    }

    for (auto& point : { P0, P1, P2, P3 }) {
        Line(img, point.x - 3, point.y, point.x + 3, point.y, 0x000000);
        Line(img, point.x, point.y - 3, point.x, point.y + 3, 0x000000);
    }
}


void CompositeCatmullRom(Mat& img, vector<Point>& controlPoints) {
    int n = controlPoints.size();

    for (size_t i = 0; i < n - 3; ++i) {
        Point P0 = controlPoints[i];
        Point P1 = controlPoints[i + 1];
        Point P2 = controlPoints[i + 2];
        Point P3 = controlPoints[i + 3];

        CatmullRom(img, P0, P1, P2, P3);
    }
}