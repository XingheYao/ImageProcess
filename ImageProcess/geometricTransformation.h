#pragma once
#include "Dib.h"
#include "math.h"
#define PI 3.1415926535898
class geometricTransformation
{

public:
	geometricTransformation();
	~geometricTransformation();
	bool translation(CDib* originalImage, CDib** m_pAllImages, long lXOffset, long lYOffset);
	bool rotate(CDib* originalImage, CDib** m_pAllImages, float fRotateAngle);
private:
	CDib* pProcessImg;

};

