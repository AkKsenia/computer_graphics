#include "library.h"
#include "set_pixel.h"


void SetPixel(Mat& img, int x, int y, int r, int g, int b) {
	if (x >= 0 && x < img.cols && y >= 0 && y < img.rows)
		img.at<Vec3b>(y, x) = Vec3b(r, g, b);
}