#pragma once

struct Cuboid;
void medianCut(const vector<Vec3b>& colors, vector<Vec3b>& palette, int numColors);
Mat quantizeImage(const Mat& img, int numColors);