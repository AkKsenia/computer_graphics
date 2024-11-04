#pragma once

Point BezierPoint(Point P0, Point P1, Point P2, Point P3, double t);
float Dist(Point P0, Point P1);
float MaxDist(Point P0, Point P1, Point P2, Point P3);
void BezierCurve(Mat& img, Point& P0, Point& P1, Point& P2, Point& P3);