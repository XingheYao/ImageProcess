#include "pch.h"
#include "geometricTransformation.h"

geometricTransformation::geometricTransformation()
{
	pProcessImg = new CDib();
}

geometricTransformation::~geometricTransformation()
{
	if (pProcessImg) 
	{
		delete pProcessImg;
		pProcessImg = nullptr;
	}
}
// =============================================
//	函数功能：          图像平移
//	输入参数：
//  CDib originalImage  原始图像
//	CDib* m_pAllImages  所有图像指针
//  long lXOffset       横坐标平移量
//  long lYOffset       纵坐标平移量
//	返回值              TRUE or FALSE（是否平移成功）
//
//=============================================
bool geometricTransformation::translation(CDib* originalImage, CDib** m_pAllImages,long lXOffset,long lYOffset)
{
	/*if (!originalImage->IsGrade())
	{
		originalImage->RgbToGrade();
	}*/
	long height = originalImage->GetHeight();
	long width = originalImage->GetWidth();
	long lLineBytes = originalImage->GetLineByte();
	int i, j;
	LPBYTE lpOriginalData = (originalImage->GetData());
	*pProcessImg = *originalImage;
	
	LPBYTE lpProcessData;
	LPBYTE lpTempData;
	if (originalImage->IsGrade())
	{
		for (i = 0; i < height; ++i)//行
		{
			for (j = 0; j < width; ++j)//列
			{
				//指向新图像第i行，第j列像素的指针
				lpProcessData = pProcessImg->GetData() + lLineBytes * (height - 1 - i) + j;
				//判断是否在原图像范围内
				if ((j - lYOffset >= 0) && (j - lYOffset < width) &&
					(i - lXOffset >= 0) && (i - lXOffset < height))
				{
					lpTempData = lpOriginalData + lLineBytes *
						(height - 1 - (i - lXOffset)) + (j - lYOffset);
					*lpProcessData = *lpTempData;
				}
				else
				{
					*lpProcessData = 255;
				}
			}
		}
	}
	else
	{
		for (i = 0; i < height; ++i)//行
		{
			for (j = 0; j < width; ++j)//列
			{
				//指向新图像第i行，第j列像素的指针
				lpProcessData = pProcessImg->GetData() + lLineBytes * (height - 1 - i) + 3 * j;
				//判断是否在原图像范围内
				if ((j - lYOffset >= 0) && (j - lYOffset < width) &&
					(i - lXOffset >= 0) && (i - lXOffset < height))
				{
					lpTempData = lpOriginalData + lLineBytes *(height - 1 - (i - lXOffset)) + 3 * (j - lYOffset);
					*lpProcessData = *lpTempData;
					*(lpProcessData + 1) = *(lpTempData + 1);
					*(lpProcessData + 2) = *(lpTempData + 2);
				}
				else
				{
					*lpProcessData = 255;
					*(lpProcessData + 1) = 255;
					*(lpProcessData + 2) = 255;
				}
			}
		}
	}
	if (m_pAllImages[1])
		delete m_pAllImages[1];
	pProcessImg->m_title = "图像平移";
	m_pAllImages[1] = pProcessImg;
	return true;
}
// =============================================
//	函数功能：          图像旋转
//	输入参数：
//  CDib originalImage  原始图像
//	CDib* m_pAllImages  所有图像指针
//  float fAngle        旋转角度
//	返回值              TRUE or FALSE（是否平移成功）
//
//=============================================
bool geometricTransformation::rotate(CDib* originalImage, CDib** m_pAllImages, float fAngle)
{
	*pProcessImg = *originalImage;
	pProcessImg->rotate(fAngle);
	if (m_pAllImages[1])
		delete m_pAllImages[1];
	pProcessImg->m_title = "图像旋转";
	m_pAllImages[1] = pProcessImg;
	return true;
}