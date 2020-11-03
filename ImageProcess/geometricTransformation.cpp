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
//	�������ܣ�          ͼ��ƽ��
//	���������
//  CDib originalImage  ԭʼͼ��
//	CDib* m_pAllImages  ����ͼ��ָ��
//  long lXOffset       ������ƽ����
//  long lYOffset       ������ƽ����
//	����ֵ              TRUE or FALSE���Ƿ�ƽ�Ƴɹ���
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
		for (i = 0; i < height; ++i)//��
		{
			for (j = 0; j < width; ++j)//��
			{
				//ָ����ͼ���i�У���j�����ص�ָ��
				lpProcessData = pProcessImg->GetData() + lLineBytes * (height - 1 - i) + j;
				//�ж��Ƿ���ԭͼ��Χ��
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
		for (i = 0; i < height; ++i)//��
		{
			for (j = 0; j < width; ++j)//��
			{
				//ָ����ͼ���i�У���j�����ص�ָ��
				lpProcessData = pProcessImg->GetData() + lLineBytes * (height - 1 - i) + 3 * j;
				//�ж��Ƿ���ԭͼ��Χ��
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
	pProcessImg->m_title = "ͼ��ƽ��";
	m_pAllImages[1] = pProcessImg;
	return true;
}
// =============================================
//	�������ܣ�          ͼ����ת
//	���������
//  CDib originalImage  ԭʼͼ��
//	CDib* m_pAllImages  ����ͼ��ָ��
//  float fAngle        ��ת�Ƕ�
//	����ֵ              TRUE or FALSE���Ƿ�ƽ�Ƴɹ���
//
//=============================================
bool geometricTransformation::rotate(CDib* originalImage, CDib** m_pAllImages, float fAngle)
{
	*pProcessImg = *originalImage;
	pProcessImg->rotate(fAngle);
	if (m_pAllImages[1])
		delete m_pAllImages[1];
	pProcessImg->m_title = "ͼ����ת";
	m_pAllImages[1] = pProcessImg;
	return true;
}