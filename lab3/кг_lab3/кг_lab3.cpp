#include "library.h"
#include "set_pixel.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"


int main() {
    Mat img(450, 500, CV_8UC3, Scalar(255, 255, 255));
    //Mat img(10, 10, CV_8UC3, Scalar(255, 255, 255));

    // __TASK 1__

    //Line(img, 10, 10, 440, 440, 0x8E7CC3);
    //Line(img, 60, 10, 490, 440, 0xC27BA0);
    //Line(img, 0, 0, 8, 3, 0x8E7CC3);
    //Line(img, 8, 3, 0, 0, 0x8E7CC3);

    // __TASK 2__

    // square
    // vector<Point> points = {Point(100, 100), Point(400, 100), Point(400, 400), Point(100, 400)};

    // star
    // vector<Point> points = {Point(450, 250), Point(88, 368), Point(312, 60), Point(312, 440), Point(88, 132)};

    // hourglass
    // vector<Point> points = {Point(100, 100), Point(300, 300), Point(100, 300), Point(300, 100)};

    // weird figure
    vector<Point> points = {Point(400, 100), Point(88, 368), Point(312, 60), Point(450, 250), Point(312, 440), Point(88, 132)};

    DrawPolygon(img, points, 0xB4A7D6);

    // __TASK 4__
    FillPolygon(img, points, 0xB4A7D6, false);

    //imshow("Image with Lines", img);
    imshow("Polygon", img);
    waitKey(0);

    // __TASK 3__

    // cout << (isConvexPolygon(points) ? "True" : "False") << endl;
    // cout << (isSimplePolygon(points) ? "True" : "False") << endl;

    //imwrite("lines.png", img);
    //imwrite("polygon4.png", img);
    imwrite("polygon4_non_zero_winding.png", img);

    return 0;
}
