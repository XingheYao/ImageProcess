#pragma once

#include "Dib.h"
#include "math.h"

class PatternMatching
{

public:
	bool calcMoment(CDib* pDibSrc, CDib* pDibTemplate); //��������ͼ���7�������
	void calcBaryCenter(CDib* pDib, int* nBaryCenterX, int* nBaryCenterY);//��������x��y����
	double BaryCenterMoment(CDib* pDib, int nBaryCenterX, int nBaryCenterY, int ip,int jq);//����ͼ��ľ�
private:
	//CDib* pProcessImg;
};

