#include "library.h"
#include "line.h"
#include "draw_projection.h"


// функция для построения проекций на плоскость z = 0
void drawProjection(Mat& img, const vector<Point3f>& parallelepipedVertices, const Mat& transformationMatrix) {
    vector<Point2f> projectedPoints;

    // применяем матрицу преобразования к каждой вершине параллелепипеда
    for (const auto& vertex : parallelepipedVertices) {
        Mat pointMat = (Mat_<double>(4, 1) << vertex.x, vertex.y, vertex.z, 1);
        Mat transformedPoint = transformationMatrix.t() * pointMat;

        // нормализуем координаты
        double H = transformedPoint.at<double>(3);
        Point2f projectedPoint(transformedPoint.at<double>(0) / H, transformedPoint.at<double>(1) / H);
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
