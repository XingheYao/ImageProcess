#include "afx.h"
#include "Dib.h"

//==============================
//构造函数，初始化数据成员
//==============================
CDib::CDib(void)
{
	//数据成员初始化
	strcpy(m_fileName, "");
	m_lpBmpFileHeader = NULL;
	m_lpDib = NULL;
	m_lpBmpInfo = NULL;
	m_lpBmpFileHeader = NULL;
	m_lpRgbQuad = NULL;
	m_lpData = NULL;
	m_hPalette = NULL;
	m_bHasRgbQuad = FALSE;
	m_bValid = FALSE;
}