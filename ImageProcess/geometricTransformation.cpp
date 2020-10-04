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
/*  ��������
*   CDib originalImage  ԭʼͼ��
*	CDib* m_pAllImages  ����ͼ��ָ��
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
	for (i = 0; i < height; ++i)//��
	{
		for (j = 0; j < width; ++j)//��
		{
			//ָ����ͼ���i�У���j�����ص�ָ��
			lpTempData = lpProcessData + lLineBytes * (height - 1 - i) + j;
			//�ж��Ƿ���ԭͼ��Χ��
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
	pProcessImg->m_title = "ͼ��ƽ��";
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
	for (i = 0; i < height; ++i)//��
	{
		for (j = 0; j < width; ++j)//��
		{
			//ָ����ͼ���i�У���j�����ص�ָ��
			lpTempData = lpProcessData + lLineBytes * (height - 1 - i) + j;
			//�ж��Ƿ���ԭͼ��Χ��
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
	pProcessImg->m_title = "ͼ����ת";
	m_pAllImages[1] = pProcessImg;
	return true;
}*/