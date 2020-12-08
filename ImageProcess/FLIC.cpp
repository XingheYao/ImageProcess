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
//函数功能：初始化	the desired number of superpixels K,		超像素个数
//					the maximal iteration numbers itrmax		最大迭代次数
//					the spatial distance weight m				空间距离权重
//输入参数：输入图像CDib* lpDib,图像数组指针 CDib** m_pAllImages, 超像素个数, 最大迭代次数, 空间距离权重
//返回值：无
//==============================================
void FLIC::init(CDib* lpDib, CDib** m_pAllImages, int superpixels, int maxIteration, int weight)
{
	const int superpixelsize = 0.5 + double(lpDib->GetWidth()*lpDib->GetHeight()) / double(superpixels);
	const int step = sqrt(double(superpixelsize)) + 0.5;
}


