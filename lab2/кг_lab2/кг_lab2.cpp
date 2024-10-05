#include "library.h"
#include "floyd_steinberg.h"
#include "stucki.h"


int main() {
    std::string inputFileName = "kitten";
    Mat img = imread(inputFileName + ".png", IMREAD_GRAYSCALE);
    //Mat img = imread(inputFileName + ".png");

    int n = 4;
    //floyd_steinberg_dithering_grayscale(img, n);
    //floyd_steinberg_dithering_color(img, n);
    //stucki_dithering_color(img, n);
    stucki_dithering_grayscale(img, n);

    //std::string outputFileName = inputFileName + "_fs_gray_" + std::to_string(n) + ".png";
    //std::string outputFileName = inputFileName + "_fs_color_" + std::to_string(n) + ".png";
    //std::string outputFileName = inputFileName + "_stucki_color_" + std::to_string(n) + ".png";
    std::string outputFileName = inputFileName + "_stucki_gray_" + std::to_string(n) + ".png";
    imwrite(outputFileName, img);

    return 0;
}