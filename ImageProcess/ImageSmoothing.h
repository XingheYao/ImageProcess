#pragma once
#include "Dib.h"
class ImageSmoothing
{
public:
	ImageSmoothing();
	ImageSmoothing(CDib* pDib);
	~ImageSmoothing();
	void MeanSmoothing(CDib* lpDib, CDib** m_pAllImages);
	double neighborOperation(LPBYTE lpData, LONG range, LONG height, LONG width, LONG lLineBytes, LONG depth, LONG offset, LONG x, LONG y, double* templt, double coef);
	void WeightedMeanSmoothing(CDib* lpDib, CDib** m_pAllImages);
	void HistogramEnhancement(CDib* lpDib, CDib** m_pAllImages);
	
private:
	CDib* pProcessImg;
};

