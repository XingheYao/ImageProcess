#include "pch.h"
#include "PatternMatching.h"
//=============================================
//�������ܣ���������ͼ���7�������
//���������ԭͼ������CDib* pDib, ģ��ͼ�� CDib* pDibTemplate
//����ֵ���ɹ�TRUE ʧ��FALSE
//==============================================
bool PatternMatching::calcMoment(CDib* pDibSrc, CDib* pDibTemplate)
{
	//ԭͼ��ģ������X��Y����ֵ
	int nBaryCenterX = 0, nBaryCenterY = 0;
	int nTemBaryCenterX = 0, nTemBaryCenterY = 0;
	int i;
	double temp;
	//ԭͼ��ģ���7������ء������������ľ�
	double Sa[8], Ta[8];
	double Su00, Su02, Su20, Su11, Su30, Su12, Su21, Su03;
	double Tu00, Tu02, Tu20, Tu11, Tu30, Tu12, Tu21, Tu03;
	double dbR;					//ԭͼ��ģ������ƶ�
	double dSigmaST = 0;		//�м���
	double dSigmaS = 0;
	double dSigmaT = 0;
	//��������x��y����
	calcBaryCenter(pDibSrc, &nBaryCenterX, &nBaryCenterY);
	calcBaryCenter(pDibTemplate, &nBaryCenterX, &nBaryCenterY);
	//����������׾�
	Su00 = BaryCenterMoment(pDibSrc, nTemBaryCenterX, nTemBaryCenterY, 0, 0);
	Su02 = BaryCenterMoment(pDibSrc, nTemBaryCenterX, nTemBaryCenterY, 0, 2)/pow(Su00,2);//pow(x,y) ����x��y�η�
	Su20 = BaryCenterMoment(pDibSrc, nTemBaryCenterX, nTemBaryCenterY, 2, 0) / pow(Su00, 2);//pow(x,y) ����x��y�η�
	Su11 = BaryCenterMoment(pDibSrc, nTemBaryCenterX, nTemBaryCenterY, 1, 1) / pow(Su00, 2);//pow(x,y) ����x��y�η�
	Su30 = BaryCenterMoment(pDibSrc, nTemBaryCenterX, nTemBaryCenterY, 3, 0) / pow(Su00, 2.5);//pow(x,y) ����x��y�η�
	Su12 = BaryCenterMoment(pDibSrc, nTemBaryCenterX, nTemBaryCenterY, 1, 2) / pow(Su00, 2.5);//pow(x,y) ����x��y�η�
	Su21 = BaryCenterMoment(pDibSrc, nTemBaryCenterX, nTemBaryCenterY, 2, 1) / pow(Su00, 2.5);//pow(x,y) ����x��y�η�
	Su03 = BaryCenterMoment(pDibSrc, nTemBaryCenterX, nTemBaryCenterY, 0, 3) / pow(Su00, 2.5);//pow(x,y) ����x��y�η�
	Tu00 = BaryCenterMoment(pDibTemplate, nTemBaryCenterX, nTemBaryCenterY, 0, 0);
	Tu02 = BaryCenterMoment(pDibTemplate, nTemBaryCenterX, nTemBaryCenterY, 0, 2) / pow(Tu00, 2);//pow(x,y) ����x��y�η�
	Tu20 = BaryCenterMoment(pDibTemplate, nTemBaryCenterX, nTemBaryCenterY, 2, 0) / pow(Tu00, 2);//pow(x,y) ����x��y�η�
	Tu11 = BaryCenterMoment(pDibTemplate, nTemBaryCenterX, nTemBaryCenterY, 1, 1) / pow(Tu00, 2);//pow(x,y) ����x��y�η�
	Tu30 = BaryCenterMoment(pDibTemplate, nTemBaryCenterX, nTemBaryCenterY, 3, 0) / pow(Tu00, 2.5);//pow(x,y) ����x��y�η�
	Tu12 = BaryCenterMoment(pDibTemplate, nTemBaryCenterX, nTemBaryCenterY, 1, 2) / pow(Tu00, 2.5);//pow(x,y) ����x��y�η�
	Tu21 = BaryCenterMoment(pDibTemplate, nTemBaryCenterX, nTemBaryCenterY, 2, 1) / pow(Tu00, 2.5);//pow(x,y) ����x��y�η�
	Tu03 = BaryCenterMoment(pDibTemplate, nTemBaryCenterX, nTemBaryCenterY, 0, 3) / pow(Tu00, 2.5);//pow(x,y) ����x��y�η�
	Sa[1] = Su20 + Su02;
	Sa[2] = (Su20 - Su02) * (Su20 - Su02) + 4 * Su11 * Su11;
	Sa[3] = pow((Su30 - 3 * Su12), 2) + pow((3 * Su21 - Su03), 2);
	Sa[4] = pow((Su30 + Su12), 2) + pow((Su21 + Su03), 2);
	Sa[5] = (Su30 - 3 * Su12) * (Su30 + Su12) * (pow((Su30 + Su12), 2) - 3 * pow((Su21 + Su03), 2))
		+ (3 * Su21 - Su03) * (Su21 + Su03) * (3 * pow((Su30 + Su12), 2) - pow((Su21 + Su03), 2));
	Sa[6] = (Su20 - Su02) *(pow((Su30 + Su12), 2) - pow((Su21 + Su03), 2))
		+ 4 * Su11 * (Su30 + Su12) * (Su21 + Su03);
	Sa[7] = (3 * Su21 - Su03) * (Su30 + Su12) *( pow((Su30 + Su12), 2) - 3 * pow((Su21 + Su03), 2))
		+ (Su30 - 3 * Su12) * (Su21 + Su03) * (3 * pow((Su30 + Su12), 2) - pow((Su21 + Su03), 2));
	Ta[1] = Tu20 + Tu02;
	Ta[2] = (Tu20 - Tu02) * (Tu20 - Tu02) + 4 * Tu11 * Tu11;
	Ta[3] = pow((Tu30 - 3 * Tu12), 2) + pow((3 * Tu21 - Tu03), 2);
	Ta[4] = pow((Tu30 + Tu12), 2) + pow((Tu21 + Tu03), 2);
	Ta[5] = (Tu30 - 3 * Tu12) * (Tu30 + Tu12) * pow((Tu30 + Tu12), 2) - 3 * pow((Tu21 + Tu03), 2)
		+ (3 * Tu21 - Tu03) * (Tu21 + Tu03) * (3 * pow((Tu30 + Tu12), 2) - pow((Tu21 + Tu03), 2));
	Ta[6] = (Tu20 - Tu02) * (pow((Tu30 + Tu12), 2) - pow((Tu21 + Tu03), 2))
		+ 4 * Tu11 * (Tu30 + Tu12) * (Tu21 + Tu03);
	Ta[7] = (3 * Tu21 - Tu03) * (Tu30 + Tu12) * (pow((Tu30 + Tu12), 2) - 3 * pow((Tu21 + Tu03), 2))
		+ (Tu30 - 3 * Tu12) * (Tu21 + Tu03) * (3 * pow((Tu30 + Tu12), 2) - pow((Tu21 + Tu03), 2));
	double logSa[8], logTa[8];
	logSa[0] = 0;
	logTa[0] = 0;
	Sa[0] = 0;
	Ta[0] = 0;
	for (i = 1; i < 8; ++i)
	{
		logSa[i] = log(abs(Sa[i]));
		logTa[i] = log(abs(Ta[i]));
		temp = Sa[i] * Ta[i];
		dSigmaST += temp;
		dSigmaS += pow(Sa[i], 2);
		dSigmaT += pow(Ta[i], 2);
	}
	dbR = dSigmaST / (sqrt(dSigmaS) * sqrt(dSigmaT));


	return true;
}
//=============================================
//�������ܣ���������x��y����
//���������ͼ������CDib* pDib, ����X����int* nBaryCenterX,  ����Y����int* nBaryCenterY
//����ֵ����
//==============================================
void PatternMatching::calcBaryCenter(CDib* pDib, int* nBaryCenterX, int* nBaryCenterY)
{
	BYTE* lpSrc;
	LONG lWidth, lHeight;
	LONG lLineBytes;
	CSize SizeDim;
	SizeDim = pDib->GetDimension();
	lWidth = SizeDim.cx;
	lHeight = SizeDim.cy;
	//CSize SizeRealDim;
	lLineBytes = pDib->GetLineByte();
	LPBYTE lpDibBits = pDib->GetData();
	int nPixelValue;
	double m00, m10, m01;//0�ξ�m00��x�����һ�ξغ�y�����һ�ξ�m10
	int i, j;
	double temp;
	m00 = 0;
	m01 = 0;
	m10 = 0;
	//��0�ξ�m00��x�����һ�ξ�m01��y�����һ�ξ�m10
	for (j = 0; j < lHeight; ++j)
	{
		for (i = 0; i < lWidth; ++i)
		{
			lpSrc = (unsigned char*)lpDibBits + lLineBytes * j + i;
			nPixelValue = *lpSrc;
			m00 = m00 + nPixelValue;
			temp = i * nPixelValue;
			m10 = m10 + temp;
			temp = j * nPixelValue;
			m01 = m01 + temp;
		}
	}
	//����x��y����ֵ
	*nBaryCenterX = (int)(m10 / m00 + 0.5);
	*nBaryCenterY = (int)(m01 / m00 + 0.5);

}
//=============================================
//�������ܣ�����ͼ��ľ�
//���������ͼ������CDib* pDib, ����X����int* nBaryCenterX,  ����Y����int* nBaryCenterY��X������int ip, Y������int jq
//����ֵ��ͼ��ľص�ֵ
//==============================================
double  PatternMatching::BaryCenterMoment(CDib* pDib, int nBaryCenterX, int nBaryCenterY, int ip, int jq)
{
	BYTE* lpSrc;
	LONG lWidth, lHeight;
	LONG lLineBytes;
	CSize SizeDim = pDib->GetDimension();
	lWidth = SizeDim.cx;
	lHeight = SizeDim.cy;
	lLineBytes = pDib->GetLineByte();
	LPBYTE lpDibBits = pDib->GetData();
	double dbImageMoment;                        //ͼ��ľ�
	int i, j;
	double temp;
	dbImageMoment = 0;
	//���صļ���
	for (j = 0; j < lHeight; ++j)
	{
		for (i = 0; i < lWidth; ++i)
		{
			lpSrc = (unsigned char*)lpDibBits + lLineBytes * j + i;
			temp = pow((double)(i - nBaryCenterX), ip) * (pow((double)(j - nBaryCenterY), jq));
			temp = temp * (*lpSrc);
			dbImageMoment += temp;
		}
	}
	return dbImageMoment;
}