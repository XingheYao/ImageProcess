
// ImageProcessView.cpp: CImageProcessView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ImageProcess.h"
#endif

#include "ImageProcessDoc.h"
#include "ImageProcessView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageProcessView

IMPLEMENT_DYNCREATE(CImageProcessView, CView)

BEGIN_MESSAGE_MAP(CImageProcessView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageProcessView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_SHOW_ORIGIN, &CImageProcessView::OnShowOrigin)
	ON_COMMAND(ID_FILE_OPEN, &CImageProcessView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CImageProcessView::OnFileSave)
	ON_COMMAND(ID_ImgTranslation, &CImageProcessView::OnImgtranslation)
	ON_COMMAND(ID_ImgRotate, &CImageProcessView::OnImgRotate)
	ON_COMMAND(ID_32787, &CImageProcessView::OnHorizontalMirror)
	ON_COMMAND(ID_32788, &CImageProcessView::OnVerticalMirror)
	ON_COMMAND(ID_32789, &CImageProcessView::OnImageTransposition)
	ON_COMMAND(ID_InvariantMoment, &CImageProcessView::OnInvariantmoment)
	ON_COMMAND(ID_NegativeEffect, &CImageProcessView::OnNegativeeffect)
	ON_COMMAND(ID_EmBoss, &CImageProcessView::OnEmboss)
	ON_COMMAND(ID_AVClolorToBW, &CImageProcessView::OnAvclolortobw)
	ON_COMMAND(ID_MaxColorToBW, &CImageProcessView::OnMaxcolortobw)
	ON_COMMAND(ID_WeightColorToBW, &CImageProcessView::OnWeightcolortobw)
	ON_COMMAND(ID_LevelFog, &CImageProcessView::OnLevelfog)
	ON_COMMAND(ID_verticalFog, &CImageProcessView::OnVerticalfog)
	ON_COMMAND(ID_ComFog, &CImageProcessView::OnComfog)
	ON_COMMAND(ID_Sketch, &CImageProcessView::OnSketch)
	ON_COMMAND(ID_Roberts, &CImageProcessView::OnRoberts)
	ON_COMMAND(ID_Sobel, &CImageProcessView::OnSobel)
	ON_COMMAND(ID_Prewitt, &CImageProcessView::OnPrewitt)
	ON_COMMAND(ID_Krisch, &CImageProcessView::OnKrisch)
	ON_COMMAND(ID_Laplacian, &CImageProcessView::OnLaplacian)
	ON_COMMAND(ID_GaussLaplacian, &CImageProcessView::OnGausslaplacian)
	ON_COMMAND(ID_LinearEnhancement, &CImageProcessView::OnLinearEnhancement)
	ON_COMMAND(ID_PiecewiseLinearEnhancement, &CImageProcessView::OnPiecewiselinearenhancement)
	ON_COMMAND(ID_NonLinearEnhancement, &CImageProcessView::OnNonlinearenhancement)
	ON_COMMAND(ID_HistogramEnhancement, &CImageProcessView::OnHistogramenhancement)
	ON_COMMAND(ID_MeanSmoothing, &CImageProcessView::OnMeansmoothing)
	ON_COMMAND(ID_WeightedMeanSmoothing, &CImageProcessView::OnWeightedmeansmoothing)
	ON_COMMAND(ID_FLIC, &CImageProcessView::OnFlic)
	ON_COMMAND(ID_SLIC, &CImageProcessView::OnSlic)
END_MESSAGE_MAP()

// CImageProcessView 构造/析构

CImageProcessView::CImageProcessView() noexcept
{
	// TODO: 在此处添加构造代码
	for (int i = 0; i < MAXNUM; ++i)
	{
		m_pAllImages[i] = nullptr;
	}
}

CImageProcessView::~CImageProcessView()
{
}

BOOL CImageProcessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CImageProcessView 绘图

void CImageProcessView::OnDraw(CDC* pDC)
{
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	// 获取客户区的大小
	CRect rect;
	GetClientRect(rect);
	// 设置为灰色背景
	pDC->FillSolidRect(rect, RGB(230, 230, 230));
	int windowWidth = rect.Width();
	int windowHeight = rect.Height();
	if (m_Image.IsValid())
	{
		m_initPoint.x = 0;
		m_initPoint.y = 0;
		LONG width = m_Image.GetWidth();
		LONG height = m_Image.GetHeight();
		for (int i = 0; i < MAXNUM; ++i)
		{
			if (m_pAllImages[i] != nullptr)
			{
				if ((m_initPoint.x + width) > windowWidth)
				{
					m_initPoint.x = 0;
					m_initPoint.y += height + 30;
					/*if ((m_initPoint.y + height) > windowHeight)
					{
						MessageBox(L"图片显示超出窗口！");
					}*/
				}
				m_pAllImages[i]->Draw(pDC, m_initPoint, m_pAllImages[i]->GetDimension());
				m_initPoint.x += (width +10);				
			}
		}
	}		
	return;
}


