#include "library.h"
#include "floyd_steinberg.h"


// implementation of the transformation of an 8 bpp grayscale image into n bpp (n < 8) using the Floyd-Stenberg algorithm
void floyd_steinberg_dithering_grayscale(Mat& img, int n) {

    Mat out_img = Mat::zeros(img.size(), CV_8U);

    // iterate over each pixel in the image
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {

            // calculate the error between the original pixel value and the quantized value
            uchar pixel = img.at<uchar>(y, x);

            // quantize the pixel, reducing the number of bits
            uchar quantized_pixel = pixel >> (8 - n) << (8 - n);
            int error = pixel - quantized_pixel;

            if (x < img.cols - 1) {
                // add the error to the pixel to the right
                img.at<uchar>(y, x + 1) = std::min(255, std::max(0, img.at<uchar>(y, x + 1) + error * 7 / 16));
            }
            if (y < img.rows - 1) {
                // add the error to the pixel below
                img.at<uchar>(y + 1, x) = std::min(255, std::max(0, img.at<uchar>(y + 1, x) + error * 5 / 16));
                if (x < img.cols - 1) {
                    // add the error to the pixel below and to the right
                    img.at<uchar>(y + 1, x + 1) = std::min(255, std::max(0, img.at<uchar>(y + 1, x + 1) + error * 1 / 16));
                }
                if (x > 0) {
                    // add the error to the pixel below and to the left
                    img.at<uchar>(y + 1, x - 1) = std::min(255, std::max(0, img.at<uchar>(y + 1, x - 1) + error * 3 / 16));
                }
            }

            // quantize the pixel value
            out_img.at<uchar>(y, x) = quantized_pixel;
        }
    }

    img = out_img;
}


// implementation of the transformation of an 8 bpp rgb image into n bpp (n < 8) using the Floyd-Stenberg algorithm
void floyd_steinberg_dithering_color(Mat& img, int n) {

    Mat out_img = Mat::zeros(img.size(), img.type());

    // iterate over each pixel in the image
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {

            // calculate the error between the original pixel value and the quantized value
            Vec3b pixel = img.at<Vec3b>(y, x);

            // quantize the pixel, reducing the number of bits
            Vec3b quantized_pixel;
            for (int i = 0; i < 3; i++) {
                quantized_pixel[i] = pixel[i] >> (8 - n) << (8 - n);
            }

            Vec3b error;
            for (int i = 0; i < 3; i++) {
                error[i] = pixel[i] - quantized_pixel[i];
            }

            if (x < img.cols - 1) {
                // add the error to the pixel to the right
                Vec3b& right_pixel = img.at<Vec3b>(y, x + 1);
                for (int i = 0; i < 3; i++) {
                    right_pixel[i] = std::min(255, std::max(0, right_pixel[i] + error[i] * 7 / 16));
                }
            }
            if (y < img.rows - 1) {
                // add the error to the pixel below
                Vec3b& below_pixel = img.at<Vec3b>(y + 1, x);
                for (int i = 0; i < 3; i++) {
                    below_pixel[i] = std::min(255, std::max(0, below_pixel[i] + error[i] * 5 / 16));
                }
                if (x < img.cols - 1) {
                    // add the error to the pixel below and to the right
                    Vec3b& below_right_pixel = img.at<Vec3b>(y + 1, x + 1);
                    for (int i = 0; i < 3; i++) {
                        below_right_pixel[i] = std::min(255, std::max(0, below_right_pixel[i] + error[i] * 1 / 16));
                    }
                }
                if (x > 0) {
                    // add the error to the pixel below and to the left
                    Vec3b& below_left_pixel = img.at<Vec3b>(y + 1, x - 1);
                    for (int i = 0; i < 3; i++) {
                        below_left_pixel[i] = std::min(255, std::max(0, below_left_pixel[i] + error[i] * 3 / 16));
                    }
                }
            }

            // quantize the pixel value
            out_img.at<Vec3b>(y, x) = quantized_pixel;
        }
    }

    img = out_img;
}
