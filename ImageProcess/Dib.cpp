#include "afx.h"
#include "Dib.h"

//==============================
//���캯������ʼ�����ݳ�Ա
//==============================
CDib::CDib(void)
{
	//���ݳ�Ա��ʼ��
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