// CImageProcessView 打印


void CImageProcessView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageProcessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CImageProcessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CImageProcessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CImageProcessView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageProcessView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageProcessView 诊断

#ifdef _DEBUG
void CImageProcessView::AssertValid() const
{
	CView::AssertValid();
}

void CImageProcessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageProcessDoc* CImageProcessView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessDoc)));
	return (CImageProcessDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageProcessView 消息处理程序


void CImageProcessView::OnFileOpen()
{

	// TODO: 在此添加命令处理程序代码
	//四种格式的文件：bmp gif jpg tiff
	CString filter;
	filter = "所有文件(*.bmp,*.jpg,*.gif,*tiff)|*.bmp;*.jpg;*.gif;*.tiff| BMP(*.bmp)|*.bmp| JPG(*.jpg)|*.jpg| GIF(*.gif)|*.gif| TIFF(*.tiff)|*.tiff||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);

	//按下确定按钮 dlg.DoModal() 函数显示对话框
	if ( IDOK == dlg.DoModal() )
	{
		for (int i = 0; i < MAXNUM; ++i)
		{
			if (m_pAllImages[i])
				m_pAllImages[i]->Empty(true);
			m_pAllImages[i] = nullptr;
		}
		m_EntName = dlg.GetFileExt();      //获取文件扩展名
		m_EntName.MakeLower();             //将文件扩展名转换为一个小写字符
		if (m_EntName == "bmp")
			m_Image.LoadFromFile(dlg.GetPathName());//获得图片的地址，并且加载图片
		else if (m_EntName == "jpg")
			m_Image.LoadJPGFromFile(dlg.GetPathName());
		else
		{
			return;
		}
		//m_OriginName = dlg.GetPathName();     //获取文件路径名   

		m_pAllImages[0]=&m_Image;
		
		Invalidate(TRUE);                    //调用该函数就会调用OnDraw重绘画图
	}
	

}


void CImageProcessView::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
	for (int i = 1; i < MAXNUM; ++i)
	{
		if (nullptr == m_pAllImages[i])
		{
			continue;
		}
		if (!m_pAllImages[i]->IsValid())
		{
			MessageBox(_T("图像文件无效！"));
			continue;
		}
		CString strFilter;
		//strFilter ="位图文件|*.bmp|JPEG 图像文件|*.jpg|GIF 图像文件|*.gif|PNG 图像文件|*.png||";   //
		strFilter = "位图文件|*.bmp||";   //

		CFileDialog dlg(FALSE, NULL, NULL, NULL, strFilter);
		//CFileDialog dlg(FALSE,NULL,NULL,NULL);

		if (IDOK != dlg.DoModal())
			return;
		// 如果用户没有指定文件扩展名，则为其添加一个
		CString strFileName;
		CString strExtension;
		strFileName = dlg.m_ofn.lpstrFile;   //  获得文件名
		//if(dlg.m_ofn.nFileExtension == 0)     //获得文件扩展名
		//{
		//	switch (dlg.m_ofn.nFilterIndex)
		//	{
		//	case 1:
		//		strExtension = "bmp"; break;
		//	case 2:
		//		strExtension = "jpg"; break;
		//	case 3:
		//		strExtension = "gif"; break;
		//	case 4:
		//		strExtension = "png"; break;
		//	default:
		//		break;
		//	}
		strExtension = "bmp";
		strFileName = strFileName + _T(".") + strExtension;
		//	//strFileName="C:\\Users\\Lenovo\\Desktop\\QSanguosha-Qingming\\abc.bmp";
		//}
		// 图像保存
		BOOL hResult = m_pAllImages[i]->SaveToFile(strFileName);
		if (!hResult)
		{
			MessageBox(_T("保存图像文件失败！"));
		}
	}
}

void CImageProcessView::OnShowOrigin()
{
	
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
		
	CDib* pTest = new CDib(m_Image);

	/*LPBYTE TempData = (pTest->GetData());
	for (int i = 0; i < 5 * pTest->GetLineByte(); ++i)
		TempData[i] = 0;*/
	pTest->RgbToLab();
	pTest->LabToRgb();
	/*LPBYTE lpData = pTest->GetData();
	for (int i = 100; i < 120; ++i)
		for(int j = 100; j< 120 ; ++j)
		{
			 *(lpData + pTest->GetLineByte() * (i -100) + j+100 ) = 255;

		}*/
	pTest->m_title = "测试图像";
	m_pAllImages[1] = pTest;
	Invalidate();
}

