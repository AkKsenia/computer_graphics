#include "library.h"
#include "parallel.h"


// функция для ортографической проекции на плоскость z = 0
Mat orthographicProjection() {
    return (Mat_<double>(4, 4) <<
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 1);
}

// функция для аксонометрической проекции на плоскость z = 0
Mat axonometricProjection(double phi, double theta) {

    Mat first = (Mat_<double>(4, 4) <<
        cos(phi), 0, -sin(phi), 0,
        0, 1, 0, 0,
        sin(phi), 0, cos(phi), 0,
        0, 0, 0, 1);

    Mat second = (Mat_<double>(4, 4) <<
        1, 0, 0, 0,
        0, cos(theta), sin(theta), 0,
        0, -sin(theta), cos(theta), 0,
        0, 0, 0, 1);

    Mat third = (Mat_<double>(4, 4) <<
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 1);

    return  first * second * third;
}

// функция для наклонной проекции на плоскость z = 0
Mat obliqueProjection(double h, double i) {
    Mat first = (Mat_<double>(4, 4) <<
        1, 0, 0, 0,
        0, 1, 0, 0,
        h, i, 1, 0,
        0, 0, 0, 1);

    Mat second = (Mat_<double>(4, 4) <<
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 1);

    return  first * second;
}