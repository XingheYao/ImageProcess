#pragma once
#include "Dib.h"
class ImageEnhancement
{
public:
	ImageEnhancement();
	ImageEnhancement(CDib* pDib);
	~ImageEnhancement();
	void LinearEnhancement(CDib* lpDib, CDib** m_pAllImages);
	void PiecewiseLinearEnhancement(CDib* lpDib, CDib** m_pAllImages);
	void NonLinearEnhancement(CDib* lpDib, CDib** m_pAllImages);
	void HistogramEnhancement(CDib* lpDib, CDib** m_pAllImages);
	void Histogram(CDib* lpDib, double* probability);
private:
	CDib* pProcessImg;
}; 

