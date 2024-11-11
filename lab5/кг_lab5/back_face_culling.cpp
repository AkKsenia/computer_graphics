#include "library.h"
#include "line.h"
#include "back_face_culling.h"


// функция для удаления невидимых ребер (после построения проекций)
void drawVisibleFaces(Mat& img, const vector<Point3f>& parallelepipedVertices, const Mat& transformationMatrix) {
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

    // алгоритм back face culling:

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
