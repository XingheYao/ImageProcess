#include "pch.h"
#include "ImageEffects.h"
ImageEffects::ImageEffects()
{
	pProcessImg = new CDib();
}
ImageEffects::ImageEffects(CDib* pDib)
{
	pProcessImg = new CDib(*pDib);
}
ImageEffects::~ImageEffects()
{
	if (pProcessImg)
	{
		delete pProcessImg;
		pProcessImg = nullptr;
	}
}
//=============================================
//函数功能：底片效果
//输入参数：输入图像CDib* lpDib,图像数组指针 CDib** m_pAllImages
//返回值：无
//==============================================
void ImageEffects::NegativeEffect(CDib* lpDib, CDib** m_pAllImages)
{
	*pProcessImg = *lpDib;
	LPBYTE tempData, lpSrcData;
	lpSrcData = pProcessImg->GetData();
	LONG lHeight, lWidth, lLineBytes;
	lLineBytes = pProcessImg->GetLineByte();
	LONG depth = pProcessImg->GetDepth();
	lHeight = pProcessImg->GetHeight();
	lWidth = pProcessImg->GetWidth();
	for (LONG i = 0; i < lHeight; ++i)
	{
		for (LONG j = 0; j < lWidth; ++j)
		{
			tempData = lpSrcData + i * lLineBytes + j * depth;
			if(depth == 1)
			*tempData = 255 - *tempData;
			else
			{
				*tempData = 255 - *tempData;
				*(tempData + 1) = 255 - *(tempData + 1);
				*(tempData + 2) = 255 - *(tempData + 2);
			}
		}
	}
	m_pAllImages[1] = pProcessImg;
	pProcessImg->m_title = "底片特效";
}
//=============================================
//函数功能：底片效果
//输入参数：输入图像CDib* lpDib,图像数组指针 CDib** m_pAllImages
//返回值：无
//==============================================
void ImageEffects::EmBoss(CDib* lpDib, CDib** m_pAllImages)
{
	*pProcessImg = *lpDib;
	LPBYTE tempData, lpSrcData, tempData1;
	lpSrcData = pProcessImg->GetData();
	LONG lHeight, lWidth, lLineBytes;
	lLineBytes = pProcessImg->GetLineByte();
	LONG depth = pProcessImg->GetDepth();
	lHeight = pProcessImg->GetHeight();
	lWidth = pProcessImg->GetWidth();
	for (LONG i = 0; i < lHeight - 1; ++i)
	{
		for (LONG j = 0; j < lWidth - 1; ++j)
		{
			tempData = lpSrcData + i * lLineBytes + j * depth;
			tempData1 = lpSrcData + (i + 1) * lLineBytes + (j + 1) * depth;
			if (depth == 1){
				*tempData = abs(*tempData - *tempData1 + 128);
				if (*tempData > 255)
					*tempData = 255;
			}
			else
			{
				int b, g, r;
				b = abs(*tempData - *tempData1  );
				g = abs(*(tempData + 1) - *(tempData1 + 1) );
				r = abs(*(tempData + 2) - *(tempData1 + 2) );
				if (b > 10)
					b = 255;
				if (g > 10)
					g = 255;
				if (r > 10)
					r = 255;
				*tempData = b;
				*(tempData + 1) = g;
				*(tempData + 2) = r;
			}
		}
	}
	for (LONG j = (lHeight -1) * lLineBytes; j < lHeight * lLineBytes; ++j)
	{
		tempData = lpSrcData + j;	
		*tempData = 0;
	}
	for (LONG i = lLineBytes; i < lHeight * lLineBytes; i+= lLineBytes)
	{
		tempData = lpSrcData + i;
		if(depth == 1)
			*tempData = 128;
		else
		{
			*tempData = 0;
			*(tempData - 1) = 0;
			*(tempData - 2) = 0;
		}
	}
	m_pAllImages[1] = pProcessImg;
	pProcessImg->m_title = "浮雕特效";
}
//=============================================
//函数功能：黑白效果
//输入参数：输入图像CDib* lpDib,图像数组指针 CDib** m_pAllImages caseType 1：平均值法 2：最大值法 3：加权平均值法
//返回值：无
//==============================================
void ImageEffects::ColorToBW(CDib* lpDib, CDib** m_pAllImages, int caseType)
{
	*pProcessImg = *lpDib;
	LPBYTE tempData, lpSrcData;
	lpSrcData = pProcessImg->GetData();
	LONG lHeight, lWidth, lLineBytes;
	lLineBytes = pProcessImg->GetLineByte();
	LONG depth = pProcessImg->GetDepth();
	lHeight = pProcessImg->GetHeight();
	lWidth = pProcessImg->GetWidth();
	for (LONG i = 0; i < lHeight; ++i)
	{
		for (LONG j = 0; j < lWidth; ++j)
		{
			tempData = lpSrcData + i * lLineBytes + j * depth;
			if (depth == 1)
				break;
			else
			{
				switch (caseType)
				{
				case 1:
					//平均值法
					*tempData = ((*tempData) + *(tempData + 1) + *(tempData + 2))/ 3;
					*(tempData + 1) = *tempData;
					*(tempData + 2) = *tempData;
					break;
				case 2:
					//最大值法
					*tempData = ((*tempData) > *(tempData + 1) ? (*tempData) : *(tempData + 1)) > *(tempData + 2) ?
						((*tempData) > * (tempData + 1) ? (*tempData) : *(tempData + 1)) : *(tempData + 2);
				
					*(tempData + 1) = *tempData;
					*(tempData + 2) = *tempData;
					break;
				case 3:
					//加权平均值法
					*tempData = ((*tempData) * 0.2 + *(tempData + 1) * 0.3 + *(tempData + 2) * 0.5) / 3;
					*(tempData + 1) = *tempData;
					*(tempData + 2) = *tempData;
					break;
				case 4:
					*tempData = (*tempData) * 2;
					*(tempData + 1) = *(tempData + 1) * 2;
					*(tempData + 2) = *(tempData + 2) * 2;
					break;
				default:
					break;
				}
				
			}
		}
	}
	m_pAllImages[1] = pProcessImg;
	pProcessImg->m_title = "黑白特效";
}
//=============================================
//函数功能：雾化效果
//输入参数：输入图像CDib* lpDib,图像数组指针 CDib** m_pAllImages，caseType 
//			1：水平雾化 2：垂直雾化 3：复合雾化 
//			int range 雾化随机范围
//返回值：无
//==============================================
void ImageEffects::ComFog(CDib* lpDib, CDib** m_pAllImages, int caseType, int range)
{
	*pProcessImg = *lpDib;
	LPBYTE tempData, lpSrcData, tempRandData;
	lpSrcData = pProcessImg->GetData();
	LONG lHeight, lWidth, lLineBytes, k, randPos;
	lLineBytes = pProcessImg->GetLineByte();
	LONG depth = pProcessImg->GetDepth();
	lHeight = pProcessImg->GetHeight();
	lWidth = pProcessImg->GetWidth();
	for (LONG i = 0; i < lHeight; ++i)
	{
		for (LONG j = 0; j < lWidth; ++j)
		{
			k = abs(rand() % range);


			switch (caseType)
			{
			case 1:
				//水平雾化
				tempData = lpSrcData + i * lLineBytes + j * depth;
				randPos = j * depth + depth * k;
				if (randPos > lLineBytes - 3)
					randPos = lLineBytes -3;

				if (depth == 1)
					*tempData = *(lpSrcData + i * lLineBytes + randPos);
				else
				{
					*tempData = *(lpSrcData + i * lLineBytes + randPos);
					*(tempData + 1) = *(lpSrcData + i * lLineBytes + randPos + 1);
					*(tempData + 2) = *(lpSrcData + i * lLineBytes + randPos + 2);
				}
				break;
			case 2:
				//垂直雾化
				tempData = lpSrcData + i * lLineBytes + j * depth;
				randPos = i + k;
				if (randPos > lHeight -1 )
					randPos = lHeight -1;

				if (depth == 1)
					*tempData = *(lpSrcData + randPos * lLineBytes + j * depth);
				else
				{
					*tempData = *(lpSrcData + randPos * lLineBytes + j * depth);
					*(tempData + 1) = *(lpSrcData + randPos * lLineBytes + j * depth + 1);
					*(tempData + 2) = *(lpSrcData + randPos * lLineBytes + j * depth + 2);
				}
				break;
			case 3:
				//复合雾化
				tempData = lpSrcData + i * lLineBytes + j * depth;
				randPos = j * depth + depth * k;
				if (randPos > lLineBytes - 3)
					randPos = lLineBytes - 3;
				LONG randPos1;
				randPos1 = i + k;
				if (randPos1 > lHeight - 1)
					randPos1 = lHeight - 1;
				if (depth == 1)
					*tempData = *(lpSrcData + randPos1 * lLineBytes + randPos);
				else
				{
					*tempData = *(lpSrcData + randPos1 * lLineBytes + randPos);
					*(tempData + 1) = *(lpSrcData + randPos1 * lLineBytes + randPos + 1);
					*(tempData + 2) = *(lpSrcData + randPos1 * lLineBytes + randPos + 2);
				}
				break;
			default:
				break;
			}


		}
	}
	m_pAllImages[1] = pProcessImg;
	pProcessImg->m_title = "雾化特效";
}
//=============================================
//函数功能：邻域操作
//输入参数：位图数据指针 LPBYTE lpData,邻域范围 LONG range, 
//			图像高度 LONG height,图像宽度 LONG width, 中心点坐标 LONG i, LONG j
//返回值：邻域操作后的值
//==============================================
double ImageEffects::neighborOperation(LPBYTE lpData, LONG range, LONG height, LONG width, LONG lLineBytes, LONG depth, LONG offset, LONG x, LONG y, double* templt,double coef)
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
				tempData1 = (((double) *(lpData + tempX * lLineBytes + depth * tempY + offset)) * templt[i * range + j]);
				tempData += tempData1;

			}
			else
			{
				tempData1 = (((double)*(lpData + x * lLineBytes + depth * y + offset)) * templt[i * (range)+j]);
				tempData += tempData1;
			}
				
		}
	}
	/*if (tempData < 0)
		tempData = 0;
	if (tempData > 255)
		tempData = 255;*/
	return tempData * coef;
}
//=============================================
//函数功能：素描效果
//输入参数：输入图像CDib* lpDib,图像数组指针 CDib** m_pAllImages
//返回值：无
//==============================================
void ImageEffects::sketch(CDib* lpDib, CDib** m_pAllImages)
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
	double templt[9] = { 1.0, 1.0, 1.0, 
						 1.0, 1.0 ,1.0 ,
						 1.0, 1.0, 1.0 };
	double templt1[9] = { 1.0, 1.0, -1.0,
						  1.0, 0, -1.0,
						  1.0, -1.0, -1.0 };
	double templt2[9] = { 1.0, 1.0, 1.0,
						  -1.0, 0, 1.0,
						  -1.0, -1.0, -1.0 };
	double templtAve[9] = { 0.0, 1.0, 0.0,
						    1.0, 1.0, 1.0,
						    0.0, 1.0, 0.0 };
	/*for (int k = 0; k < 9; ++k)
	{
		templt[k] = 1.0;
	}*/
	//templt[5] = -8;

	double scale = 2;
	double a, b1, b2,b;
	for (LONG i = 0; i < lHeight; ++i)
	{
		for (LONG j = 0; j < lWidth; ++j)
		{
			tempData = lpNewData + i * lLineBytes + j * depth;
			if (depth == 1)
			{
				a = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1/9);
				b1 = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt1, 1/9);
				b2 = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt2, 1/9);;
				b = b1 > b2 ? b1 : b2;
				if (b < 25) 
					a = 0;
				else
				{
					a = (int)(a * scale);
					if (a > 255) a = 255;
					else if (a < 32) a = 0;
				}
				a = 255 - a;
				*tempData = a;
			}
			else
			{
				*tempData = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1)/9;
				*(tempData + 1) = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 1, i, j, templt, 1)/9;
				*(tempData + 2) = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 2, i, j, templt, 1)/9;
			}
		}
	}
	/*for (LONG i = 0; i < lHeight; ++i)
	{
		for (LONG j = 0; j < lWidth; ++j)
		{
			tempData = lpNewData + i * lLineBytes + j * depth;
			if (depth == 1)
			{
				a = neighborOperation(lpNewData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templtAve, 1)/5;
				*tempData = a;
			}
		}
	}*/
	m_pAllImages[1] = pProcessImg;
	pProcessImg->m_title = "素描特效";
}