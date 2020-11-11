#include "pch.h"
#include "PatternMatching.h"
//=============================================
//函数功能：计算两张图像的7个不变矩
//输入参数：原图像数据CDib* pDib, 模板图像 CDib* pDibTemplate
//返回值：成功TRUE 失败FALSE
//==============================================
bool PatternMatching::calcMoment(CDib* pDibSrc, CDib* pDibTemplate)
{
	//原图、模板中心X、Y坐标值
	int nBaryCenterX = 0, nBaryCenterY = 0;
	int nTemBaryCenterX = 0, nTemBaryCenterY = 0;
	int i;
	double temp;
	//原图、模板的7个不变矩、二阶三阶中心矩
	double Sa[8], Ta[8];
	double Su00, Su02, Su20, Su11, Su30, Su12, Su21, Su03;
	double Tu00, Tu02, Tu20, Tu11, Tu30, Tu12, Tu21, Tu03;
	double dbR;					//原图、模板的相似度
	double dSigmaST = 0;		//中间结果
	double dSigmaS = 0;
	double dSigmaT = 0;
	//计算重心x、y坐标
	calcBaryCenter(pDibSrc, &nBaryCenterX, &nBaryCenterY);
	calcBaryCenter(pDibTemplate, &nBaryCenterX, &nBaryCenterY);
	//计算二阶三阶矩
	Su00 = BaryCenterMoment(pDibSrc, nTemBaryCenterX, nTemBaryCenterY, 0, 0);
	Su02 = BaryCenterMoment(pDibSrc, nTemBaryCenterX, nTemBaryCenterY, 0, 2)/pow(Su00,2);//pow(x,y) 返回x的y次方
	Su20 = BaryCenterMoment(pDibSrc, nTemBaryCenterX, nTemBaryCenterY, 2, 0) / pow(Su00, 2);//pow(x,y) 返回x的y次方
	Su11 = BaryCenterMoment(pDibSrc, nTemBaryCenterX, nTemBaryCenterY, 1, 1) / pow(Su00, 2);//pow(x,y) 返回x的y次方
	Su30 = BaryCenterMoment(pDibSrc, nTemBaryCenterX, nTemBaryCenterY, 3, 0) / pow(Su00, 2.5);//pow(x,y) 返回x的y次方
	Su12 = BaryCenterMoment(pDibSrc, nTemBaryCenterX, nTemBaryCenterY, 1, 2) / pow(Su00, 2.5);//pow(x,y) 返回x的y次方
	Su21 = BaryCenterMoment(pDibSrc, nTemBaryCenterX, nTemBaryCenterY, 2, 1) / pow(Su00, 2.5);//pow(x,y) 返回x的y次方
	Su03 = BaryCenterMoment(pDibSrc, nTemBaryCenterX, nTemBaryCenterY, 0, 3) / pow(Su00, 2.5);//pow(x,y) 返回x的y次方
	Tu00 = BaryCenterMoment(pDibTemplate, nTemBaryCenterX, nTemBaryCenterY, 0, 0);
	Tu02 = BaryCenterMoment(pDibTemplate, nTemBaryCenterX, nTemBaryCenterY, 0, 2) / pow(Tu00, 2);//pow(x,y) 返回x的y次方
	Tu20 = BaryCenterMoment(pDibTemplate, nTemBaryCenterX, nTemBaryCenterY, 2, 0) / pow(Tu00, 2);//pow(x,y) 返回x的y次方
	Tu11 = BaryCenterMoment(pDibTemplate, nTemBaryCenterX, nTemBaryCenterY, 1, 1) / pow(Tu00, 2);//pow(x,y) 返回x的y次方
	Tu30 = BaryCenterMoment(pDibTemplate, nTemBaryCenterX, nTemBaryCenterY, 3, 0) / pow(Tu00, 2.5);//pow(x,y) 返回x的y次方
	Tu12 = BaryCenterMoment(pDibTemplate, nTemBaryCenterX, nTemBaryCenterY, 1, 2) / pow(Tu00, 2.5);//pow(x,y) 返回x的y次方
	Tu21 = BaryCenterMoment(pDibTemplate, nTemBaryCenterX, nTemBaryCenterY, 2, 1) / pow(Tu00, 2.5);//pow(x,y) 返回x的y次方
	Tu03 = BaryCenterMoment(pDibTemplate, nTemBaryCenterX, nTemBaryCenterY, 0, 3) / pow(Tu00, 2.5);//pow(x,y) 返回x的y次方
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
//函数功能：计算重心x、y坐标
//输入参数：图像数据CDib* pDib, 重心X坐标int* nBaryCenterX,  重心Y坐标int* nBaryCenterY
//返回值：空
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
	double m00, m10, m01;//0次矩m00、x方向的一次矩和y方向的一次矩m10
	int i, j;
	double temp;
	m00 = 0;
	m01 = 0;
	m10 = 0;
	//求0次矩m00、x方向的一次矩m01和y方向的一次矩m10
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
	//重心x、y坐标值
	*nBaryCenterX = (int)(m10 / m00 + 0.5);
	*nBaryCenterY = (int)(m01 / m00 + 0.5);

}
//=============================================
//函数功能：计算图像的矩
//输入参数：图像数据CDib* pDib, 重心X坐标int* nBaryCenterX,  重心Y坐标int* nBaryCenterY，X坐标幂int ip, Y坐标幂int jq
//返回值：图像的矩的值
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
	double dbImageMoment;                        //图像的矩
	int i, j;
	double temp;
	dbImageMoment = 0;
	//力矩的计算
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