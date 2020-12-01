#include "pch.h"
#include "ImageEnhancement.h"
ImageEnhancement::ImageEnhancement()
{
	pProcessImg = new CDib();
}
ImageEnhancement::ImageEnhancement(CDib* pDib)
{
	pProcessImg = new CDib(*pDib);
}
ImageEnhancement::~ImageEnhancement()
{
	if (pProcessImg)
	{
		delete pProcessImg;
		pProcessImg = nullptr;
	}
}
//=============================================
//函数功能：线性灰度增强
//输入参数：输入图像CDib* lpDib,图像数组指针 CDib** m_pAllImages
//返回值：无
//==============================================
void ImageEnhancement::LinearEnhancement(CDib* lpDib, CDib** m_pAllImages)
{
	*pProcessImg = *lpDib;
	LPBYTE tempData, lpSrcData;
	lpSrcData = pProcessImg->GetData();
	LONG lHeight, lWidth, lLineBytes;
	lLineBytes = pProcessImg->GetLineByte();
	LONG depth = pProcessImg->GetDepth();
	lHeight = pProcessImg->GetHeight();
	lWidth = pProcessImg->GetWidth();
	BYTE max, min, pixel;
	max = 200;
	min = 100;
	for (LONG i = 0; i < lHeight; ++i)
	{
		for (LONG j = 0; j < lWidth; ++j)
		{
			tempData = lpSrcData + i * lLineBytes + j * depth;
			if (depth == 1) {
				pixel = *tempData;
				*tempData = (BYTE)(((float)(max - min) / 255) * pixel + min + 0.5);
			}
			else
			{
				pixel = *tempData;
				*tempData = (BYTE)(((float)(max - min) / 255) * pixel + min + 0.5);
				pixel = *(tempData + 1);
				*(tempData + 1) = (BYTE)(((float)(max - min) / 255) * pixel + min + 0.5);
				pixel = *(tempData + 2);
				*(tempData + 2) = (BYTE)(((float)(max - min) / 255) * pixel + min + 0.5);
			}
		}
	}
	m_pAllImages[1] = pProcessImg;
	pProcessImg->m_title = "线性灰度增强";
}
//=============================================
//函数功能：分段线性灰度增强
//输入参数：输入图像CDib* lpDib,图像数组指针 CDib** m_pAllImages
//返回值：无
//==============================================
void ImageEnhancement::PiecewiseLinearEnhancement(CDib* lpDib, CDib** m_pAllImages)
{
	*pProcessImg = *lpDib;
	LPBYTE tempData, lpSrcData;
	lpSrcData = pProcessImg->GetData();
	LONG lHeight, lWidth, lLineBytes;
	lLineBytes = pProcessImg->GetLineByte();
	LONG depth = pProcessImg->GetDepth();
	lHeight = pProcessImg->GetHeight();
	lWidth = pProcessImg->GetWidth();
	BYTE pixel;
	BYTE SplitPoint = 100, SplitPoint1 = 150;
	BYTE ASplitPoint = 50, ASplitPoint1 = 200;
	for (LONG i = 0; i < lHeight; ++i)
	{
		for (LONG j = 0; j < lWidth; ++j)
		{
			tempData = lpSrcData + i * lLineBytes + j * depth;
			if (depth == 1) {
				pixel = *tempData;
				if (pixel < SplitPoint) {
					*tempData = (BYTE)(((float)(ASplitPoint) / SplitPoint) * pixel + 0.5);
				}
				else if ((pixel >= SplitPoint)&&(pixel <= SplitPoint1)) {
					*tempData = (BYTE)(((float)(ASplitPoint1 - ASplitPoint) / (SplitPoint1 - SplitPoint)) * (pixel - SplitPoint) + ASplitPoint + 0.5);
				}
				else if ((pixel > SplitPoint1) && (pixel <= 255)) {
					*tempData = (BYTE)(((float)(255 - ASplitPoint1) / (255 - SplitPoint1)) * (pixel - SplitPoint1) + ASplitPoint1 + 0.5);
				}
			}
			else
			{
				pixel = *tempData;
				if (pixel < SplitPoint) {
					*tempData = (BYTE)(((float)(ASplitPoint) / SplitPoint) * pixel + 0.5);
				}
				else if ((pixel >= SplitPoint) && (pixel <= SplitPoint1)) {
					*tempData = (BYTE)(((float)(ASplitPoint1 - ASplitPoint) / (SplitPoint1 - SplitPoint)) * (pixel - SplitPoint) + ASplitPoint + 0.5);
				}
				else if ((pixel > SplitPoint1) && (pixel <= 255)) {
					*tempData = (BYTE)(((float)(255 - ASplitPoint1) / (255 - SplitPoint1)) * (pixel - SplitPoint1) + ASplitPoint1 + 0.5);
				}
				pixel = *(tempData + 1);
				if (pixel < SplitPoint) {
					*(tempData + 1) = (BYTE)(((float)(ASplitPoint) / SplitPoint) * pixel + 0.5);
				}
				else if ((pixel >= SplitPoint) && (pixel <= SplitPoint1)) {
					*(tempData + 1) = (BYTE)(((float)(ASplitPoint1 - ASplitPoint) / (SplitPoint1 - SplitPoint)) * (pixel - SplitPoint) + ASplitPoint + 0.5);
				}
				else if ((pixel > SplitPoint1) && (pixel <= 255)) {
					*(tempData + 1) = (BYTE)(((float)(255 - ASplitPoint1) / (255 - SplitPoint1)) * (pixel - SplitPoint1) + ASplitPoint1 + 0.5);
				}
				pixel = *(tempData + 2);
				if (pixel < SplitPoint) {
					*(tempData + 2) = (BYTE)(((float)(ASplitPoint) / SplitPoint) * pixel + 0.5);
				}
				else if ((pixel >= SplitPoint) && (pixel <= SplitPoint1)) {
					*(tempData + 2) = (BYTE)(((float)(ASplitPoint1 - ASplitPoint) / (SplitPoint1 - SplitPoint)) * (pixel - SplitPoint) + ASplitPoint + 0.5);
				}
				else if ((pixel > SplitPoint1) && (pixel <= 255)) {
					*(tempData + 2) = (BYTE)(((float)(255 - ASplitPoint1) / (255 - SplitPoint1)) * (pixel - SplitPoint1) + ASplitPoint1 + 0.5);
				}
			}
		}
	}
	m_pAllImages[1] = pProcessImg;
	pProcessImg->m_title = "分段线性灰度增强";
}
//=============================================
//函数功能：非线性灰度增强
//输入参数：输入图像CDib* lpDib,图像数组指针 CDib** m_pAllImages
//返回值：无
//==============================================
void ImageEnhancement::NonLinearEnhancement(CDib* lpDib, CDib** m_pAllImages)
{
	*pProcessImg = *lpDib;
	LPBYTE tempData, lpSrcData;
	lpSrcData = pProcessImg->GetData();
	LONG lHeight, lWidth, lLineBytes;
	lLineBytes = pProcessImg->GetLineByte();
	LONG depth = pProcessImg->GetDepth();
	lHeight = pProcessImg->GetHeight();
	lWidth = pProcessImg->GetWidth();
	BYTE pixel;
	double a, b, c;
	a = 20;
	b = 0.1;
	c = 128;
	for (LONG i = 0; i < lHeight; ++i)
	{
		for (LONG j = 0; j < lWidth; ++j)
		{
			tempData = lpSrcData + i * lLineBytes + j * depth;
			if (depth == 1) {
				pixel = *tempData;
				if (pixel == 0)
					pixel += 1;
				*tempData = (BYTE)((log((double)(pixel))) / (b * log(c)) * +a + 0.5);
			}
			else
			{
				pixel = *tempData;
				if (pixel == 0)
					pixel += 1;
				*tempData = (BYTE)((log((double)(pixel))) / (b * log(c)) * +a + 0.5);
				pixel = *(tempData + 1);
				if (pixel == 0)
					pixel += 1;
				*(tempData + 1) = (BYTE)((log((double)(pixel))) / (b * log(c)) * +a + 0.5);
				pixel = *(tempData + 2);
				if (pixel == 0)
					pixel += 1;
				*(tempData + 2) = (BYTE)((log((double)(pixel))) / (b * log(c)) * +a + 0.5);
			}
		}
	}
	m_pAllImages[1] = pProcessImg;
	pProcessImg->m_title = "非线性灰度增强";
}
//=============================================
//函数功能：直方图均衡
//输入参数：输入图像CDib* lpDib,图像数组指针 CDib** m_pAllImages
//返回值：无
//==============================================
void ImageEnhancement::HistogramEnhancement(CDib* lpDib, CDib** m_pAllImages)
{
	*pProcessImg = *lpDib;
	LPBYTE tempData, lpSrcData;
	lpSrcData = pProcessImg->GetData();
	LONG lHeight, lWidth, lLineBytes;
	lLineBytes = pProcessImg->GetLineByte();
	LONG depth = pProcessImg->GetDepth();
	lHeight = pProcessImg->GetHeight();
	lWidth = pProcessImg->GetWidth();
	BYTE pixel;
	double* histogram = new double[256 * depth];
	Histogram(pProcessImg, histogram);
	double* temp = new double[256 * depth];
	memset(temp, 0, sizeof(double) * 256 * (int)depth);
	int* nRst = new int[256 * depth];
	memset(nRst, 0, sizeof(int) * 256 * (int)depth);
	for (int k = 0; k < 256 * (int)depth; ++k)
	{
		if ((k == 0) || (k == 256) || (k == 256 * 2))
		{
			temp[k] = histogram[k];
		}
		else
		{
			temp[k] = temp[k - 1] + histogram[k];
		}
		nRst[k] = (int)(255.0 * temp[k] + 0.5);
	}
	for (LONG i = 0; i < lHeight; ++i)
	{
		for (LONG j = 0; j < lWidth; ++j)
		{
			tempData = lpSrcData + i * lLineBytes + j * depth;
			if (depth == 1) {
				pixel = *tempData;
				*tempData = (BYTE)nRst[pixel];
			}
			else
			{
				pixel = *tempData;
				*tempData = (BYTE)nRst[pixel];
				pixel = *(tempData + 1);
				*(tempData + 1) = (BYTE)nRst[pixel + 256];
				pixel = *(tempData + 2);
				*(tempData + 2) = (BYTE)nRst[pixel + 256 * 2];
			}
		}
	}
	delete[] nRst;
	delete[] temp;
	delete[] histogram;
	m_pAllImages[1] = pProcessImg;
	pProcessImg->m_title = "直方图均衡";
}
//=============================================
//函数功能：直方图统计
//输入参数：输入图像CDib* lpDib,图像直方图概率 double* probability
//返回值：无
//==============================================
void ImageEnhancement::Histogram(CDib* lpDib, double* probability)
{
	LPBYTE tempData, lpSrcData;
	lpSrcData = lpDib->GetData();
	LONG lHeight, lWidth, lLineBytes;
	lLineBytes = lpDib->GetLineByte();
	LONG depth = lpDib->GetDepth();
	lHeight = lpDib->GetHeight();
	lWidth = lpDib->GetWidth();
	BYTE pixel;
	int* gray = new int[256 * depth];
	memset(gray, 0, sizeof(int) * 256 * depth);
	for (LONG i = 0; i < lHeight; ++i)
	{
		for (LONG j = 0; j < lWidth; ++j)
		{
			tempData = lpSrcData + i * lLineBytes + j * depth;
			if (depth == 1) {
				pixel = *tempData;
				++gray[pixel];
			}
			else
			{
				pixel = *tempData;
				++gray[pixel];
				pixel = *(tempData + 1);
				++gray[pixel + 256];
				pixel = *(tempData + 2);
				++gray[pixel + 256 * 2];
			}
		}
	}
	for (int k = 0; k < 256 * (int)depth; ++k)
	{
		probability[k] = ((double)gray[k] / (lHeight * lWidth * 1.0));
	}
	delete[] gray;
}