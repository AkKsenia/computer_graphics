#pragma once

double getAngle(Point p1, Point p2, Point p3);
bool isConvexPolygon(vector<Point> points);
bool doIntersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
bool isSimplePolygon(vector<Point> points);