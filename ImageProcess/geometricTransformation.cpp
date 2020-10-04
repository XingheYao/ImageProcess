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
/*  函数参数
*   CDib originalImage  原始图像
*	CDib* m_pAllImages  所有图像指针
*
*/
bool geometricTransformation::translation(CDib* originalImage, CDib** m_pAllImages,long lXOffset,long lYOffset)
{
	long height = originalImage->GetHeight();
	long width = originalImage->GetWidth();
	long lLineBytes = originalImage->GetLineByte();
	int i, j;
	LPBYTE lpOriginalData = (originalImage->GetData());
	*pProcessImg = *originalImage;
	bool IsRgb = false;
	if (!pProcessImg->IsGrade())
	{
		pProcessImg->RgbToGrade();
		IsRgb = true;
	}
	LPBYTE lpProcessData = pProcessImg -> GetData();
	LPBYTE lpTempData = lpProcessData;
	for (i = 0; i < height; ++i)//行
	{
		for (j = 0; j < width; ++j)//列
		{
			//指向新图像第i行，第j列像素的指针
			lpTempData = lpProcessData + lLineBytes * (height - 1 - i) + j;
			//判断是否在原图像范围内
			if ((j - lYOffset >= 0) && (j - lYOffset < width) &&
				(i - lXOffset >= 0) && (i - lXOffset < height))
			{
				*lpTempData = *(lpOriginalData + lLineBytes *
					(height - 1 - (i - lXOffset)) + (j - lYOffset));
			}
			else
			{
				*(lpTempData) = 255;
			}
		}
	}
	if (IsRgb)
	{
		pProcessImg->GradeToRgb();
	}
	if (m_pAllImages[1])
		delete m_pAllImages[1];
	pProcessImg->m_title = "图像平移";
	m_pAllImages[1] = pProcessImg;
	return true;
}

/*bool geometricTransformation::rotate(CDib* originalImage, CDib** m_pAllImages, float fAngle)
{
	long height = originalImage->GetHeight();
	long width = originalImage->GetWidth();
	long lLineBytes = originalImage->GetLineByte();
	int i, j;
	LPBYTE lpOriginalData = (originalImage->GetData());
	*pProcessImg = *originalImage;
	float fRotateAngle = (float)(fAngle / 180 * PI);
	
	bool IsRgb = false;
	if (pProcessImg->GetBitCount() == 24)
	{
		pProcessImg->RgbToGrade();
		IsRgb = true;
	}
	LPBYTE lpProcessData = pProcessImg->GetData();
	LPBYTE lpTempData = lpProcessData;
	for (i = 0; i < height; ++i)//行
	{
		for (j = 0; j < width; ++j)//列
		{
			//指向新图像第i行，第j列像素的指针
			lpTempData = lpProcessData + lLineBytes * (height - 1 - i) + j;
			//判断是否在原图像范围内
			if ((j - lYOffset >= 0) && (j - lYOffset < width) &&
				(i - lXOffset >= 0) && (i - lXOffset < height))
			{
				*lpTempData = *(lpOriginalData + lLineBytes *
					(height - 1 - (i - lXOffset)) + (j - lYOffset));
			}
			else
			{
				*(lpTempData) = 255;
			}
		}
	}
	if (IsRgb)
	{
		pProcessImg->GradeToRgb();
	}
	if (m_pAllImages[1])
		delete m_pAllImages[1];
	pProcessImg->m_title = "图像旋转";
	m_pAllImages[1] = pProcessImg;
	return true;
}*/