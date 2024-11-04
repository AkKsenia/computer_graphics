#pragma once

float MyDist(Point P0, Point P1);
bool CheckSmoothness(Point& P2, Point& P3, Point& Q1);
void CompositeBezierCurve(Mat& img, vector<Point>& controlPoints);