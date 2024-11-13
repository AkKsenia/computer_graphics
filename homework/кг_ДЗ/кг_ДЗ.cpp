#include "library.h"
#include "line.h"
#include "task1.h"
#include "task2.h"


int main() {
    Mat img(400, 400, CV_8UC3, Scalar(255, 255, 255));
    //Mat img(800, 1300, CV_8UC3, Scalar(255, 255, 255));

    // __TASK 1__

    // смайлик
    circle(img, Point(200, 200), 100, Scalar(132, 234, 253), -1);
    circle(img, Point(160, 180), 20, Scalar(102, 102, 102), -1);
    circle(img, Point(240, 180), 20, Scalar(102, 102, 102), -1);
    ellipse(img, Point(200, 220), Size(55, 30), 0, 0, 180, Scalar(102, 102, 102), 2);

    int oldColor = 0xFDEA84;
    int newColor = 0xD9D2E9;
    int x = 200;
    int y = 200;

    int currentColor = GetPixel(img, x, y);

    if (currentColor == oldColor) {
        floodFill8(img, x, y, newColor, oldColor);
    }

    // __TASK 2__

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

    CompositeCatmullRom(img, controlPoints);
    */

    imshow("Flood Fill", img);
    //imshow("Composite Catmull-Rom curve", img);
    waitKey(0);
    imwrite("Flood_Fill.png", img);
    //imwrite("Composite_CatmullRom_curve.png", img);
    return 0;
}
