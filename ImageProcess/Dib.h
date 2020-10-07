#pragma once
//=======================================================
//内容：设备无关位图
//功能：
//      位图的加载与保存
//		位图的信息获取
//		位图的数据获取
//		位图的显示
//      位图的转换
//		位图的相关判断
//================================================

#include <afx.h>
#include <afxwin.h>

extern "C" {
#include "jpeglib.h"
}

#pragma comment(lib, "libjpeg.lib")

class CDib : public CObject
{
public:
	CDib(void);											//构造函数，初始化数据成员
	CDib(CDib& rhs);                                    //构造函数，初始化数据成员
	~CDib(void);										//析构函数，释放内存空间	
	BOOL LoadFromFile(CString lpszPath);				//从文件加载位图
	BOOL SaveToFile(CString lpszPath);					//将位图保存到文件
	CString GetFileName();								//获取位图文件名
	LONG GetWidth();									//获取位图宽度
	LONG GetHeight();									//获取位图高度
	CSize GetDimension();								//获取位图的宽度和高度
	DWORD GetSize();									//获取位图的大小
	WORD GetBitCount();									//获取单个像素所占位数
	UINT GetLineByte();									//获取每行像素所占位数
	DWORD GetNumOfColor();								//获取位图颜色数
	LPRGBQUAD GetRgbQuad();								//获取位图颜色表
	LPBYTE GetData();									//获取位图数据
	BOOL Draw(CDC* pDC, CPoint origin, CSize size);		//显示位图
	BOOL RgbToGrade();									//24位彩色位图转8位灰度位图
	BOOL GradeToRgb();									//8位灰度位图转24位彩色位图
	BOOL HasRgbQuad();									//判断是否含有颜色表
	BOOL IsGrade();										//判断是否是灰度图
	BOOL IsValid();										//判断位图是否有效
	CDib& operator=(const CDib& rhs);                   //“=”运算符重载
	void Empty(BOOL bFlag = TRUE);						//清理空间
protected:
	DWORD CalcRgbQuadLength();							//计算位图颜色表长度
	BOOL MakePalette();									//根据颜色表生成调色板
	
public:
	CString m_title;                                    //文件显示标题
private:
	
	CString m_fileName;							        //位图文件名
	LPBITMAPFILEHEADER m_lpBmpFileHeader;				//位图文件头指针，需动态分配和释放
	LPBYTE m_lpDib;										//位图指针，包含除位图文件头的所有内容
														//需动态分配和释放
	LPBITMAPINFO m_lpBmpInfo;							//位图信息指针
	LPBITMAPINFOHEADER m_lpBmpInfoHeader;				//位图信息头指针
	LPRGBQUAD m_lpRgbQuad;								//位图颜色表指针
	LPBYTE m_lpData;									//位图数据指针
	HPALETTE m_hPalette;								//调色板句柄
	BOOL m_bHasRgbQuad;									//是否有颜色表
	BOOL m_bValid;										//位图是否有效
};