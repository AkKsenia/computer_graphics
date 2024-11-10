#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
using namespace cv;
using namespace std;


struct Point3D {
    float x, y, z;
};


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


Point projectPoint(const Point3D& point, float k) {
    float x_proj = point.x * k / (k + point.z);
    float y_proj = point.y * k / (k + point.z);
    return Point(x_proj, y_proj);
}


void displayCuboid(const vector<Point3D>& points, float k) {

    Mat image(600, 800, CV_8UC3, Scalar(255, 255, 255));

    vector<Point> projectedPoints;
    for (const auto& point : points) {
        Point projPoint = projectPoint(point, k);
        projectedPoints.push_back(projPoint);
    }

    vector<pair<int, int>> edges = {
        {0, 1}, {0, 2}, {0, 4},
        {1, 3}, {1, 5},
        {2, 3}, {2, 6},
        {3, 7},
        {4, 5}, {4, 6},
        {5, 7},
        {6, 7}
    };

    for (const auto& edge : edges) {
        Line(image, projectedPoints[edge.first].x + 400, projectedPoints[edge.first].y + 300,
            projectedPoints[edge.second].x + 400, projectedPoints[edge.second].y + 300, 0xB4A7D6);
    }

    imshow("Perspective Projection of Cuboid", image);
    waitKey(0);
}


int main() {

    vector<Point3D> cuboid = {
        {100, 100, 100}, 
        {100, 100, -100}, 
        {100, -100, 100},
        {100, -100, -100},
        {-100, 100, 100}, 
        {-100, 100, -100},
        {-100, -100, 100}, 
        {-100, -100, -100} 
    };

    float k = 200.0f;

    displayCuboid(cuboid, k);

    return 0;
}
