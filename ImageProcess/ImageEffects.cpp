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
				b = abs(*tempData - *tempData1 + 128);
				g = abs(*(tempData + 1) - *(tempData1 + 1) + 128);
				r = abs(*(tempData + 2) - *(tempData1 + 2) + 128);
				if (b > 255)
					b = 255;
				if (g > 255)
					g = 255;
				if (r > 255)
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
		*tempData = 128;
	}
	for (LONG i = lLineBytes; i < lHeight * lLineBytes; i+= lLineBytes)
	{
		tempData = lpSrcData + i;
		if(depth == 1)
			*tempData = 128;
		else
		{
			*tempData = 128;
			*(tempData - 1) = 128;
			*(tempData - 2) = 128;
		}
	}
	m_pAllImages[1] = pProcessImg;
	pProcessImg->m_title = "浮雕特效";
}