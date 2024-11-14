#include "library.h"
#include "line.h"
#include "task1.h"
#include "task2.h"


struct Cuboid {
    Vec3d min; // минимум по RGB
    Vec3d max; // максимум по RGB
    vector<Vec3b> colors; // цвета в кубе

    Cuboid(const vector<Vec3b>& points) {
        if (points.empty()) return;
        min = Vec3d(255, 255, 255);
        max = Vec3d(0, 0, 0);
        colors = points;

        for (const auto& color : colors) {
            min[0] = std::min(min[0], (double)color[0]);
            min[1] = std::min(min[1], (double)color[1]);
            min[2] = std::min(min[2], (double)color[2]);
            max[0] = std::max(max[0], (double)color[0]);
            max[1] = std::max(max[1], (double)color[1]);
            max[2] = std::max(max[2], (double)color[2]);
        }
    }

    int longestDimension() {
        double lengthX = max[0] - min[0];
        double lengthY = max[1] - min[1];
        double lengthZ = max[2] - min[2];
        if (lengthX >= lengthY && lengthX >= lengthZ) return 0;
        if (lengthY >= lengthX && lengthY >= lengthZ) return 1;
        return 2;
    }

    double median(int dim) {
        vector<double> values;
        for (const auto& color : colors)
            values.push_back(color[dim]);
        sort(values.begin(), values.end());//

        size_t n = values.size();
        if (n % 2 == 0)
            return (values[n / 2 - 1] + values[n / 2]) / 2.0;
        else
            return values[n / 2];
    }
};


void medianCut(const vector<Vec3b>& colors, vector<Vec3b>& palette, int numColors) {
    vector<Cuboid> cuboids;
    cuboids.emplace_back(colors);

    while (cuboids.size() < numColors) {
        Cuboid current = cuboids.back();
        cuboids.pop_back();

        int dim = current.longestDimension();
        double medianValue = current.median(dim);

        vector<Vec3b> leftColors, rightColors;

        for (const auto& color : current.colors)
            if (color[dim] < medianValue) 
                leftColors.push_back(color);
            else 
                rightColors.push_back(color);

        if (!leftColors.empty()) 
            cuboids.emplace_back(leftColors);
        if (!rightColors.empty())
            cuboids.emplace_back(rightColors);
    }

    for (const auto& cuboid : cuboids) {
        Vec3d averageColor(0, 0, 0);
        for (const auto& color : cuboid.colors)
            averageColor += Vec3d(color[0], color[1], color[2]);

        averageColor[0] /= cuboid.colors.size();
        averageColor[1] /= cuboid.colors.size();
        averageColor[2] /= cuboid.colors.size();

        palette.push_back(Vec3b((uchar)averageColor[0], (uchar)averageColor[1], (uchar)averageColor[2]));
    }
}


Mat quantizeImage(const Mat& img, int numColors) {
    vector<Vec3b> colors;
    for (int y = 0; y < img.rows; ++y)
        for (int x = 0; x < img.cols; ++x)
            colors.push_back(img.at<Vec3b>(y, x));

    vector<Vec3b> palette;
    medianCut(colors, palette, numColors);

    Mat output_img = Mat::zeros(img.size(), img.type());
    for (int y = 0; y < img.rows; ++y) {
        for (int x = 0; x < img.cols; ++x) {
            Vec3b color = img.at<Vec3b>(y, x);
            // находим ближайший цвет в палитре
            int closestIndex = 0;
            double closestDistance = numeric_limits<double>::max();

            for (size_t i = 0; i < palette.size(); ++i) {
                double distance = norm(color - palette[i]);
                if (distance < closestDistance) {
                    closestDistance = distance;
                    closestIndex = i;
                }
            }

            output_img.at<Vec3b>(y, x) = palette[closestIndex];
        }
    }
    return output_img;
}


int main() {
    //Mat img(400, 400, CV_8UC3, Scalar(255, 255, 255));
    //Mat img(800, 1300, CV_8UC3, Scalar(255, 255, 255));

    // __TASK 1__

    // смайлик
    /*
    circle(img, Point(200, 200), 100, Scalar(132, 234, 253), -1);
    circle(img, Point(160, 180), 20, Scalar(102, 102, 102), -1);
    circle(img, Point(240, 180), 20, Scalar(102, 102, 102), -1);
    ellipse(img, Point(200, 220), Size(55, 30), 0, 0, 180, Scalar(102, 102, 102), 2);

    int oldColor = 0xFDEA84;
    int newColor = 0xD9D2E9;
    */

    /*
    ellipse(img, Point(200, 200), Size(100, 100), 0, 0, 360, Scalar(102, 102, 102), 2);
    ellipse(img, Point(160, 180), Size(20, 20), 0, 0, 360, Scalar(102, 102, 102), 2);
    ellipse(img, Point(240, 180), Size(20, 20), 0, 0, 360, Scalar(102, 102, 102), 2);
    ellipse(img, Point(200, 220), Size(55, 30), 0, 0, 180, Scalar(102, 102, 102), 2);

    int oldColor = 0xFFFFFF;
    int newColor = 0xD9D2E9;
    */
    
    /*
    for (int y = 0; y < img.rows; ++y)
        for (int x = 0; x < img.cols; ++x)
            if (GetPixel(img, x, y) == oldColor)
                floodFill8(img, x, y, newColor, oldColor);
    */

    // __TASK 2__

    /*
    vector<Point> controlPoints = {
        Point(100, 500),
        Point(200, 100),
        Point(400, 100),
        Point(500, 400),
        Point(600, 700),
        Point(700, 100),
        Point(800, 100),
        Point(900, 100),
        Point(1000, 200),
        Point(1200, 150)
    };

    CompositeCatmullRom(img, controlPoints);
    */

    // __TASK 3__

    Mat img = imread("peaches.png");
    int numColors = 16;
    Mat output_img = quantizeImage(img, numColors);

    //imshow("Flood Fill", img);
    //imshow("Composite Catmull-Rom curve", img);
    imshow("Quantized Image", output_img);
    waitKey(0);
    //imwrite("before_Flood_Fill1.png", img);
    //imwrite("Composite_CatmullRom_curve.png", img);
    imwrite("peaches_quantized_16.png", output_img);
    return 0;
}
