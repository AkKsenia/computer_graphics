#include "library.h"
#include "line.h"
#include "bezier_curve.h"
#include "composite_bezier_curve.h"
#include "polygon.h"
#include "convexity_and_orientation.h"
#include "cyrus_beck.h"


int main() {
    Mat img(800, 900, CV_8UC3, Scalar(255, 255, 255));
    // Mat img(800, 1300, CV_8UC3, Scalar(255, 255, 255));

    // __TASK 1__

    /*
    Point P0(100, 500);
    Point P1(200, 200);
    Point P2(600, 600);
    Point P3(700, 500);

    BezierCurve(img, P0, P1, P2, P3);
    */

    // __TASK 2__
    vector<Point> Points = {
        Point(100, 400),
        Point(500, 600),
        Point(700, 500),
        Point(600, 200),
        Point(500, 100),
        Point(200, 200)
    };
    Point p1 = {100, 300};
    Point p2 = {700, 550};
    Point p3 = {100, 200};
    Point p4 = {600, 100};
    Point p5 = {200, 500};
    Point p6 = {100, 700};
    Point p7 = {300, 200};
    Point p8 = {500, 400};
    Point p9 = {800, 400};
    Point p10 = {500, 700};
    CyrusBeckClipLine(img, p1, p2, Points);
    CyrusBeckClipLine(img, p3, p4, Points);
    CyrusBeckClipLine(img, p5, p6, Points);
    CyrusBeckClipLine(img, p7, p8, Points);
    CyrusBeckClipLine(img, p9, p10, Points);

    // __TASK 3__

    /*
    vector<Point> controlPoints = {
        Point(100, 500),
        Point(200, 100),
        Point(400, 100),
        Point(500, 400),
        Point(600, 700),
        Point(700, 100),
        Point(800, 100),
        Point(900, 100),
        Point(1000, 200),
        Point(1200, 150)
    };

    CompositeBezierCurve(img, controlPoints);
    */

    // imshow("Cubic Bezier curve", img);
    // imshow("Composite cubic Bezier curve", img);
    imshow("ClipLine", img);
    waitKey(0);
    // imwrite("Bezier_curve2.png", img);
    // imwrite("Composite_Bezier_curve2.png", img);
    imwrite("ClipLine.png", img);
    return 0;
}