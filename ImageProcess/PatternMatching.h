#pragma once

#include "Dib.h"
#include "math.h"

class PatternMatching
{

public:
	bool calcMoment(CDib* pDibSrc, CDib* pDibTemplate); //计算两张图像的7个不变矩
	void calcBaryCenter(CDib* pDib, int* nBaryCenterX, int* nBaryCenterY);//计算重心x、y坐标
	double BaryCenterMoment(CDib* pDib, int nBaryCenterX, int nBaryCenterY, int ip,int jq);//计算图像的矩
private:
	//CDib* pProcessImg;
};

