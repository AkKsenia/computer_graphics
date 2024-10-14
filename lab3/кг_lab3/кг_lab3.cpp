#include "library.h"
#include "set_pixel.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"


int main() {
    Mat img(450, 500, CV_8UC3, Scalar(255, 255, 255));

    // __TASK 1__

    //Line(img, 10, 10, 440, 440, 0x8E7CC3);
    //Line(img, 60, 10, 490, 440, 0xC27BA0);

    // __TASK 2__

    // setting the square
    /* vector<Point> points = {Point(100, 100), Point(400, 100), Point(400, 400), Point(100, 400)}; */

    // drawing a star

    vector<Point> points;
    int numPoints = 10;
    double outerRadius = 200, innerRadius = 100;
    double centerX = 250, centerY = 250;
    double angle = 0, angleStep = M_PI / numPoints;

    for (int i = 0; i < numPoints * 2; i++) {
        double radius = (i % 2 == 0) ? outerRadius : innerRadius;
        double x = centerX + radius * cos(angle);
        double y = centerY + radius * sin(angle);
        points.push_back(Point(x, y));
        angle += angleStep;
    }

    // there are self-intersections here
    /* vector<Point> points = {Point(100, 100), Point(300, 100), Point(200, 200), Point(300, 300), Point(100, 300), Point(200, 200)}; */

    DrawPolygon(img, points, 0xB4A7D6);

    // __TASK 4__
    FillPolygon(img, points, 0xB4A7D6, false);

    //imshow("Image with Lines", img);
    imshow("Polygon", img);
    waitKey(0);

    // __TASK 3__

    //cout << (isConvexPolygon(points) ? "True" : "False") << endl;
    //cout << (isSimplePolygon(points) ? "True" : "False") << endl;

    //imwrite("lines.png", img);
    //imwrite("polygon1.png", img);
    imwrite("polygon2_non_zero_winding.png", img);

    return 0;
}
