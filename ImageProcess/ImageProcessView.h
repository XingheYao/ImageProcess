
// ImageProcessView.h: CImageProcessView 类的接口
//

#pragma once
#include "Dib.h"
#define MAXNUM 10
#include "geometricTransformation.h"
#include "PatternMatching.h"
#include "ImageEffects.h"
#include "EdgeDetection.h"
#include "ImageEnhancement.h"
#include "ImageSmoothing.h"
#include "FLIC.h"
#include "SLIC.h"

class CImageProcessView : public CView
{
protected: // 仅从序列化创建
	CImageProcessView() noexcept;
	DECLARE_DYNCREATE(CImageProcessView)

// 特性
public:
	CImageProcessDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CImageProcessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
private:
	CDib m_Image;
	CString m_EntName;
	CPoint m_initPoint;
	CDib* m_pAllImages[MAXNUM];
// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowOrigin();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnImgtranslation();
	afx_msg void OnImgRotate();
	afx_msg void OnHorizontalMirror();
	afx_msg void OnVerticalMirror();
	afx_msg void OnImageTransposition();
	afx_msg void OnInvariantMoment();
	afx_msg void OnInvariantmoment();
	afx_msg void OnNegativeeffect();
	afx_msg void OnEmboss();
	afx_msg void OnAvclolortobw();
	afx_msg void OnMaxcolortobw();
	afx_msg void OnWeightcolortobw();
	afx_msg void OnLevelfog();
	afx_msg void OnVerticalfog();
	afx_msg void OnComfog();
	afx_msg void OnSketch();
	afx_msg void OnRoberts();
	afx_msg void OnSobel();
	afx_msg void OnPrewitt();
	afx_msg void OnKrisch();
	afx_msg void OnLaplacian();
	afx_msg void OnGausslaplacian();
	afx_msg void OnLinearEnhancement();
	afx_msg void OnPiecewiselinearenhancement();
	afx_msg void OnNonlinearenhancement();
	afx_msg void OnHistogramenhancement();
	afx_msg void OnMeansmoothing();
	afx_msg void OnWeightedmeansmoothing();
	afx_msg void OnFlic();
	afx_msg void OnSlic();
};

#ifndef _DEBUG  // ImageProcessView.cpp 中的调试版本
inline CImageProcessDoc* CImageProcessView::GetDocument() const
   { return reinterpret_cast<CImageProcessDoc*>(m_pDocument); }
#endif

