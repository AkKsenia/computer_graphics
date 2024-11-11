#include "library.h"
#include "line.h"
#include "parallel.h"
#include "perspective.h"
#include "draw_projection.h"
#include "back_face_culling.h"
#include "rotation.h"


int main() {
    Mat img(600, 800, CV_8UC3, Scalar(255, 255, 255));

    vector<Point3f> parallelepipedVertices = {
        Point3f(100, 100, 100),
        Point3f(100, 100, -100),
        Point3f(100, -100, 100),
        Point3f(100, -100, -100),
        Point3f(-100, 100, 100),
        Point3f(-100, 100, -100),
        Point3f(-100, -100, 100),
        Point3f(-100, -100, -100)
    };

    // __TASK 1__

    // ортографическая проекция
    //Mat orthographicProjectionMatrix = orthographicProjection();
    //drawProjection(img, parallelepipedVertices, orthographicProjectionMatrix);

    // свободная проекция
    //Mat cavalierProjectionMatrix = obliqueProjection(cos(M_PI / 4), cos(M_PI / 4));
    //drawProjection(img, parallelepipedVertices, cavalierProjectionMatrix);

    // кабинетная проекция
    //Mat cabinetProjectionMatrix = obliqueProjection(0.5 * cos(M_PI / 4), 0.5 * cos(M_PI / 4));
    //drawProjection(img, parallelepipedVertices, cabinetProjectionMatrix);

    // изометричесая проекция
    //Mat isometricProjectionMatrix = axonometricProjection(45, 35.26);
    //drawProjection(img, parallelepipedVertices, isometricProjectionMatrix);

    // диаметрическая проекция
    //Mat diametricProjectionMatrix = axonometricProjection(90, 45);
    //drawProjection(img, parallelepipedVertices, diametricProjectionMatrix);

    // __TASK 2__

    //Mat perspectiveProjectionMatrix = perspectiveProjection(0.002);
    //drawProjection(img, parallelepipedVertices, perspectiveProjectionMatrix);

    // __TASK 3__

    // свободная проекция
    //Mat cavalierProjectionMatrix = obliqueProjection(cos(M_PI / 4), cos(M_PI / 4));
    //drawVisibleFaces(img, parallelepipedVertices, cavalierProjectionMatrix);

    // кабинетная проекция
    //Mat cabinetProjectionMatrix = obliqueProjection(0.5 * cos(M_PI / 4), 0.5 * cos(M_PI / 4));
    //drawVisibleFaces(img, parallelepipedVertices, cabinetProjectionMatrix);

    // __TASK 4__

    VideoWriter video("rotating_parallelepiped_perspective.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(800, 600));
    //VideoWriter video("rotating_parallelepiped_parallel.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(800, 600));
    double angle = 0.0f;
    double rotationAxisX = 1.0f, rotationAxisY = 1.0f, rotationAxisZ = 1.0f;

    while (true) {
        img.setTo(Scalar(255, 255, 255)); 
        Mat rotationMatrix = createRotationMatrix(angle, rotationAxisX, rotationAxisY, rotationAxisZ);
        Mat perspectiveProjectionMatrix = perspectiveProjection(0.002);
        drawProjection(img, parallelepipedVertices, rotationMatrix * perspectiveProjectionMatrix);
        //Mat cavalierProjectionMatrix = obliqueProjection(cos(M_PI / 4), cos(M_PI / 4));
        //drawProjection(img, parallelepipedVertices, rotationMatrix * cavalierProjectionMatrix);

        video.write(img); 
        imshow("Rotating Parallelepiped", img);
        angle += 0.01f; 
        if (waitKey(10) >= 0) break;
    }

    video.release();

    // отображаем изображение
    //imshow("Projection of Parallelepiped", img);
    //waitKey(0);

    //imwrite("cabinetProjection_visible_faces.png", img);

    return 0;
}