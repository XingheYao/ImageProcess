#include "pch.h"
#include "EdgeDetection.h"
EdgeDetection::EdgeDetection()
{
	pProcessImg = new CDib();
}
EdgeDetection::EdgeDetection(CDib* pDib)
{
	pProcessImg = new CDib(*pDib);
}
EdgeDetection::~EdgeDetection()
{
	if (pProcessImg)
	{
		delete pProcessImg;
		pProcessImg = nullptr;
	}
}
//=============================================
//函数功能：邻域操作
//输入参数：位图数据指针 LPBYTE lpData,邻域范围 LONG range, 
//			图像高度 LONG height,图像宽度 LONG width, 中心点坐标 LONG x, LONG y
//返回值：邻域操作后的值
//==============================================
double EdgeDetection::neighborOperation(LPBYTE lpData, LONG range, LONG height, LONG width, LONG lLineBytes, LONG depth, LONG offset, LONG x, LONG y, double* templt, double coef)
{
	LONG tempX, tempY;
	double tempData = 0.0, tempData1;
	for (LONG i = 0; i < range; ++i)
	{
		for (LONG j = 0; j < range; ++j)
		{
			tempX = x - (range - 1) / 2 + i;
			tempY = y - (range - 1) / 2 + j;
			if (tempX >= 0 && tempX < height && tempY >= 0 && tempY < width)
			{
				tempData1 = (((double) * (lpData + tempX * lLineBytes + depth * tempY + offset)) * templt[i * range + j]);
				tempData += tempData1;

			}
			else
			{
				tempData1 = (((double) * (lpData + x * lLineBytes + depth * y + offset)) * templt[i * (range)+j]);
				tempData += tempData1;
			}

		}
	}
	
	tempData = tempData * coef;
	if (tempData < 0)
		tempData = 0;
	if (tempData > 255)
		tempData = 255;
	return tempData ;
}
//=============================================
//函数功能：Roberts算子
//输入参数：输入图像CDib* lpDib,图像数组指针 CDib** m_pAllImages
//返回值：无
//==============================================
void EdgeDetection::Roberts(CDib* lpDib, CDib** m_pAllImages)
{

	*pProcessImg = *lpDib;
	LPBYTE tempData, lpSrcData, lpNewData, tempRandData;
	lpSrcData = lpDib->GetData();
	lpNewData = pProcessImg->GetData();
	LONG lHeight, lWidth, lLineBytes, k, randPos;
	lLineBytes = pProcessImg->GetLineByte();
	LONG depth = pProcessImg->GetDepth();
	lHeight = pProcessImg->GetHeight();
	lWidth = pProcessImg->GetWidth();
	double templt[4] = { -1.0, 0.0, 0.0, 1.0 };
	double templt1[4] = { 0.0, -1.0, 1.0,0.0 };
	/*for (int k = 0; k < 9; ++k)
	{
		templt[k] = 1.0;
	}*/
	//templt[5] = -8;

	double scale = 2;
	double a, b1, b2, b;
	for (LONG i = 0; i < lHeight; ++i)
	{
		for (LONG j = 0; j < lWidth; ++j)
		{
			tempData = lpNewData + i * lLineBytes + j * depth;
			if (depth == 1)
			{
				a = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1 );
				b1 = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt1, 1 );
				b2 = sqrt(a * a + b1 * b1);
				
				//a = 255 - a;
				*tempData = b2;
			}
			else
			{
				/**tempData = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1);
				*(tempData + 1) = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 1, i, j, templt, 1) ;
				*(tempData + 2) = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 2, i, j, templt, 1) ;*/

			}
		}
	}

	m_pAllImages[1] = pProcessImg;
	pProcessImg->m_title = "Roberts算子";
}
//=============================================
//函数功能：Sobel算子
//输入参数：输入图像CDib* lpDib,图像数组指针 CDib** m_pAllImages
//返回值：无
//==============================================
void EdgeDetection::Sobel(CDib* lpDib, CDib** m_pAllImages)
{

	*pProcessImg = *lpDib;
	LPBYTE tempData, lpSrcData, lpNewData, tempRandData;
	lpSrcData = lpDib->GetData();
	lpNewData = pProcessImg->GetData();
	LONG lHeight, lWidth, lLineBytes, k, randPos;
	lLineBytes = pProcessImg->GetLineByte();
	LONG depth = pProcessImg->GetDepth();
	lHeight = pProcessImg->GetHeight();
	lWidth = pProcessImg->GetWidth();
	double templt[9] = { -1.0, -2.0, -1.0,
						 0.0, 0.0 ,0.0 ,
						 1.0, 2.0, 1.0 };
	double templt1[9] = { 1.0, 0.0, -1.0,
						  2.0, 0.0, -2.0,
						  1.0, 0.0, -1.0 };
	/*for (int k = 0; k < 9; ++k)
	{
		templt[k] = 1.0;
	}*/
	//templt[5] = -8;

	double scale = 2;
	double a, b1, b2, b;
	for (LONG i = 0; i < lHeight; ++i)
	{
		for (LONG j = 0; j < lWidth; ++j)
		{
			tempData = lpNewData + i * lLineBytes + j * depth;
			if (depth == 1)
			{
				a = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1);
				b1 = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt1, 1);
				b = b1 > a ? b1 : a;

				//a = 255 - a;
				*tempData = b;
			}
			else
			{
				/**tempData = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1);
				*(tempData + 1) = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 1, i, j, templt, 1) ;
				*(tempData + 2) = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 2, i, j, templt, 1) ;*/

			}
		}
	}

	m_pAllImages[1] = pProcessImg;
	pProcessImg->m_title = "Sobel算子";
}
//=============================================
//函数功能：Prewitt算子
//输入参数：输入图像CDib* lpDib,图像数组指针 CDib** m_pAllImages
//返回值：无
//==============================================
void EdgeDetection::Prewitt(CDib* lpDib, CDib** m_pAllImages)
{

	*pProcessImg = *lpDib;
	LPBYTE tempData, lpSrcData, lpNewData, tempRandData;
	lpSrcData = lpDib->GetData();
	lpNewData = pProcessImg->GetData();
	LONG lHeight, lWidth, lLineBytes, k, randPos;
	lLineBytes = pProcessImg->GetLineByte();
	LONG depth = pProcessImg->GetDepth();
	lHeight = pProcessImg->GetHeight();
	lWidth = pProcessImg->GetWidth();
	double templt[9] = { -1.0, -1.0, -1.0,
						 0.0, 0.0 ,0.0 ,
						 1.0, 1.0, 1.0 };
	double templt1[9] = { 1.0, 0.0, -1.0,
						  1.0, 0.0, -1.0,
						  1.0, 0.0, -1.0 };
	/*for (int k = 0; k < 9; ++k)
	{
		templt[k] = 1.0;
	}*/
	//templt[5] = -8;

	double scale = 2;
	double a, b1, b2, b;
	for (LONG i = 0; i < lHeight; ++i)
	{
		for (LONG j = 0; j < lWidth; ++j)
		{
			tempData = lpNewData + i * lLineBytes + j * depth;
			if (depth == 1)
			{
				a = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1);
				b1 = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt1, 1);
				b = b1 > a ? b1 : a;

				//a = 255 - a;
				*tempData = b;
			}
			else
			{
				/**tempData = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1);
				*(tempData + 1) = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 1, i, j, templt, 1) ;
				*(tempData + 2) = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 2, i, j, templt, 1) ;*/

			}
		}
	}

	m_pAllImages[1] = pProcessImg;
	pProcessImg->m_title = "Prewitt算子";
}
//=============================================
//函数功能：Krisch算子
//输入参数：输入图像CDib* lpDib,图像数组指针 CDib** m_pAllImages
//返回值：无
//==============================================
void EdgeDetection::Krisch(CDib* lpDib, CDib** m_pAllImages)
{

	*pProcessImg = *lpDib;
	LPBYTE tempData, lpSrcData, lpNewData, tempRandData;
	lpSrcData = lpDib->GetData();
	lpNewData = pProcessImg->GetData();
	LONG lHeight, lWidth, lLineBytes, k, randPos;
	lLineBytes = pProcessImg->GetLineByte();
	LONG depth = pProcessImg->GetDepth();
	lHeight = pProcessImg->GetHeight();
	lWidth = pProcessImg->GetWidth();
	double templt[9] = { 5.0, 5.0, 5.0,
						 -3.0, 0.0 ,-3.0 ,
						 -3.0, -3.0, -3.0 };
	double templt1[9] = { -3.0, 5.0, 5.0,
						  -3.0, 0.0, 5.0,
						  -3.0, -3.0, -3.0 };
	double templt2[9] = { -3.0, -3.0, 5.0,
						  -3.0, 0.0, 5.0,
						  -3.0, -3.0, 5.0 };
	double templt3[9] = { -3.0, -3.0,-3.0,
						  -3.0, 0.0, 5.0,
						  -3.0, 5.0, 5.0 };
	double templt4[9] = { -3.0, -3.0, -3.0,
						  -3.0, 0.0, -3.0,
						  5.0, 5.0, 5.0 };
	double templt5[9] = { -3.0, -3.0, -3.0,
						  5.0, 0.0, -3.0,
						  5.0, 5.0, -3.0 };
	double templt6[9] = { 5.0, -3.0, -3.0,
						  5.0, 0.0, -3.0,
						  5.0, -3.0, -3.0 };
	double templt7[9] = { 5.0, 5.0, -3.0,
						  5.0, 0.0, -3.0,
						  -3.0, -3.0, -3.0 };
	/*for (int k = 0; k < 9; ++k)
	{
		templt[k] = 1.0;
	}*/
	//templt[5] = -8;

	double scale = 2;
	double a, b, b1, b2, b3, b4, b5, b6, b7;
	for (LONG i = 0; i < lHeight; ++i)
	{
		for (LONG j = 0; j < lWidth; ++j)
		{
			tempData = lpNewData + i * lLineBytes + j * depth;
			if (depth == 1)
			{
				b = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1);
				a = b;
				b1 = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt1, 1);
				a = a > b1 ? a : b1;
				b2 = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt2, 1);
				a = a > b2 ? a : b2;
				b3 = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt3, 1);
				a = a > b3 ? a : b3;
				b4 = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt4, 1);
				a = a > b4 ? a : b4;
				b5 = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt5, 1);
				a = a > b5 ? a : b5;
				b6 = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt6, 1);
				a = a > b6 ? a : b6;
				b7 = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt7, 1);
				a = a > b7 ? a : b7;

				//a = 255 - a;
				*tempData = a;
			}
			else
			{
				/**tempData = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1);
				*(tempData + 1) = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 1, i, j, templt, 1) ;
				*(tempData + 2) = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 2, i, j, templt, 1) ;*/

			}
		}
	}

	m_pAllImages[1] = pProcessImg;
	pProcessImg->m_title = "Krisch算子";
}
//=============================================
//函数功能：Laplacian算子
//输入参数：输入图像CDib* lpDib,图像数组指针 CDib** m_pAllImages
//返回值：无
//==============================================
void EdgeDetection::Laplacian(CDib* lpDib, CDib** m_pAllImages)
{

	*pProcessImg = *lpDib;
	LPBYTE tempData, lpSrcData, lpNewData, tempRandData;
	lpSrcData = lpDib->GetData();
	lpNewData = pProcessImg->GetData();
	LONG lHeight, lWidth, lLineBytes, k, randPos;
	lLineBytes = pProcessImg->GetLineByte();
	LONG depth = pProcessImg->GetDepth();
	lHeight = pProcessImg->GetHeight();
	lWidth = pProcessImg->GetWidth();
	double templt[9] = { 0.0, -1.0, 0.0,
						 -1.0, 4.0 ,-1.0 ,
						 0.0, -1.0, 0.0 };
	double templt1[9] = { -1.0, -1.0, -1.0,
						  -1.0, 8.0, -1.0,
						  -1.0, -1.0, -1.0 };
	/*for (int k = 0; k < 9; ++k)
	{
		templt[k] = 1.0;
	}*/
	//templt[5] = -8;

	double scale = 2;
	double a, b1, b2, b;
	for (LONG i = 0; i < lHeight; ++i)
	{
		for (LONG j = 0; j < lWidth; ++j)
		{
			tempData = lpNewData + i * lLineBytes + j * depth;
			if (depth == 1)
			{
				//a = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1);
				b1 = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1);
				//b = b1 > a ? b1 : a;

				//a = 255 - a;
				*tempData = b1;
			}
			else
			{
				/**tempData = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1);
				*(tempData + 1) = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 1, i, j, templt, 1) ;
				*(tempData + 2) = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 2, i, j, templt, 1) ;*/

			}
		}
	}

	m_pAllImages[1] = pProcessImg;
	pProcessImg->m_title = "Laplacian算子";
}
//=============================================
//函数功能：Gauss-Laplacian算子
//输入参数：输入图像CDib* lpDib,图像数组指针 CDib** m_pAllImages
//返回值：无
//==============================================
void EdgeDetection::GaussLaplacian(CDib* lpDib, CDib** m_pAllImages)
{

	*pProcessImg = *lpDib;
	LPBYTE tempData, lpSrcData, lpNewData, tempRandData;
	lpSrcData = lpDib->GetData();
	lpNewData = pProcessImg->GetData();
	LONG lHeight, lWidth, lLineBytes, k, randPos;
	lLineBytes = pProcessImg->GetLineByte();
	LONG depth = pProcessImg->GetDepth();
	lHeight = pProcessImg->GetHeight();
	lWidth = pProcessImg->GetWidth();
	double templt[25] = { -2.0, -4.0, -4.0, -4.0, -2.0,
						  -4.0, 0.0 ,8.0, 0.0, -4.0,
						  -4.0, 8.0 ,24.0, 8.0, -4.0,
						  -4.0, 0.0 ,8.0, 0.0, -4.0,
						  -2.0, -4.0, -4.0,-4.0,-2.0, };
	/*double templt1[9] = { -1.0, -1.0, -1.0,
						  -1.0, 8.0, -1.0,
						  -1.0, -1.0, -1.0 };*/
	/*for (int k = 0; k < 9; ++k)
	{
		templt[k] = 1.0;
	}*/
	//templt[5] = -8;

	double scale = 2;
	double a, b1, b2, b;
	for (LONG i = 0; i < lHeight; ++i)
	{
		for (LONG j = 0; j < lWidth; ++j)
		{
			tempData = lpNewData + i * lLineBytes + j * depth;
			if (depth == 1)
			{
				//a = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1);
				b1 = neighborOperation(lpSrcData, 5, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1);
				//b = b1 > a ? b1 : a;

				//a = 255 - a;
				*tempData = b1;
			}
			else
			{
				/**tempData = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1);
				*(tempData + 1) = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 1, i, j, templt, 1) ;
				*(tempData + 2) = neighborOperation(lpSrcData, 2, lHeight, lWidth, lLineBytes, depth, 2, i, j, templt, 1) ;*/

			}
		}
	}

	m_pAllImages[1] = pProcessImg;
	pProcessImg->m_title = "Gauss-Laplacian算子";
}