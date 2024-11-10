#define _USE_MATH_DEFINES
#include <iostream>
#include <opencv2/opencv.hpp>
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

// функция для ортографической проекции на плоскость z = 0
Mat orthographicProjection() {
    return (Mat_<float>(4, 4) <<
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 1);
}

// функция для аксонометрической проекции на плоскость z = 0
Mat axonometricProjection(float phi, float theta) {

    Mat first = (Mat_<float>(4, 4) <<
        cos(phi), 0, -sin(phi), 0,
        0, 1, 0, 0,
        sin(phi), 0, cos(phi), 0,
        0, 0, 0, 1);

    Mat second = (Mat_<float>(4, 4) <<
        1, 0, 0, 0,
        0, cos(theta), sin(theta), 0,
        0, -sin(theta), cos(theta), 0,
        0, 0, 0, 1);

    Mat third = (Mat_<float>(4, 4) <<
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 1);

    return  first * second * third;
}

// функция для наклонной проекции на плоскость z = 0
Mat obliqueProjection(float h, float i) {
    Mat first = (Mat_<float>(4, 4) <<
        1, 0, 0, 0,
        0, 1, 0, 0,
        h, i, 1, 0,
        0, 0, 0, 1);

    Mat second = (Mat_<float>(4, 4) <<
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 1);

    return  first * second;
}

// функция для построения одноточечной перспективной проекции с центром проекции в точке [0, 0, k]
Mat perspectiveProjection(float k) {
    return (Mat_<float>(4, 4) <<
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, k,
        0, 0, 0, 1);
}

// функция для построения проекций на плоскость z = 0
void drawProjection(Mat& img, const vector<Point3f>& parallelepipedVertices, const Mat& transformationMatrix) {
    vector<Point2f> projectedPoints;

    // применяем матрицу преобразования к каждой вершине параллелепипеда
    for (const auto& vertex : parallelepipedVertices) {
        Mat pointMat = (Mat_<float>(4, 1) << vertex.x, vertex.y, vertex.z, 1);
        Mat transformedPoint = transformationMatrix.t() * pointMat;

        // нормализуем координаты
        float H = transformedPoint.at<float>(3);
        Point2f projectedPoint(transformedPoint.at<float>(0) / H, transformedPoint.at<float>(1) / H);
        projectedPoints.push_back(projectedPoint);
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
        Line(img, projectedPoints[edge.first].x + 400, projectedPoints[edge.first].y + 300,
            projectedPoints[edge.second].x + 400, projectedPoints[edge.second].y + 300, 0xB4A7D6);
    }
}

// функция для удаления невидимых граней (ну и построения проекций)
void drawVisibleFaces(Mat& img, const vector<Point3f>& parallelepipedVertices, const Mat& transformationMatrix) {
    vector<Point2f> projectedPoints;

    // применяем матрицу преобразования к каждой вершине параллелепипеда
    for (const auto& vertex : parallelepipedVertices) {
        Mat pointMat = (Mat_<float>(4, 1) << vertex.x, vertex.y, vertex.z, 1);
        Mat transformedPoint = transformationMatrix.t() * pointMat;

        // нормализуем координаты
        float H = transformedPoint.at<float>(3);
        Point2f projectedPoint(transformedPoint.at<float>(0) / H, transformedPoint.at<float>(1) / H);
        projectedPoints.push_back(projectedPoint);
    }

    // определяем грани параллелепипеда 
    // для каждой грани нормаль направляется внутрь тела, тогда по правилу правой руки получаем 4-ки вершин:
    vector<vector<int>> faces = {
        {2, 3, 7, 6},
        {0, 4, 5, 1},
        {4, 6, 7, 5},
        {0, 1, 3, 2},
        {1, 5, 7, 3},
        {0, 2, 6, 4},
    };

    // вектор направления наблюдения
    Vec3f v(0, 0, -1);

    for (const auto& face : faces) {
        // вычисляем нормаль к грани
        Vec3f ab = Vec3f(projectedPoints[face[1]].x - projectedPoints[face[0]].x,
            projectedPoints[face[1]].y - projectedPoints[face[0]].y, 0);
        Vec3f ac = Vec3f(projectedPoints[face[2]].x - projectedPoints[face[0]].x,
            projectedPoints[face[2]].y - projectedPoints[face[0]].y, 0);
        Vec3f normal = ab.cross(ac);
        normal = normal / norm(normal);

        // проверяем видимость грани
        if (normal.dot(v) > 0) {
            // если грань видима, рисуем ее
            for (size_t i = 0; i < face.size(); ++i) {
                Line(img, projectedPoints[face[i]].x + 400, projectedPoints[face[i]].y + 300,
                    projectedPoints[face[(i + 1) % face.size()]].x + 400, projectedPoints[face[(i + 1) % face.size()]].y + 300, 0xB4A7D6);
            }
        }
    }
}


int main() {
    Mat img(600, 800, CV_8UC3, Scalar(255, 255, 255));

    vector<Point3f> parallelepipedVertices = {
        Point3f(100, 100, 100),
        Point3f(100, 100, -100),
        Point3f(100, -100, 100),
        Point3f(100, -100, -100),
        Point3f(-100, 100, 100),
        Point3f(-100, 100, -100),
        Point3f(-100, -100, 100),
        Point3f(-100, -100, -100)
    };

    // __TASK 1__

    // ортографическая проекция
    //Mat orthographicProjectionMatrix = orthographicProjection();
    //drawProjection(img, parallelepipedVertices, orthographicProjectionMatrix);

    // свободная проекция
    //Mat cavalierProjectionMatrix = obliqueProjection(cos(M_PI / 4), cos(M_PI / 4));
    //drawProjection(img, parallelepipedVertices, cavalierProjectionMatrix);

    // кабинетная проекция
    //Mat cabinetProjectionMatrix = obliqueProjection(0.5 * cos(M_PI / 4), 0.5 * cos(M_PI / 4));
    //drawProjection(img, parallelepipedVertices, cabinetProjectionMatrix);

    // изометричесая проекция
    //Mat isometricProjectionMatrix = axonometricProjection(45, 35.26);
    //drawProjection(img, parallelepipedVertices, isometricProjectionMatrix);

    // диаметрическая проекция
    //Mat diametricProjectionMatrix = axonometricProjection(90, 45);
    //drawProjection(img, parallelepipedVertices, diametricProjectionMatrix);

    // __TASK 2__

    //Mat perspectiveProjectionMatrix = perspectiveProjection(0.002);
    //drawProjection(img, parallelepipedVertices, perspectiveProjectionMatrix);

    // __TASK 3__

    // свободная проекция
    //Mat cavalierProjectionMatrix = obliqueProjection(cos(M_PI / 4), cos(M_PI / 4));
    //drawVisibleFaces(img, parallelepipedVertices, cavalierProjectionMatrix);

    // кабинетная проекция
    //Mat cabinetProjectionMatrix = obliqueProjection(0.5 * cos(M_PI / 4), 0.5 * cos(M_PI / 4));
    //drawVisibleFaces(img, parallelepipedVertices, cabinetProjectionMatrix);

    // __TASK 4__


    // отображаем изображение
    imshow("Projection of Parallelepiped", img);
    waitKey(0);

    imwrite("cabinetProjection_visible_faces.png", img);

    return 0;
}