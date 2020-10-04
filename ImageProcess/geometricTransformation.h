#pragma once
#include "Dib.h"
#define PI 3.14159
class geometricTransformation
{

public:
	geometricTransformation();
	~geometricTransformation();
	bool translation(CDib* originalImage, CDib** m_pAllImages, long lXOffset, long lYOffset);
	//bool rotate(CDib* originalImage, CDib** m_pAllImages, float fRotateAngle);
private:
	CDib* pProcessImg;

};

