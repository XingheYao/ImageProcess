#include "pch.h"
#include "FLIC.h"
FLIC::FLIC()
{
	pProcessImg = new CDib();
}
FLIC::FLIC(CDib* pDib)
{
	pProcessImg = new CDib(*pDib);
}
FLIC::~FLIC()
{
	if (pProcessImg)
	{
		delete pProcessImg;
		pProcessImg = nullptr;
	}
}
//=============================================
//�������ܣ���ʼ��	the desired number of superpixels K,		�����ظ���
//					the maximal iteration numbers itrmax		����������
//					the spatial distance weight m				�ռ����Ȩ��
//�������������ͼ��CDib* lpDib,ͼ������ָ�� CDib** m_pAllImages, �����ظ���, ����������, �ռ����Ȩ��
//����ֵ����
//==============================================
void FLIC::init(CDib* lpDib, CDib** m_pAllImages, int superpixels, int maxIteration, int weight)
{
	const int superpixelsize = 0.5 + double(lpDib->GetWidth()*lpDib->GetHeight()) / double(superpixels);
	const int step = sqrt(double(superpixelsize)) + 0.5;
}


