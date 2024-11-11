#include "library.h"
#include "rotation.h"


// функция для создания матрицы поворота
Mat createRotationMatrix(double angle, double x, double y, double z) {
    double n = sqrt(x * x + y * y + z * z);
    double nx = x / n;
    double ny = y / n;
    double nz = z / n;

    double cosAngle = cos(angle);
    double sinAngle = sin(angle);

    return (Mat_<double>(4, 4) <<
        cosAngle + nx * nx * (1 - cosAngle), nx * ny * (1 - cosAngle) + nz * sinAngle, nx * nz * (1 - cosAngle) - ny * sinAngle, 0,
        ny * nx * (1 - cosAngle) - nz * sinAngle, cosAngle + ny * ny * (1 - cosAngle), ny * nz * (1 - cosAngle) + nx * sinAngle, 0,
        nz * nx * (1 - cosAngle) + ny * sinAngle, nz * ny * (1 - cosAngle) - nx * sinAngle, cosAngle + nz * nz * (1 - cosAngle), 0,
        0, 0, 0, 1);
}
