#include "library.h"
#include "perspective.h"


// функция для построения одноточечной перспективной проекции с центром проекции в точке [0, 0, k]
Mat perspectiveProjection(double k) {
    return (Mat_<double>(4, 4) <<
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, k,
        0, 0, 0, 1);
}