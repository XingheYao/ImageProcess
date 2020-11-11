#pragma once
#include "Dib.h"
class ImageEffects
{
public:
	ImageEffects();
	ImageEffects(CDib* pDib);
	~ImageEffects();

	void NegativeEffect(CDib* lpDib, CDib** m_pAllImages);
	void EmBoss(CDib* lpDib, CDib** m_pAllImages);
private:
	CDib* pProcessImg;
};

