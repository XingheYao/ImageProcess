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
	void ColorToBW(CDib* lpDib, CDib** m_pAllImages, int caseType);
	void ComFog(CDib* lpDib, CDib** m_pAllImages, int caseType, int range);
	void sketch(CDib* lpDib, CDib** m_pAllImages);
	double neighborOperation(LPBYTE lpData, LONG range, LONG height, LONG width, LONG lLineBytes, LONG depth, LONG offset, LONG i, LONG j, double* templt, double coef);
private:
	CDib* pProcessImg;
};

