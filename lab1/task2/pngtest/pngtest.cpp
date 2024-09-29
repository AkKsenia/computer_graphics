// pngtest.cpp : Defines the entry point for the console application.
//

#include "PngProc.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdexcept>


// blending modes

enum BlendMode {
    Normal,
    Multiply,
    Screen,
    Darken,
    Lighten,
    Difference,
    ColorDodge,
    ColorBurn,
    SoftLight
};


void BlendImages(unsigned char* pOut,
    unsigned char* pImage1,
    unsigned char* pImage2,
    unsigned char* pAlpha,
    size_t nWidth1,
    size_t nHeight1,
    size_t nWidth2,
    size_t nHeight2,
    size_t nWidth3,
    size_t nHeight3,
    BlendMode mode,
    const char* OutputFileName);


int main(int argc, char* argv[])
{
	class CBitsPtrGuard
	{
	public:
		CBitsPtrGuard(unsigned char** pB) : m_ppBits(pB) { }
		~CBitsPtrGuard() { if (*m_ppBits) delete *m_ppBits, *m_ppBits = 0; }
	protected:
		unsigned char** m_ppBits;
	};

	// parse input parameters
	char	szInputFileName1[256];
    char	szInputFileName2[256];
    char	szInputFileName3[256];
	char    szOutputFileName[256];
	if (argc < 4)
		printf("\nformat: pngtest <input_file1> <input_file2> <input_file3> [<output_file>]");
	else 
	{
		strcpy(szInputFileName1, argv[1]);
        strcpy(szInputFileName2, argv[2]);
        strcpy(szInputFileName3, argv[3]);
		if (argc > 4)
			strcpy(szOutputFileName, argv[4]);
		else
		{
			strcpy(szOutputFileName, szInputFileName1);
			strcat(szOutputFileName, "_out.png");
		}
	}


    size_t nReqSize1 = NPngProc::readPngFile(szInputFileName1, 0, 0, 0, 0);
    size_t nReqSize2 = NPngProc::readPngFile(szInputFileName2, 0, 0, 0, 0);
    size_t nReqSize3 = NPngProc::readPngFile(szInputFileName3, 0, 0, 0, 0);
    if (nReqSize1 == NPngProc::PNG_ERROR || nReqSize2 == NPngProc::PNG_ERROR || nReqSize3 == NPngProc::PNG_ERROR)
    {
        printf("\nError occurred while png file was read");
        return -1;
    }


    unsigned char* pInputBits1 = new unsigned char[nReqSize1];
    if (!pInputBits1)
    {
        printf("\nCan't allocate memory for image, required size is %u", nReqSize1);
        return -1;
    }
    CBitsPtrGuard InputBitsPtrGuard1(&pInputBits1);

    unsigned char* pInputBits2 = new unsigned char[nReqSize2];
    if (!pInputBits2)
    {
        printf("\nCan't allocate memory for image, required size is %u", nReqSize2);
        return -1;
    }
    CBitsPtrGuard InputBitsPtrGuard2(&pInputBits2);

    unsigned char* pInputBits3 = new unsigned char[nReqSize3];
    if (!pInputBits3)
    {
        printf("\nCan't allocate memory for image, required size is %u", nReqSize3);
        return -1;
    }
    CBitsPtrGuard InputBitsPtrGuard3(&pInputBits3);

    unsigned char* pOutputBits = new unsigned char[nReqSize1];
    if (!pOutputBits)
    {
        printf("\nCan't allocate memory for image, required size is %u", nReqSize1);
        return -1;
    }
    CBitsPtrGuard OutputBitsPtrGuard(&pOutputBits);

    size_t nWidth1, nHeight1, nWidth2, nHeight2, nWidth3, nHeight3;
    unsigned int nBPP;
    nBPP = 8;

    size_t nRetSize1 = NPngProc::readPngFileGray(szInputFileName1, pInputBits1, &nWidth1, &nHeight1);
    size_t nRetSize2 = NPngProc::readPngFileGray(szInputFileName2, pInputBits2, &nWidth2, &nHeight2);
    size_t nRetSize3 = NPngProc::readPngFileGray(szInputFileName3, pInputBits3, &nWidth3, &nHeight3);
    

    BlendImages(pOutputBits, pInputBits1, pInputBits2, pInputBits3, nWidth1, nHeight1, nWidth2, nHeight2, nWidth3, nHeight3, Normal, szOutputFileName);

    return 0;
}


// implementation of mixing 8bpp images

void BlendImages(unsigned char* pOut,
    unsigned char* pImage1,
    unsigned char* pImage2,
    unsigned char* pAlpha,
    size_t nWidth1,
    size_t nHeight1,
    size_t nWidth2,
    size_t nHeight2,
    size_t nWidth3,
    size_t nHeight3,
    BlendMode mode,
    const char* OutputFileName) 
{
    try {
        // we check that the dimensions match
        if (nWidth1 != nWidth2 || nWidth1 != nWidth3 || nHeight1 != nHeight2 || nHeight1 != nHeight3) {
            throw std::invalid_argument("Images have different sizes");
        }

        // iterate over each pixel of the original image
        for (size_t y = 0; y < nHeight1; ++y) {
            for (size_t x = 0; x < nWidth1; ++x) {
                size_t pixelIndex = y * nWidth1 + x;

                unsigned char alpha_s = pAlpha[pixelIndex];
                unsigned char alpha_b = 255 - alpha_s;

                unsigned char C_b = pImage1[pixelIndex];
                unsigned char C_s = pImage2[pixelIndex];

                unsigned char B;

                switch (mode) {
                case Normal:
                    B = C_s;
                    break;
                case Multiply:
                    B = (unsigned char)(C_b * C_s / 255);
                    break;
                case Screen:
                    B = (unsigned char)(255 * (1 - (1 - C_b / 255) * (1 - C_s / 255)));
                    break;
                case Darken:
                    B = (unsigned char)(std::min(C_b, C_s));
                    break;
                case Lighten:
                    B = (unsigned char)(std::max(C_b, C_s));
                    break;
                case Difference:
                    B = (unsigned char)(std::abs(C_b - C_s));
                    break;
                case ColorDodge:
                    if (C_s < 255) {
                        B = (unsigned char)(std::min(255, 255 * C_b / (255 - C_s)));
                    }
                    else {
                        B = 255;
                    }
                    break;
                case ColorBurn:
                    if (C_s > 0) {
                        B = (unsigned char)(255 - std::min(255, 255 * (255 - C_b) / C_s));
                    }
                    else {
                        B = 0;
                    }
                    break;
                case SoftLight:
                    if (C_s < 128) {
                        B = (unsigned char)(C_b - (1 - 2 * C_s / 255) * C_b * (1 - C_b / 255));
                    }
                    else {
                        unsigned char D = (C_b <= 64) ? (unsigned char)((16 * C_b - 12) * C_b + 4) * C_b : (unsigned char)(sqrt(C_b));
                        B = (unsigned char)(C_b + (2 * C_s / 255 - 1) * (D - C_b / 255) * 255);
                    }
                    break;
                }

                pOut[pixelIndex] = (unsigned char)((1 - alpha_s / 255.0) * (alpha_b / 255.0) * C_b + (1 - alpha_b / 255.0) * (alpha_s / 255.0) * C_s + (alpha_s / 255.0) * (alpha_b / 255.0) * B);
            }
        }
        // save it to a file
        NPngProc::WriteImageToFile(pOut, nWidth1, nHeight1, OutputFileName);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
    return;
}
