#include "library.h"
#include "line.h"
#include "bezier_curve.h"
#include "composite_bezier_curve.h"


float MyDist(Point P0, Point P1) {
    return sqrt(pow(P0.x - P1.x, 2) + pow(P0.y - P1.y, 2));
}


bool CheckSmoothness(Point& P2, Point& P3, Point& Q1) {
    float P2P3 = MyDist(P2, P3);
    float P3Q1 = MyDist(P3, Q1);
    float P2Q1 = MyDist(P2, Q1);
    return abs(P2P3 + P3Q1 - P2Q1) < 1e-6; // here we take into account a small error
}


// construction of composite cubic Bezier curves
void CompositeBezierCurve(Mat& img, vector<Point>& controlPoints) {
    int n = controlPoints.size();
    if (n < 4 || n % 3 != 1) {
        cerr << "Amount of control points must be equal to 4, 7, 10, etc." << endl;
        return;
    }

    for (size_t i = 0; i < n - 3; i += 3) {
        Point P0 = controlPoints[i];
        Point P1 = controlPoints[i + 1];
        Point P2 = controlPoints[i + 2];
        Point P3 = controlPoints[i + 3];


        if (i != n - 4)
            if (!CheckSmoothness(P2, P3, controlPoints[i + 4])) {
                cerr << "The connection is not smooth, choose another option for point Q1" << endl;
                return;
            }

        BezierCurve(img, P0, P1, P2, P3);
    }
}