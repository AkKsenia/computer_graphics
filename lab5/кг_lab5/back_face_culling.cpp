#include "library.h"
#include "line.h"
#include "back_face_culling.h"


// ������� ��� �������� ��������� ����� (����� ���������� ��������)
void drawVisibleFaces(Mat& img, const vector<Point3f>& parallelepipedVertices, const Mat& transformationMatrix) {
    vector<Point2f> projectedPoints;

    // ��������� ������� �������������� � ������ ������� ���������������
    for (const auto& vertex : parallelepipedVertices) {
        Mat pointMat = (Mat_<double>(4, 1) << vertex.x, vertex.y, vertex.z, 1);
        Mat transformedPoint = transformationMatrix.t() * pointMat;

        // ����������� ����������
        double H = transformedPoint.at<double>(3);
        Point2f projectedPoint(transformedPoint.at<double>(0) / H, transformedPoint.at<double>(1) / H);
        projectedPoints.push_back(projectedPoint);
    }

    // ���������� ����� ��������������� 
    // ��� ������ ����� ������� ������������ ������ ����, ����� �� ������� ������ ���� �������� 4-�� ������:
    vector<vector<int>> faces = {
        {2, 3, 7, 6},
        {0, 4, 5, 1},
        {4, 6, 7, 5},
        {0, 1, 3, 2},
        {1, 5, 7, 3},
        {0, 2, 6, 4},
    };

    // �������� back face culling:

    // ������ ����������� ����������
    Vec3f v(0, 0, -1);

    for (const auto& face : faces) {
        // ��������� ������� � �����
        Vec3f ab = Vec3f(projectedPoints[face[1]].x - projectedPoints[face[0]].x,
            projectedPoints[face[1]].y - projectedPoints[face[0]].y, 0);
        Vec3f ac = Vec3f(projectedPoints[face[2]].x - projectedPoints[face[0]].x,
            projectedPoints[face[2]].y - projectedPoints[face[0]].y, 0);
        Vec3f normal = ab.cross(ac);
        normal = normal / norm(normal);

        // ��������� ��������� �����
        if (normal.dot(v) > 0) {
            // ���� ����� ������, ������ ��
            for (size_t i = 0; i < face.size(); ++i) {
                Line(img, projectedPoints[face[i]].x + 400, projectedPoints[face[i]].y + 300,
                    projectedPoints[face[(i + 1) % face.size()]].x + 400, projectedPoints[face[(i + 1) % face.size()]].y + 300, 0xB4A7D6);
            }
        }
    }
}