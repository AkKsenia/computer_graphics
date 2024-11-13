#pragma once
#define _USE_MATH_DEFINES
#include <opencv2/opencv.hpp> 
#include <iostream> 
#include <cmath>

using namespace cv;
using namespace std;


void SetPixel(Mat& img, int x, int y, int r, int g, int b) {
    if (x >= 0 && x < img.cols && y >= 0 && y < img.rows)
        img.at<Vec3b>(y, x) = Vec3b(r, g, b);
}


void Line(Mat& img, int x1, int y1, int x2, int y2, int color) {
    int x = x1, y = y1;
    int dx = x2 - x1, dy = y2 - y1;
    int e, i;

    int ix = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
    if (ix == -1) dx = -dx;

    int iy = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;
    if (iy == -1) dy = -dy;

    auto setPixel = [&](int x, int y) {
        int r = color & 0xFF;
        int g = (color >> 8) & 0xFF;
        int b = (color >> 16) & 0xFF;
        SetPixel(img, x, y, r, g, b);
    };

    if (dx >= dy) {
        e = 2 * dy - dx;
        for (i = 0; i <= dx; i++) {
            setPixel(x, y);
            if (e >= (iy >= 0 ? 0 : 1)) {
                y += iy;
                e -= 2 * dx;
            }
            x += ix;
            e += dy * 2;
        }
    }
    else {
        e = 2 * dx - dy;
        for (i = 0; i <= dy; i++) {
            setPixel(x, y);
            if (e >= (ix >= 0 ? 0 : 1)) {
                x += ix;
                e -= 2 * dy;
            }
            y += iy;
            e += dx * 2;
        }
    }
}


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

int main() {
    Mat img(800, 1300, CV_8UC3, Scalar(255, 255, 255));

    // __TASK 2__

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

    imshow("Composite Catmull-Rom curve", img);
    waitKey(0);
    imwrite("Composite_CatmullRom_curve.png", img);
    return 0;
}
