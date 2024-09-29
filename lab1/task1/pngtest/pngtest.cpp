// pngtest.cpp : Defines the entry point for the console application.
//

#include "PngProc.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <png.h>


void HalftoneCircularImage(unsigned char* pOut,
	unsigned char* pMask,
	unsigned char* pIn,
	size_t nWidth1,
	size_t nHeight1,
	size_t nWidth2,
	size_t nHeight2,
	const char* OutputFileName
);


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

	size_t nRetSize1 = NPngProc::readPngFileGray(szInputFileName1, pInputBits1, &nWidth1, &nHeight1/*, &nBPP*/);
	size_t nRetSize2 = NPngProc::readPngFileGray(szInputFileName2, pInputBits2, &nWidth2, &nHeight2/*, &nBPP*/);
	nBPP = 8;

	HalftoneCircularImage(pOutputBits2, pInputBits1, pInputBits2, nWidth1, nHeight1, nWidth2, nHeight2, szOutputFileName);

	return 0;
}


// функция для сохранения изображения в файл

void WriteImageToFile(unsigned char* pImage, size_t Width, size_t Height, const char* OutputFileName) {

	FILE* file = fopen(OutputFileName, "wb");
	if (!file) {
		std::cerr << "Error opening file for writing" << std::endl;
		return;
	}

	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		std::cerr << "Error creating PNG structure" << std::endl;
		fclose(file);
		return;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		std::cerr << "Error creating PNG info structure" << std::endl;
		png_destroy_write_struct(&png_ptr, NULL);
		fclose(file);
		return;
	}

	png_set_IHDR(png_ptr, info_ptr, Width, Height, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	png_init_io(png_ptr, file);
	png_write_info(png_ptr, info_ptr);


	png_bytep* row_pointers = new png_bytep[Height];
	for (size_t y = 0; y < Height; ++y) {
		row_pointers[y] = (png_bytep)(pImage + y * Width * 4);
	}
	png_write_image(png_ptr, row_pointers);

	png_write_end(png_ptr, NULL);

	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(file);
}


// реализация получения круглого полутонового изображения

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
		// проверяем, что размеры маски и изображения совпадают
		if (nWidth1 != nWidth2 || nHeight1 != nHeight2) {
			throw std::invalid_argument("Mask and image sizes don't match");
		}

		// создаем новый буфер вывода с альфа-каналом (альфа-канал используется для определения прозрачности пикселя)
		unsigned char* pOut = new unsigned char[nWidth1 * nHeight1 * 4];

		// перебираем каждый пиксель исходного изображения
		for (size_t y = 0; y < nHeight1; ++y) {
			for (size_t x = 0; x < nWidth1; ++x) {

				size_t pixelIndex = y * nWidth1 + x;

				// проверяем, что пиксель находится внутри маски
				if (pMask[pixelIndex] > 0) {
					pOut[(y * nWidth1 + x) * 4] = pIn[pixelIndex]; // R
					pOut[(y * nWidth1 + x) * 4 + 1] = pIn[pixelIndex]; // G
					pOut[(y * nWidth1 + x) * 4 + 2] = pIn[pixelIndex]; // B
					pOut[(y * nWidth1 + x) * 4 + 3] = 255; // A
				}
				else {
					// устанавливаем пиксель прозрачным
					pOut[(y * nWidth1 + x) * 4] = 0; // R
					pOut[(y * nWidth1 + x) * 4 + 1] = 0; // G
					pOut[(y * nWidth1 + x) * 4 + 2] = 0; // B
					pOut[(y * nWidth1 + x) * 4 + 3] = 0; // A
				}
			}
		}

		// сохраняем в файл
		WriteImageToFile(pOut, nWidth1, nHeight1, OutputFileName);
	}
	catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
	return;
}