void CImageProcessView::OnImgtranslation()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}

	geometricTransformation* pImgTransformation = new geometricTransformation();
	pImgTransformation->translation(&m_Image, m_pAllImages, 50, 50);
	Invalidate();
}


void CImageProcessView::OnImgRotate()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	geometricTransformation* pImgTransformation = new geometricTransformation();
	pImgTransformation->rotate(&m_Image, m_pAllImages, 90);
	Invalidate();
}


void CImageProcessView::OnHorizontalMirror()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	geometricTransformation* pImgTransformation = new geometricTransformation();
	pImgTransformation->mirror(&m_Image, m_pAllImages, true);
	Invalidate();
}


void CImageProcessView::OnVerticalMirror()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	geometricTransformation* pImgTransformation = new geometricTransformation();
	pImgTransformation->mirror(&m_Image, m_pAllImages, false);
	Invalidate();
}


void CImageProcessView::OnImageTransposition()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	geometricTransformation* pImgTransformation = new geometricTransformation();
	pImgTransformation->transposition(&m_Image, m_pAllImages);
	Invalidate();
}



void CImageProcessView::OnInvariantmoment()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}

	CString filter;
	filter = "所有文件(*.bmp,*.jpg,*.gif,*tiff)|*.bmp;*.jpg;*.gif;*.tiff| BMP(*.bmp)|*.bmp| JPG(*.jpg)|*.jpg| GIF(*.gif)|*.gif| TIFF(*.tiff)|*.tiff||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);

	//按下确定按钮 dlg.DoModal() 函数显示对话框
	if (IDOK == dlg.DoModal())
	{
		m_pAllImages[1] = (CDib*)new CDib();
		m_EntName = dlg.GetFileExt();      //获取文件扩展名
		m_EntName.MakeLower();             //将文件扩展名转换为一个小写字符
		if (m_EntName == "bmp")
			m_pAllImages[1]->LoadFromFile(dlg.GetPathName());//获得图片的地址，并且加载图片
		else if (m_EntName == "jpg")
			m_pAllImages[1]->LoadJPGFromFile(dlg.GetPathName());
		else
		{
			return;
		}
		//m_OriginName = dlg.GetPathName();     //获取文件路径名   

		
	}
	PatternMatching* pPatternMatching = new PatternMatching();
	pPatternMatching->calcMoment(m_pAllImages[0], m_pAllImages[1]);
	Invalidate();

}


void CImageProcessView::OnNegativeeffect()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	ImageEffects* pImageEffects = new ImageEffects();
	pImageEffects->NegativeEffect(m_pAllImages[0], m_pAllImages);
	Invalidate();
}


void CImageProcessView::OnEmboss()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	ImageEffects* pImageEffects = new ImageEffects();
	pImageEffects->EmBoss(m_pAllImages[0], m_pAllImages);
	Invalidate();
}


void CImageProcessView::OnAvclolortobw()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	ImageEffects* pImageEffects = new ImageEffects();
	pImageEffects->ColorToBW(m_pAllImages[0], m_pAllImages, 1);
	Invalidate();
}


void CImageProcessView::OnMaxcolortobw()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	ImageEffects* pImageEffects = new ImageEffects();
	pImageEffects->ColorToBW(m_pAllImages[0], m_pAllImages, 2);
	Invalidate();
}


void CImageProcessView::OnWeightcolortobw()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	ImageEffects* pImageEffects = new ImageEffects();
	pImageEffects->ColorToBW(m_pAllImages[0], m_pAllImages, 3);
	Invalidate();
}


void CImageProcessView::OnLevelfog()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	ImageEffects* pImageEffects = new ImageEffects();
	pImageEffects->ComFog(m_pAllImages[0], m_pAllImages, 1, 30);
	Invalidate();
}


void CImageProcessView::OnVerticalfog()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	ImageEffects* pImageEffects = new ImageEffects();
	pImageEffects->ComFog(m_pAllImages[0], m_pAllImages, 2, 5);
	Invalidate();
}


void CImageProcessView::OnComfog()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	ImageEffects* pImageEffects = new ImageEffects();
	pImageEffects->ComFog(m_pAllImages[0], m_pAllImages, 3, 5);
	Invalidate();
}


void CImageProcessView::OnSketch()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	CDib* pTest = new CDib(*m_pAllImages[0]);
	//pTest->RgbToGrade();
	ImageEffects* pImageEffects = new ImageEffects();
	pImageEffects->sketch(pTest, m_pAllImages);
	Invalidate();
}


