#pragma once

Point CatmullRomPoint(Point P0, Point P1, Point P2, Point P3, double t);
double Dist(Point P0, Point P1);
double MaxDist(Point P0, Point P1, Point P2, Point P3);
void CatmullRom(Mat& img, Point& P0, Point& P1, Point& P2, Point& P3);
void CompositeCatmullRom(Mat& img, vector<Point>& controlPoints);