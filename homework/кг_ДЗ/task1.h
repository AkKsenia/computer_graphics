#pragma once

int GetPixel(const Mat& img, int x, int y);
void floodFill8(Mat& img, int x, int y, int newColor, int oldColor);