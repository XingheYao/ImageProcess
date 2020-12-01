#pragma once
#include "Dib.h"
class EdgeDetection
{

public:
	EdgeDetection();
	EdgeDetection(CDib* pDib);
	~EdgeDetection();

	void Roberts(CDib* lpDib, CDib** m_pAllImages);
	void Sobel(CDib* lpDib, CDib** m_pAllImages);
	void Prewitt(CDib* lpDib, CDib** m_pAllImages);
	void Krisch(CDib* lpDib, CDib** m_pAllImages);
	void Laplacian(CDib* lpDib, CDib** m_pAllImages);
	void GaussLaplacian(CDib* lpDib, CDib** m_pAllImages);
	double neighborOperation(LPBYTE lpData, LONG range, LONG height, LONG width, LONG lLineBytes, LONG depth, LONG offset, LONG x, LONG y, double* templt, double coef);
private:
	CDib* pProcessImg;
};

