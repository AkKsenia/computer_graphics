#include "library.h"
#include "line.h"
#include "bezier_curve.h"


Point BezierPoint(Point P0, Point P1, Point P2, Point P3, double t) {
    return pow(1 - t, 3) * P0 + 3 * t * pow(1 - t, 2) * P1 + 3 * pow(t, 2) * (1 - t) * P2 + pow(t, 3) * P3;
}


float Dist(Point P0, Point P1) {
    return abs(P0.x - P1.x) + abs(P0.y - P1.y);
}


float MaxDist(Point P0, Point P1, Point P2, Point P3) {
    float d1 = Dist(P0 - 2 * P1 + P2, Point(0, 0));
    float d2 = Dist(P1 - 2 * P2 + P3, Point(0, 0));
    return max(d1, d2);
}


// construction of third-order Bezier curves
void BezierCurve(Mat& img, Point& P0, Point& P1, Point& P2, Point& P3) {
    float H = MaxDist(P0, P1, P2, P3);
    int N = 1 + static_cast<int>(sqrt(3 * H));

    vector<Point> curvePoints;
    for (int i = 0; i <= N; ++i) {
        float t = static_cast<float>(i) / N;
        Point point = BezierPoint(P0, P1, P2, P3, t);
        curvePoints.push_back(point);
    }

    for (size_t i = 1; i < curvePoints.size(); ++i) {
        Line(img, curvePoints[i - 1].x, curvePoints[i - 1].y, curvePoints[i].x, curvePoints[i].y, 0xB4A7D6);
    }

    for (auto& point : { P0, P1, P2, P3 }) {
        Line(img, point.x - 3, point.y, point.x + 3, point.y, 0x000000);
        Line(img, point.x, point.y - 3, point.x, point.y + 3, 0x000000);
    }
}