void CImageProcessView::OnRoberts()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	CDib* pTest = new CDib(*m_pAllImages[0]);
	pTest->RgbToGrade();
	EdgeDetection* pImageEffects = new EdgeDetection();
	pImageEffects->Roberts(pTest, m_pAllImages);
	Invalidate();
}


void CImageProcessView::OnSobel()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	CDib* pTest = new CDib(*m_pAllImages[0]);
	pTest->RgbToGrade();
	EdgeDetection* pImageEffects = new EdgeDetection();
	pImageEffects->Sobel(pTest, m_pAllImages);
	Invalidate();
}


void CImageProcessView::OnPrewitt()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	CDib* pTest = new CDib(*m_pAllImages[0]);
	pTest->RgbToGrade();
	EdgeDetection* pImageEffects = new EdgeDetection();
	pImageEffects->Prewitt(pTest, m_pAllImages);
	Invalidate();
}


void CImageProcessView::OnKrisch()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	CDib* pTest = new CDib(*m_pAllImages[0]);
	pTest->RgbToGrade();
	EdgeDetection* pImageEffects = new EdgeDetection();
	pImageEffects->Krisch(pTest, m_pAllImages);
	Invalidate();
}


void CImageProcessView::OnLaplacian()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	CDib* pTest = new CDib(*m_pAllImages[0]);
	pTest->RgbToGrade();
	EdgeDetection* pImageEffects = new EdgeDetection();
	pImageEffects->Laplacian(pTest, m_pAllImages);
	Invalidate();
}


void CImageProcessView::OnGausslaplacian()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	CDib* pTest = new CDib(*m_pAllImages[0]);
	pTest->RgbToGrade();
	EdgeDetection* pImageEffects = new EdgeDetection();
	pImageEffects->GaussLaplacian(pTest, m_pAllImages);
	Invalidate();
}


void CImageProcessView::OnLinearEnhancement()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	CDib* pTest = new CDib(*m_pAllImages[0]);
	//pTest->RgbToGrade();
	ImageEnhancement* pImageEnhancement = new ImageEnhancement();
	pImageEnhancement->LinearEnhancement(pTest, m_pAllImages);
	Invalidate();
}


void CImageProcessView::OnPiecewiselinearenhancement()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	CDib* pTest = new CDib(*m_pAllImages[0]);
	//pTest->RgbToGrade();
	ImageEnhancement* pImageEnhancement = new ImageEnhancement();
	pImageEnhancement->PiecewiseLinearEnhancement(pTest, m_pAllImages);
	Invalidate();
}


void CImageProcessView::OnNonlinearenhancement()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	CDib* pTest = new CDib(*m_pAllImages[0]);
	//pTest->RgbToGrade();
	ImageEnhancement* pImageEnhancement = new ImageEnhancement();
	pImageEnhancement->NonLinearEnhancement(pTest, m_pAllImages);
	Invalidate();
}


void CImageProcessView::OnHistogramenhancement()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	CDib* pTest = new CDib(*m_pAllImages[0]);
	//pTest->RgbToGrade();
	ImageEnhancement* pImageEnhancement = new ImageEnhancement();
	pImageEnhancement->HistogramEnhancement(pTest, m_pAllImages);
	Invalidate();
}


void CImageProcessView::OnMeansmoothing()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	CDib* pTest = new CDib(*m_pAllImages[0]);
	//pTest->RgbToGrade();
	ImageSmoothing* pImageSmoothing = new ImageSmoothing();
	pImageSmoothing->MeanSmoothing(pTest, m_pAllImages);
	Invalidate();
}


void CImageProcessView::OnWeightedmeansmoothing()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	CDib* pTest = new CDib(*m_pAllImages[0]);
	//pTest->RgbToGrade();
	ImageSmoothing* pImageSmoothing = new ImageSmoothing();
	pImageSmoothing->WeightedMeanSmoothing(pTest, m_pAllImages);
	Invalidate();
}


void CImageProcessView::OnFlic()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	//CDib* pTest = new CDib(*m_pAllImages[0]);
	//pTest->RgbToGrade();
	FLIC* pImageSuperPixels = new FLIC();
	pImageSuperPixels->superPixels(m_pAllImages[0], m_pAllImages, 800);
	Invalidate();
}


void CImageProcessView::OnSlic()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_pAllImages[0]) {
		//printf("请先打开一幅图像(.BMP或.JPG格式)！");
		MessageBox(L"请先打开一幅图像(.BMP或.JPG格式)！");
		return;
	}
	//CDib* pTest = new CDib(*m_pAllImages[0]);
	//pTest->RgbToGrade();
	SLIC* pImageSuperPixels = new SLIC();
	pImageSuperPixels->superPixels(m_pAllImages[0], m_pAllImages, 1000);
	Invalidate();
}
