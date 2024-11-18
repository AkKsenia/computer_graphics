#include "library.h"
#include "line.h"
#include "task1.h"


int GetPixel(const Mat& img, int x, int y) {
    if (x >= 0 && x < img.cols && y >= 0 && y < img.rows) {
        Vec3b color = img.at<Vec3b>(y, x);
        return color[0] | (color[1] << 8) | (color[2] << 16);
    }
    return -1;
}


void floodFill8(Mat& img, int x, int y, int newColor, int oldColor) {
    if (oldColor == newColor) return; // ���� ���� ��� ���������

    stack<pair<int, int>> stack;
    stack.push({ x, y }); // �������� �������� � ����

    while (!stack.empty()) {
        auto point = stack.top();
        stack.pop();
        x = point.first;
        y = point.second;

        int Xleft = x;
        int Xright = x;

        // ��������� ������� �����
        while (Xleft >= 0 && GetPixel(img, Xleft, y) == oldColor) {
            SetPixel(img, Xleft, y, newColor & 0xFF, (newColor >> 8) & 0xFF, (newColor >> 16) & 0xFF);
            Xleft--;
        }
        // ��������� ������� ������
        while (Xright < img.cols && GetPixel(img, Xright, y) == oldColor) {
            SetPixel(img, Xright, y, newColor & 0xFF, (newColor >> 8) & 0xFF, (newColor >> 16) & 0xFF);
            Xright++;
        }

        // �������� ������ ���� (��� y ������������� ������ ����)
        if (y > 0)
            // ������ � ������, ��� ��������� ���� �������� Xleft--, ��� ��� �� ������� ������ Xleft ��� � ���� Xleft - 1 (���������� � Xright)
            for (int i = Xleft; i <= Xright; i++)
                if (GetPixel(img, i, y - 1) == oldColor) {
                    stack.push({ i, y - 1 });
                    break;
                }

        // �������� ������ ����
        if (y < img.rows - 1) 
            for (int i = Xleft; i <= Xright; i++)
                if (GetPixel(img, i, y + 1) == oldColor) {
                    stack.push({ i, y + 1 }); 
                    break;
                }
    }
}