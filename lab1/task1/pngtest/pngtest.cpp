// pngtest.cpp : Defines the entry point for the console application.
//

#include "PngProc.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdexcept>


void HalftoneCircularImage(unsigned char* pOut,
	unsigned char* pMask,
	unsigned char* pIn,
	size_t nWidth1,
	size_t nHeight1,
	size_t nWidth2,
	size_t nHeight2,
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
	char    szOutputFileName[256];
	if (argc < 3)
		printf("\nformat: pngtest <input_file1>  <input_file2> [<output_file>]");
	else 
	{
		strcpy(szInputFileName1, argv[1]);
		strcpy(szInputFileName2, argv[2]);
		if (argc > 3)
			strcpy(szOutputFileName, argv[3]);
		else
		{
			strcpy(szOutputFileName, szInputFileName2);
			strcat(szOutputFileName, "_out.png");
		}
	}


	size_t nReqSize1 = NPngProc::readPngFile(szInputFileName1, 0, 0, 0, 0);
	size_t nReqSize2 = NPngProc::readPngFile(szInputFileName2, 0, 0, 0, 0);
	if (nReqSize1 == NPngProc::PNG_ERROR || nReqSize2 == NPngProc::PNG_ERROR)
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

	unsigned char* pOutputBits2 = new unsigned char[nReqSize2];
	if (!pOutputBits2)
	{
		printf("\nCan't allocate memory for image, required size is %u", nReqSize2);
		return -1;
	}
	CBitsPtrGuard OutputBitsPtrGuard(&pOutputBits2);

	size_t nWidth1, nHeight1, nWidth2, nHeight2;
	unsigned int nBPP;
	nBPP = 32;

	size_t nRetSize1 = NPngProc::readPngFileGray(szInputFileName1, pInputBits1, &nWidth1, &nHeight1);
	size_t nRetSize2 = NPngProc::readPngFileGray(szInputFileName2, pInputBits2, &nWidth2, &nHeight2);

	HalftoneCircularImage(pOutputBits2, pInputBits1, pInputBits2, nWidth1, nHeight1, nWidth2, nHeight2, szOutputFileName);

	return 0;
}


// the implementation of obtaining a round halftone image

void HalftoneCircularImage(unsigned char* pOut,
	unsigned char* pMask,
	unsigned char* pIn,
	size_t nWidth1,
	size_t nHeight1,
	size_t nWidth2,
	size_t nHeight2,
	const char* OutputFileName)
{
	try {
		// we check that the dimensions of the mask and the image match
		if (nWidth1 != nWidth2 || nHeight1 != nHeight2) {
			throw std::invalid_argument("Mask and image sizes don't match");
		}

		// creating a new output buffer with an alpha channel (the alpha channel is used to determine the transparency of the pixel)
		unsigned char* pOut = new unsigned char[nWidth1 * nHeight1 * 4];

		// iterate over each pixel of the original image
		for (size_t y = 0; y < nHeight1; ++y) {
			for (size_t x = 0; x < nWidth1; ++x) {

				size_t pixelIndex = y * nWidth1 + x;

				// we check that the pixel is inside the mask
				if (pMask[pixelIndex] > 0) {
					pOut[(y * nWidth1 + x) * 4] = pIn[pixelIndex]; // R
					pOut[(y * nWidth1 + x) * 4 + 1] = pIn[pixelIndex]; // G
					pOut[(y * nWidth1 + x) * 4 + 2] = pIn[pixelIndex]; // B
					pOut[(y * nWidth1 + x) * 4 + 3] = 255; // A
				}
				else {
					// setting the pixel to transparent
					pOut[(y * nWidth1 + x) * 4] = 0; // R
					pOut[(y * nWidth1 + x) * 4 + 1] = 0; // G
					pOut[(y * nWidth1 + x) * 4 + 2] = 0; // B
					pOut[(y * nWidth1 + x) * 4 + 3] = 0; // A
				}
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
