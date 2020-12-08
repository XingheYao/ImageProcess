#include "pch.h"
#include "ImageSmoothing.h"
ImageSmoothing::ImageSmoothing()
{
	pProcessImg = new CDib();
}
ImageSmoothing::ImageSmoothing(CDib* pDib)
{
	pProcessImg = new CDib(*pDib);
}
ImageSmoothing::~ImageSmoothing()
{
	if (pProcessImg)
	{
		delete pProcessImg;
		pProcessImg = nullptr;
	}
}
//=============================================
//�������ܣ��������
//���������λͼ����ָ�� LPBYTE lpData,����Χ LONG range, 
//			ͼ��߶� LONG height,ͼ���� LONG width, ���ĵ����� LONG x, LONG y
//����ֵ������������ֵ
//==============================================
double ImageSmoothing::neighborOperation(LPBYTE lpData, LONG range, LONG height, LONG width, LONG lLineBytes, LONG depth, LONG offset, LONG x, LONG y, double* templt, double coef)
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
	return tempData;
}
//=============================================
//�������ܣ���ֵƽ��
//�������������ͼ��CDib* lpDib,ͼ������ָ�� CDib** m_pAllImages
//����ֵ����
//==============================================
void ImageSmoothing::MeanSmoothing(CDib* lpDib, CDib** m_pAllImages)
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
	
	/*for (int k = 0; k < 9; ++k)
	{
		templt[k] = 1.0;
	}*/
	//templt[5] = -8;

	double scale = 2;
	double a;
	for (LONG i = 0; i < lHeight; ++i)
	{
		for (LONG j = 0; j < lWidth; ++j)
		{
			tempData = lpNewData + i * lLineBytes + j * depth;
			if (depth == 1)
			{
				a = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1.0 / 9);
				*tempData = a;
			}
			else
			{
				*tempData = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1.0 / 9);
				*(tempData + 1) = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 1, i, j, templt, 1.0 /9) ;
				*(tempData + 2) = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 2, i, j, templt, 1.0 /9) ;

			}
		}
	}
	m_pAllImages[1] = pProcessImg;
	pProcessImg->m_title = "��ֵƽ��";
}
//=============================================
//�������ܣ���Ȩƽ��ƽ��
//�������������ͼ��CDib* lpDib,ͼ������ָ�� CDib** m_pAllImages
//����ֵ����
//==============================================
void ImageSmoothing::WeightedMeanSmoothing(CDib* lpDib, CDib** m_pAllImages)
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
	double templt[9] = { 1.0, 2.0, 1.0,
						 2.0, 4.0 ,2.0 ,
						 1.0, 2.0, 1.0 };
	double templt1[9] = { 1.0, 1.0, 1.0,
						  1.0, 2.0 ,1.0 ,
						  1.0, 1.0, 1.0 };
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
				a = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1.0 / 16);
				*tempData = a;
			}
			else
			{
				*tempData = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 0, i, j, templt, 1.0 / 16);
				*(tempData + 1) = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 1, i, j, templt, 1.0 / 16);
				*(tempData + 2) = neighborOperation(lpSrcData, 3, lHeight, lWidth, lLineBytes, depth, 2, i, j, templt, 1.0 / 16);

			}
		}
	}

	m_pAllImages[1] = pProcessImg;
	pProcessImg->m_title = "��Ȩƽ��ƽ��";
}