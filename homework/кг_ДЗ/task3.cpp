#include "library.h"
#include "task3.h"


struct Cuboid {
    Vec3b min; // минимум по RGB
    Vec3b max; // максимум по RGB
    vector<Vec3b> colors; // цвета в кубе

    Cuboid(const vector<Vec3b>& points) {
        if (points.empty()) return;
        min = Vec3b(255, 255, 255);
        max = Vec3b(0, 0, 0);
        colors = points;

        for (const auto& color : colors) {
            min[0] = std::min(min[0], color[0]);
            min[1] = std::min(min[1], color[1]);
            min[2] = std::min(min[2], color[2]);
            max[0] = std::max(max[0], color[0]);
            max[1] = std::max(max[1], color[1]);
            max[2] = std::max(max[2], color[2]);
        }
    }

    int longestDimension() {
        int lengthX = max[0] - min[0];
        int lengthY = max[1] - min[1];
        int lengthZ = max[2] - min[2];
        if (lengthX >= lengthY && lengthX >= lengthZ) return 0;
        if (lengthY >= lengthX && lengthY >= lengthZ) return 1;
        return 2;
    }

    int median(int dim) {
        vector<int> values;
        for (const auto& color : colors)
            values.push_back(color[dim]);

        size_t n = values.size();

        nth_element(values.begin(), values.begin() + n / 2, values.end());
        return values[n / 2];
    }
};


void medianCut(const vector<Vec3b>& colors, vector<Vec3b>& palette, int numColors) {
    queue<Cuboid> cuboids;
    cuboids.push(Cuboid(colors));

    // выполняем для новых кубов до тех пор, пока их число не станет >= необходимого количества цветов
    while (cuboids.size() < numColors) {
        Cuboid current = cuboids.front();
        cuboids.pop();

        // делим куб на два новых:
        int dim = current.longestDimension();
        int medianValue = current.median(dim);

        vector<Vec3b> leftColors, rightColors;

        for (const auto& color : current.colors) {
            if (color[dim] < medianValue)
                leftColors.push_back(color);
            else
                rightColors.push_back(color);
        }

        if (!leftColors.empty())
            cuboids.push(Cuboid(leftColors));
        if (!rightColors.empty())
            cuboids.push(Cuboid(rightColors));
    }

    // составляем итоговую палитру из цветов, равных средним арифметическим точек внутри кубов
    while (!cuboids.empty()) {
        Cuboid cuboid = cuboids.front();
        cuboids.pop();

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
