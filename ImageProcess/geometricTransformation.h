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
	bool mirror(CDib* originalImage, CDib** m_pAllImages, bool isHorizontal);
	bool transposition(CDib* originalImage, CDib** m_pAllImages);

private:
	CDib* pProcessImg;

};

