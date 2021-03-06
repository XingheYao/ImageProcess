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
#include <iostream>
#include <stdio.h>
using namespace std;
#include <afx.h>
#include <afxwin.h>
#define PI 3.1415926535898
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
	BOOL LoadJPGFromFile(CString lpszPath);				//从文件加载JPG位图
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
	double* GetDData();									//获取double类型位图数据指针
	LPBITMAPFILEHEADER GetBmpFileHeader();              //获取位图文件头指针
	LPBITMAPINFO GetlpBmpInfo();						//获取位图信息指针
	LPBYTE GetlpDib();									//获取位图指针
	LPBITMAPINFOHEADER GetlpBmpInfoHeader();			//获取位图信息头指针
	BOOL Draw(CDC* pDC, CPoint origin, CSize size);		//显示位图
	BOOL RgbToGrade();									//24位彩色位图转8位灰度位图
	BOOL GradeToRgb();									//8位灰度位图转24位彩色位图
	BOOL RgbToXYZ(const BYTE sR, const BYTE  sG, const BYTE sB,
		double& X, double& Y, double& Z);				//RGB转换成XYZ格式
	BOOL RgbToLab();									//RGB转换成LAB格式
	BOOL XYZToRgb(BYTE& R, BYTE& G, BYTE& B,
		const double& X, const double& Y, const double& Z);//XYZ转换RGB成格式
	BOOL LabToRgb();									//LAB转换成RGB格式
	BOOL HasRgbQuad();									//判断是否含有颜色表
	BOOL IsGrade();										//判断是否是灰度图
	BOOL IsValid();										//判断位图是否有效
	CDib& operator=(const CDib& rhs);                   //“=”运算符重载
	void Empty(BOOL bFlag = TRUE);						//清理空间
	DWORD CalcRgbQuadLength();							//计算位图颜色表长度
	BOOL MakePalette();									//根据颜色表生成调色板
	void rotate(float fAngle);                          //旋转图像
	LONG GetDepth();									//获取颜色深度 灰度为1，rgb为3
	LPBYTE createGradeBmp(LONG width, LONG height, bool IsBackup = TRUE);     //创建一个空白灰度位图文件
	LPBYTE createColorBmp(LONG width, LONG height, bool IsBackup = TRUE);		//创建一个空白彩色位图文件
public:
	CString m_title;                                    //文件显示标题
private:
	
	CString m_fileName;							        //位图文件名
	LPBITMAPFILEHEADER m_lpBmpFileHeader;				//位图文件头指针，需动态分配和释放
	LPBYTE m_lpDib;										//位图指针，包含除位图文件头的所有内容													//需动态分配和释放
	LPBITMAPINFO m_lpBmpInfo;							//位图信息指针
	LPBITMAPINFOHEADER m_lpBmpInfoHeader;				//位图信息头指针
	LPRGBQUAD m_lpRgbQuad;								//位图颜色表指针
	LPBYTE m_lpData;									//位图数据指针
	HPALETTE m_hPalette;								//调色板句柄
	BOOL m_bHasRgbQuad;									//是否有颜色表
	BOOL m_bValid;										//位图是否有效
	double* m_lpDData;									//位图数据double型指针（需复制lpData）
};