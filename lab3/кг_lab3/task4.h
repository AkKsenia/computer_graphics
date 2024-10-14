#pragma once

bool isInsideEvenOdd(const vector<Point>& points, const Point& p);
bool isInsideNonZeroWinding(const vector<Point>& points, const Point& p);
void FillPolygon(Mat& img, const vector<Point>& points, int color, bool useEvenOddRule);