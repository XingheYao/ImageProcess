#pragma once
#include "Dib.h"
class FLIC
{
public:
	FLIC();
	FLIC(CDib* pDib);
	~FLIC();
	void init(CDib* lpDib, CDib** m_pAllImages, int superpixels, int maxIteration, int weight);
	double neighborOperation(LPBYTE lpData, LONG range, LONG height, LONG width, LONG lLineBytes, LONG depth, LONG offset, LONG x, LONG y, double* templt, double coef);
	void WeightedMeanSmoothing(CDib* lpDib, CDib** m_pAllImages);
	void HistogramEnhancement(CDib* lpDib, CDib** m_pAllImages);

private:
	CDib* pProcessImg;
};

