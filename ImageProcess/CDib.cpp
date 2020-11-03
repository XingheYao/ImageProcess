//#include "StdAfx.h"

#include "pch.h"

#include "Dib.h"

//================================
//函数功能：构造函数。初始化数据成员
//================================
CDib::CDib(void)
{
	//数据成员初始化
	m_title = "";
	m_fileName="";
	m_lpBmpFileHeader = nullptr;
	m_lpDib = nullptr;
	m_lpBmpInfo = nullptr;
	m_lpBmpInfoHeader = nullptr;
	m_lpRgbQuad = nullptr;
	m_lpData = nullptr;
	m_hPalette = nullptr;
	m_bHasRgbQuad = FALSE;
	m_bValid = FALSE;

}
CDib::CDib(CDib& rhs)
{
	//Empty(TRUE);//清理空间
	//为位图文件头分配空间，并初始化为0
	m_lpBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
	memset(m_lpBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	memcpy(m_lpBmpFileHeader, rhs.m_lpBmpFileHeader, sizeof(BITMAPFILEHEADER));//复制位图文件头
	if (m_lpBmpFileHeader->bfType == 0x4d42)//判断此文件是否位图，0x4d42代表BMP
	{

		int dwDibSize = m_lpBmpFileHeader->bfSize - sizeof(BITMAPFILEHEADER);
		m_lpDib = (LPBYTE)new BYTE[dwDibSize];
		memset(m_lpDib, 0, dwDibSize);
		memcpy(m_lpDib, rhs.m_lpDib, dwDibSize);

		m_lpBmpInfo = (LPBITMAPINFO)m_lpDib;//设置位图信息指针
		m_lpBmpInfoHeader = (LPBITMAPINFOHEADER)m_lpDib;//设置位图信息头指针
		m_lpRgbQuad = (LPRGBQUAD)(m_lpDib + m_lpBmpInfoHeader->biSize);//颜色表指针
		if (m_lpBmpInfoHeader->biClrUsed == 0)
		{
			m_lpBmpInfoHeader->biClrUsed = GetNumOfColor();
		}
		DWORD dwRgbQuadLength = CalcRgbQuadLength();//计算颜色表长度

		m_lpData = m_lpDib + m_lpBmpInfoHeader->biSize + dwRgbQuadLength;//数据指针
		if (m_lpRgbQuad == (LPRGBQUAD)m_lpData)
		{
			m_lpRgbQuad = nullptr;//将位图颜色表指针置空
			m_bHasRgbQuad = FALSE;//无颜色表
		}
		else
		{
			m_bHasRgbQuad = TRUE;//有颜色表
			MakePalette();//根据颜色表生成调色板
		}
		m_lpBmpInfoHeader->biSizeImage = GetSize();//设置位图大小，很多文件都未设置此项
		m_bValid = TRUE;//位图有效
	}
	m_fileName = rhs.m_fileName;
}
//====================================
//函数功能：析构函数，释放内存空间
//====================================
CDib::~CDib(void)
{
	Empty();

}
//=============================================
//函数功能："="运算符重载
//输入参数：
//返回值：无
//==============================================
CDib& CDib::operator=(const CDib& rhs)
{
	Empty(TRUE);//清理空间
	//为位图文件头分配空间，并初始化为0
	m_lpBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
	memset(m_lpBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	memcpy(m_lpBmpFileHeader, rhs.m_lpBmpFileHeader, sizeof(BITMAPFILEHEADER));//复制位图文件头
	if (m_lpBmpFileHeader->bfType == 0x4d42)//判断此文件是否位图，0x4d42代表BMP
	{

		int dwDibSize = m_lpBmpFileHeader->bfSize - sizeof(BITMAPFILEHEADER);
		m_lpDib = (LPBYTE)new BYTE[dwDibSize];
		memset(m_lpDib, 0, dwDibSize);
		memcpy(m_lpDib, rhs.m_lpDib, dwDibSize);

		m_lpBmpInfo = (LPBITMAPINFO)m_lpDib;//设置位图信息指针
		m_lpBmpInfoHeader = (LPBITMAPINFOHEADER)m_lpDib;//设置位图信息头指针
		m_lpRgbQuad = (LPRGBQUAD)(m_lpDib + m_lpBmpInfoHeader->biSize);//颜色表指针
		if (m_lpBmpInfoHeader->biClrUsed == 0)
		{
			m_lpBmpInfoHeader->biClrUsed = GetNumOfColor();
		}
		DWORD dwRgbQuadLength = CalcRgbQuadLength();//计算颜色表长度

		m_lpData = m_lpDib + m_lpBmpInfoHeader->biSize + dwRgbQuadLength;//数据指针
		if (m_lpRgbQuad == (LPRGBQUAD)m_lpData)
		{
			m_lpRgbQuad = nullptr;//将位图颜色表指针置空
			m_bHasRgbQuad = FALSE;//无颜色表
		}
		else
		{
			m_bHasRgbQuad = TRUE;//有颜色表
			MakePalette();//根据颜色表生成调色板
		}
		m_lpBmpInfoHeader->biSizeImage = GetSize();//设置位图大小，很多文件都未设置此项
		m_bValid = TRUE;//位图有效
	}
	m_fileName = rhs.m_fileName;
	return *this;
}
//=====================================
//函数功能：从文件加载bmp位图
//输入函数：LPCTSTR lpszPath表示待加载位图文件路径
//返回值：BOOL-TRUE表示成功，FALSE表示失败
//=====================================
BOOL CDib::LoadFromFile(CString lpszPath)
{
	m_title = "原始图像";
	m_fileName = lpszPath;
	CFile dibFile;
	//以读模式打开位图文件
	if (!dibFile.Open(/*(LPCTSTR)*/m_fileName,CFile::modeRead|CFile::shareDenyWrite))
	{
		return FALSE;
	}
	Empty(FALSE);//清理空间
	//为位图文件头分配空间，并初始化为0
	m_lpBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
	memset(m_lpBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	//读取位图文件头
	int nCount = dibFile.Read((void*)m_lpBmpFileHeader, sizeof(BITMAPFILEHEADER));
	if(nCount != sizeof(BITMAPFILEHEADER))
	{
		return FALSE;
	}
	if (m_lpBmpFileHeader->bfType == 0x4d42)//判断此文件是否位图，0x4d42代表BMP
	{
		//若是位图文件，计算除位图文件头的空间大小，分配空间并初始化为0
		DWORD dwDibSize = dibFile.GetLength() - sizeof(BITMAPFILEHEADER);
		m_lpDib = (LPBYTE)new BYTE[dwDibSize];
		memset(m_lpDib, 0, dwDibSize);
		dibFile.Read(m_lpDib, dwDibSize);//读取除位图文件头的所有数据
		dibFile.Close();//关闭位图文件
		m_lpBmpInfo = (LPBITMAPINFO)m_lpDib;//设置位图信息指针
		m_lpBmpInfoHeader = (LPBITMAPINFOHEADER)m_lpDib;//设置位图信息头指针
		m_lpRgbQuad = (LPRGBQUAD)(m_lpDib + m_lpBmpInfoHeader->biSize);//颜色表指针
		if (m_lpBmpInfoHeader->biClrUsed == 0)
		{
			m_lpBmpInfoHeader->biClrUsed = GetNumOfColor();
		}
		DWORD dwRgbQuadLength = CalcRgbQuadLength();//计算颜色表长度

		m_lpData = m_lpDib + m_lpBmpInfoHeader->biSize + dwRgbQuadLength;//数据指针
		if (m_lpRgbQuad == (LPRGBQUAD)m_lpData)
		{
			m_lpRgbQuad = nullptr;//将位图颜色表指针置空
			m_bHasRgbQuad = FALSE;//无颜色表
		}
		else
		{
			m_bHasRgbQuad = TRUE;//有颜色表
			MakePalette();//根据颜色表生成调色板
		}
		m_lpBmpInfoHeader->biSizeImage = GetSize();//设置位图大小，很多文件都未设置此项
		m_bValid = TRUE;//位图有效
		return TRUE;
	}
	else
	{
		m_bValid = FALSE;//不是位图文件
		return FALSE;
	}
}
//=====================================
//函数功能：从文件加载bmp位图
//输入函数：LPCTSTR lpszPath表示待加载位图文件路径
//返回值：BOOL-TRUE表示成功，FALSE表示失败
//=====================================
BOOL CDib::LoadJPGFromFile(CString lpszPath)
{
	m_title = "原始图像";
	//打开JPG文件
	m_fileName = lpszPath;
	FILE* JPGFile;
	//以读模式打开位图文件
	errno_t err;
	if ((err = _tfopen_s(&JPGFile, lpszPath, _T("rb")))!= 0)
	{
		return FALSE;
	}
	Empty(FALSE);//清理空间

	struct jpeg_decompress_struct cinfo;//定义JPEG文件的解压信息
	struct jpeg_error_mgr jerr;//定义JPEG文件的错误信息
	JSAMPARRAY buffer;	//定义缓冲区
	LPBYTE lpJPGData;
	LPBYTE lpDataPoint;

	cinfo.err = jpeg_std_error(&jerr);    //libjpeg函数，具体参看相关文档
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, JPGFile);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	DWORD width = cinfo.output_width;
	DWORD height = cinfo.output_height;
	DWORD depth = cinfo.output_components;
	DWORD headerSize = 0;
	DWORD fileSize = 0;
	DWORD dwDibSize, dwJPGDataSize, dwBMPDataSize, uBmpLineByte, uJPGLineByte;
	uBmpLineByte = ((width * (depth * 8)) + 31) / 32 * 4;
	dwJPGDataSize = width * height * depth;
	dwBMPDataSize = uBmpLineByte * height;
	uJPGLineByte = width * depth;
	lpJPGData = new BYTE[dwJPGDataSize];
	memset(lpJPGData, 0, sizeof(BYTE) * dwJPGDataSize);

	buffer = (*cinfo.mem->alloc_sarray)
	((j_common_ptr)&cinfo, JPOOL_IMAGE, uJPGLineByte, 1);

	lpDataPoint = lpJPGData;


	while (cinfo.output_scanline < height)
	{
		jpeg_read_scanlines(&cinfo, buffer, 1);    //读取一行jpg图像数据到buffer
		memcpy(lpDataPoint, *buffer, uJPGLineByte);    //将buffer中的数据逐行给lpJPGData
		lpDataPoint += uJPGLineByte;            //一次改变一行
	}

	//写入bmp_Header
	if (depth == 1)
	{
		headerSize = 14 + 40 + 256 * 4;
		fileSize = headerSize + dwBMPDataSize;
	}

	if (depth == 3)
	{
		headerSize = 14 + 40;
		fileSize = headerSize + dwBMPDataSize;
	}
	//设置位图文件头
	m_lpBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
	memset(m_lpBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	m_lpBmpFileHeader->bfType = 0x4D42;
	m_lpBmpFileHeader->bfSize = fileSize;
	m_lpBmpFileHeader->bfOffBits = headerSize;
	//计算除位图文件头的空间大小，分配空间并初始化为0
	dwDibSize = fileSize - 14;

	m_lpDib = (LPBYTE)new BYTE[dwDibSize];
	memset(m_lpDib, 0, dwDibSize);
	//设置位图信息头
	m_lpBmpInfo = (LPBITMAPINFO)m_lpDib;//设置位图信息指针
	m_lpBmpInfoHeader = (LPBITMAPINFOHEADER)m_lpDib;//设置位图信息头指针
	m_lpBmpInfoHeader->biSize = 40;
	m_lpBmpInfoHeader->biWidth = width;
	m_lpBmpInfoHeader->biHeight = height;
	m_lpBmpInfoHeader->biPlanes = 1;
	m_lpBmpInfoHeader->biBitCount = depth * 8;
	m_lpBmpInfoHeader->biSizeImage = dwBMPDataSize;
	m_lpBmpInfoHeader->biXPelsPerMeter = 0;
	m_lpBmpInfoHeader->biYPelsPerMeter = 0;


	if (depth == 1)//灰度图像要添加调色板
	{
		CString seedBmp("seed.bmp");
		CFile seedFile;
		//以读模式打开位图文件
		if (!seedFile.Open(seedBmp, CFile::modeRead | CFile::shareDenyWrite))
		{
			return FALSE;
		}
		//若是位图文件，计算除位图文件头的空间大小，分配空间并初始化为0
		DWORD dwDibSize = seedFile.GetLength() - sizeof(BITMAPFILEHEADER);
		LPBYTE lpSeedDib = (LPBYTE)new BYTE[dwDibSize];
		memset(lpSeedDib, 0, dwDibSize);
		seedFile.Read(lpSeedDib, dwDibSize);//读取除位图文件头的所有数据
		seedFile.Close();//关闭位图文件
		LPBITMAPINFOHEADER lpSeedInfoHeader = (LPBITMAPINFOHEADER)lpSeedDib;//设置位图信息头指针
		m_lpRgbQuad = (LPRGBQUAD)(lpSeedDib + lpSeedInfoHeader->biSize);//颜色表指针
		if (lpSeedInfoHeader->biClrUsed == 0)
		{
			lpSeedInfoHeader->biClrUsed = GetNumOfColor();
		}
		if (m_lpRgbQuad == (LPRGBQUAD)lpSeedDib)
		{
			m_lpRgbQuad = nullptr;//将位图颜色表指针置空
			m_bHasRgbQuad = FALSE;//无颜色表
		}
		else
		{
			m_hPalette = nullptr;
			m_bHasRgbQuad = TRUE;//有颜色表
			MakePalette();//根据颜色表生成调色板
		}
		delete[] lpSeedDib;
	}
	else
	{
		m_lpRgbQuad = nullptr;//将位图颜色表指针置空
		m_bHasRgbQuad = FALSE;//无颜色表
	}
	DWORD dwRgbQuadLength = CalcRgbQuadLength();//计算颜色表长度
	//写入bmp_Data
	m_lpData = m_lpDib + 40 + dwRgbQuadLength;//数据指针
	DWORD startPosition = uJPGLineByte * (height - 1);
	lpDataPoint = lpJPGData + startPosition;    //倒着写数据，bmp格式是倒的，jpg是正的
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width ; j++)
		{
			if (depth == 1)        //处理灰度图
			{
				m_lpData[i * uBmpLineByte + j] = lpDataPoint[j];
			}

			if (depth == 3)        //处理彩色图
			{
				m_lpData[i * uBmpLineByte + 3 * j + 2] = lpDataPoint[3 * j + 0];//R
				m_lpData[i * uBmpLineByte + 3 * j + 1] = lpDataPoint[3 * j + 1];//G
				m_lpData[i * uBmpLineByte + 3 * j + 0] = lpDataPoint[3 * j + 2];//B
			}
		}
		lpDataPoint -= uJPGLineByte;
	}
	m_bValid = TRUE;//位图有效
	
	//清除临时变量
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	delete[] lpJPGData;
	fclose(JPGFile);

	return TRUE;
}
//============================================
//函数功能：将位图保存到文件
//输入参数：LPCTSTR lpszPath表示位图文件保存路径
//返回值：BOOL-TRUE表示成功，FALSE表示失败
//============================================
BOOL CDib::SaveToFile(CString lpszPath)
{
	CFile dibFile;//以写模式打开文件
	if (!dibFile.Open(lpszPath, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive))
	{
		return FALSE;
	}
	m_fileName=lpszPath;
	dibFile.Write(m_lpBmpFileHeader, sizeof(BITMAPFILEHEADER));//将文件头结构写入文件
	dibFile.Write(m_lpBmpInfoHeader, sizeof(BITMAPINFOHEADER));//将信息结构头文件写进文件
	DWORD dwRgbQuadLength = CalcRgbQuadLength();
	if (dwRgbQuadLength != 0)
	{
		dibFile.Write(m_lpRgbQuad, dwRgbQuadLength);
	}
	DWORD dwDataSize = GetLineByte() * GetHeight();
	dibFile.Write(m_lpData, dwDataSize);//将位图数据写入文件
	dibFile.Close();//关闭文件
	return TRUE;
}
//=============================================
//函数功能：获取位图文件名
//输入参数：无
//返回值：LPCTSTR表示位图文件名
//==============================================
CString CDib::GetFileName()
{
	return m_fileName;
}
//=============================================
//函数功能：获取位图宽度
//输入参数：无
//返回值：LONG 表示位图宽度
//==============================================
LONG CDib::GetWidth()
{
	if (m_lpBmpInfoHeader)
		return m_lpBmpInfoHeader->biWidth;
	else
	{
		return -1;
	}
}
//=============================================
//函数功能：获取位图高度
//输入参数：无
//返回值：LONG表示位图高度
//==============================================
LONG CDib::GetHeight()
{
	if(m_lpBmpInfoHeader)
		return m_lpBmpInfoHeader->biHeight;
	else
	{
		return -1;
	}
}
//=============================================
//函数功能：获取位图的高度和宽度
//输入参数：无
//返回值：CSize表示位图的高度和宽度
//==============================================
CSize CDib::GetDimension()
{
	return CSize(GetWidth(), GetHeight());
}
//=============================================
//函数功能：获取位图大小
//输入参数：无 
//返回值：DWORD表示位图大小
//==============================================
DWORD CDib::GetSize()
{
	if (m_lpBmpInfoHeader->biSizeImage != 0)
	{
		return m_lpBmpInfoHeader->biSizeImage;
	}
	else
	{
		return GetWidth() * GetHeight();
	}
}
//=============================================
//函数功能：获取单个像素所占位数
//输入参数：无
//返回值：WORD表示单个像素所占位数
//==============================================
WORD CDib::GetBitCount()
{
	return m_lpBmpInfoHeader->biBitCount;
}
//=============================================
//函数功能：获取每行像素所占字节数
//输入参数：无
//返回值：UINT表示每行像素所占字节数
//==============================================
UINT CDib::GetLineByte()
{
	return (GetWidth() * GetBitCount() / 8 + 3) / 4 * 4;
}
//=============================================
//函数功能：获取位图颜色数
//输入参数：无
//返回值：DWORD表示位图颜色数
//==============================================
DWORD CDib::GetNumOfColor()
{
	UINT dwNumOfColor = 0;
	if ((m_lpBmpInfoHeader->biClrUsed == 0)&&(m_lpBmpInfoHeader->biBitCount < 9))
	{
		switch (m_lpBmpInfoHeader->biBitCount)
		{
		case 1: dwNumOfColor = 2; break;
		case 2: dwNumOfColor = 16; break;
		case 8: dwNumOfColor = 256; 
		}
	}
	else
	{
		dwNumOfColor = m_lpBmpInfoHeader->biClrUsed;
	}
	return dwNumOfColor;
}
//=============================================
//函数功能：计算位图颜色表长度
//输入参数：无
//返回值：DWORD表示位图颜色表长度
//==============================================
DWORD CDib::CalcRgbQuadLength()
{
	DWORD dwNumOfColor = GetNumOfColor();
	if (dwNumOfColor > 256)
	{
		dwNumOfColor = 0;
	}
	return dwNumOfColor * sizeof(RGBQUAD);
}
//=============================================
//函数功能：获取位图颜色表
//输入参数：无
//返回值：LPRGBQUAD表示位图颜色表指针
//==============================================
LPRGBQUAD CDib::GetRgbQuad()
{
	return m_lpRgbQuad;
}
//=============================================
//函数功能：获取位图数据
//输入参数：无
//返回值：LPBYTE表示位图数据指针
//==============================================
LPBYTE CDib::GetData()
{
	return m_lpData;
}
//=============================================
//函数功能：获取m_lpBmpFileHeader
//输入参数：无
//返回值：LPBITMAPFILEHEADER表示m_lpBmpFileHeader
//==============================================
LPBITMAPFILEHEADER CDib::GetBmpFileHeader()
{
	return m_lpBmpFileHeader;
}
//=============================================
//函数功能：获取m_lpDib
//输入参数：无
//返回值：LPBYTE表示m_lpDib
//==============================================
LPBYTE CDib::GetlpDib()
{
	return m_lpDib;
}
//=============================================
//函数功能：获取位图数据
//输入参数：无
//返回值：LPBITMAPINFO表示m_lpBmpInfo
//==============================================
LPBITMAPINFO CDib::GetlpBmpInfo()
{
	return m_lpBmpInfo;
}
//=============================================
//函数功能：获取位图数据
//输入参数：无
//返回值：LPBITMAPINFOHEADER表示m_lpBmpInfoHeader
//==============================================
LPBITMAPINFOHEADER CDib::GetlpBmpInfoHeader()
{
	return m_lpBmpInfoHeader;
}
//=============================================
//函数功能：根据颜色表生成调色板
//输入参数：无
//返回值：BOOL-TRUE表示成功，FALSE表示失败
//==============================================
BOOL CDib::MakePalette()
{
	DWORD dwRgbQuadLength = CalcRgbQuadLength();//计算颜色表长度
	if (dwRgbQuadLength == 0)
	{
		return FALSE;//如果长度为0，则不生成逻辑调色板
	}
	if (m_hPalette != nullptr)//删除旧的调色板对象
	{
		DeleteObject(m_hPalette);
		m_hPalette = nullptr;
	}
	//申请缓冲区，初始化为0
	DWORD dwNumOfColor = GetNumOfColor();
	DWORD dwSize = 2 * sizeof(WORD) + dwNumOfColor * sizeof(PALETTEENTRY);
	LPLOGPALETTE lpLogPalette = (LPLOGPALETTE) new BYTE[dwSize];
	memset(lpLogPalette, 0, dwSize);
	//生成逻辑调色板
	lpLogPalette->palVersion = 0x300;
	lpLogPalette->palNumEntries = dwNumOfColor;
	LPRGBQUAD lpRgbQuad = (LPRGBQUAD)m_lpRgbQuad;
	for (int i = 0; i < dwNumOfColor; ++i)
	{
		lpLogPalette->palPalEntry[i].peRed = lpRgbQuad->rgbRed;
		lpLogPalette->palPalEntry[i].peGreen = lpRgbQuad->rgbGreen;
		lpLogPalette->palPalEntry[i].peBlue = lpRgbQuad->rgbBlue;
		lpLogPalette->palPalEntry[i].peFlags = 0;
		++lpRgbQuad;
	}
	m_hPalette = CreatePalette(lpLogPalette);//创建逻辑调色板
	delete[] lpLogPalette;//释放缓冲区
	return TRUE;
}
//=============================================
//函数功能：显示位图
//输入参数：
//		CDC *pDC表示设备环境指针
//		CPoint origin 表示显示矩形区域的左上角
//		CSize size表示显示矩形区域的尺寸
//返回值：
//		BOOL-TRUE表示成功，FALSE表示失败
//==============================================
BOOL CDib::Draw(CDC* pDc, CPoint origin, CSize size)
{
	if (!IsValid()) return FALSE;//位图无效，无法绘制，返回错误
	HPALETTE holdPalette = nullptr;//旧的调色板句柄
	if (m_lpDib == nullptr) return FALSE;//如果位图指针为空，则返回FALSE
	if (m_hPalette != nullptr)  //如果位图有调色板，则选进设备环境中
	{
		holdPalette = SelectPalette(pDc->GetSafeHdc(), m_hPalette, TRUE);
	}
	pDc->SetStretchBltMode(COLORONCOLOR);//设置位图伸缩模式
	//将位图在pDc所指的设备上进行显示
	StretchDIBits(pDc->GetSafeHdc(),origin.x,origin.y,size.cx,size.cy,0,0,
		GetWidth(),GetHeight(),m_lpData,m_lpBmpInfo,DIB_RGB_COLORS,SRCCOPY);
	pDc->TextOut(origin.x + 0.3 * size.cx, origin.y + size.cy + 5, m_title);
	if (holdPalette != nullptr)
	{
		SelectPalette(pDc->GetSafeHdc(), holdPalette, TRUE);
	}
	return TRUE;
}
//=============================================
//函数功能：24位彩色位图转8位灰度位图
//输入参数：无
//返回值：BOOL-TRUE表示成功，FALSE表示失败
//==============================================
BOOL CDib::RgbToGrade()
{
	if (!IsValid())
	{
		return FALSE;//位图无效则返回失败
	}
	if (GetBitCount() != 24)
	{
		return FALSE;//不是24位位图，返回失败
	}
	if (m_lpBmpInfoHeader->biCompression != BI_RGB)
	{
		return FALSE;//压缩位图失败返回
	}
	if (!IsGrade())//如果不是灰度位图，才需要转换
	{
		//获取源位图信息
		LONG lHeight = GetHeight();
		LONG lWidth = GetWidth();
		UINT uLineByte = GetLineByte();
		BYTE r, g, b;
		LPBYTE lpInitBmpData = createGradeBmp(lWidth, lHeight);//灰度位图数据处理
		UINT uGradeBmpLineByte = GetLineByte();
		for (int i = 0; i < lHeight; ++i)
		{
			for (int j = 0; j < lWidth; ++j)
			{
				b = lpInitBmpData[i * uLineByte + 3 * j];
				g = lpInitBmpData[i * uLineByte + 3 * j + 1];
				r = lpInitBmpData[i * uLineByte + 3 * j + 2];
				m_lpData[i * uGradeBmpLineByte + j] = (BYTE)(0.299 * r + 0.587 * g + 0.114 * b);
			}
		}
		delete[]lpInitBmpData;//删除创建的原始数据指针
	}
	
	return TRUE;
}
//=============================================
//函数功能：8为灰度位图转24位彩色位图
//输入参数：无
//返回值：BOOL-TRUE表示成功，FALSE表示失败
//==============================================
BOOL CDib::GradeToRgb()
{
	if (!IsValid()) return FALSE;//位图无效失败退出
	if (GetBitCount() != 8) return FALSE;//不是8位位图失败退出
	if (m_lpBmpInfoHeader->biCompression != BI_RGB) return FALSE;//压缩位图失败返回
	if (IsGrade())//是灰度图时，才需转换
	{
		//获取源位图信息
		LONG lHeight = GetHeight();
		LONG lWidth = GetWidth();
		UINT uLineByte = GetLineByte();
		//计算彩色位图数据所需空间
		
		//彩色位图数据模式
		LPBYTE lpInitBmpData = createColorBmp(lWidth, lHeight);
		UINT uColorBmpLineByte = GetLineByte();
		for (int i = 0; i < lWidth; ++i)
		{
			for (int j = 0; j < lHeight; ++j)
			{
				BYTE btValue = lpInitBmpData[i * uLineByte + j];
				m_lpData[i * uColorBmpLineByte + 3 * j] = btValue;
				m_lpData[i * uColorBmpLineByte + 3 * j + 1] = btValue;
				m_lpData[i * uColorBmpLineByte + 3 * j + 2] = btValue;
			}
		}
		delete[]lpInitBmpData;//删除创建的原始数据指针
	}
	return TRUE;
}
//=============================================
//函数功能：判断是否含有颜色表
//输入参数：无
//返回值：TRUE表示含有颜色表，FALSE表示不含颜色表
//==============================================
BOOL CDib::HasRgbQuad()
{
	return m_bHasRgbQuad;
}
//=============================================
//函数功能：判断是否是灰度图
//输入参数：无
//返回值：TRUE表示是灰度图，FALSE表示是彩色图
//==============================================
BOOL CDib::IsGrade()
{
	return (GetBitCount() < 9 && GetBitCount() > 0);
}
//=============================================
//函数功能：判断是否有效
//输入参数：无
//返回值：TRUE表示位图有效，FALSE表示位图无效
//==============================================
BOOL CDib::IsValid()
{
	return m_bValid;
}
//=============================================
//函数功能：清理空间
//输入参数：BOOL bFlag——TRUE表示全部清空，FALSE表示部分清空
//返回值：无
//==============================================
void CDib::Empty(BOOL bFlag)
{
	if (bFlag) {
		m_fileName = "";//文件名清空
		m_title = "";//标题名清空
	}
	if (m_lpBmpFileHeader != nullptr)
	{
		delete[] m_lpBmpFileHeader;
		m_lpBmpFileHeader = nullptr;
	}//释放位图文件头指针空间
	if (m_lpDib != nullptr)
	{
		m_lpBmpInfo = nullptr;
		m_lpBmpInfoHeader = nullptr;
		m_lpRgbQuad = nullptr;
		m_lpData = nullptr;
		delete[] m_lpDib;
		m_lpDib = nullptr;
	}//释放位图指针空间
	if (m_hPalette != nullptr)
	{
		DeleteObject(m_hPalette);
		m_hPalette = nullptr;
	}//释放调色板
	m_bHasRgbQuad = FALSE;//设置不含颜色表
	m_bValid = FALSE;//设置位图无效

}
//=============================================
//函数功能：图像旋转
//输入参数：float fAngle  旋转角度
//返回值：TRUE or FALSE（是否旋转成功）
//==============================================
void CDib::rotate(float fAngle)
{
	long height = GetHeight();
	long width = GetWidth();
	long lLineBytes = GetLineByte();
	long i, j;
	float fRotateAngle = (float)(fAngle / 180 * PI);
	float fSin = sin(fRotateAngle);
	float fCos = cos(fRotateAngle);
	float srcX1, srcY1, srcX2, srcY2, srcX3, srcY3, srcX4, srcY4;
	float dstX1, dstY1, dstX2, dstY2, dstX3, dstY3, dstX4, dstY4;
	//计算原图像四个角坐标
	srcX1 = (float)(-(width - 1) / 2);
	srcY1 = (float)((height - 1) / 2);
	srcX2 = (float)((width - 1) / 2);
	srcY2 = (float)((height - 1) / 2);
	srcX3 = (float)(-(width - 1) / 2);
	srcY3 = (float)(-(height - 1) / 2);
	srcX4 = (float)((width - 1) / 2);
	srcY4 = (float)(-(height - 1) / 2);
	//计算新图像四个角坐标
	dstX1 = fCos * srcX1 + fSin * srcY1;
	dstY1 = -fSin * srcX1 + fCos * srcY1;
	dstX2 = fCos * srcX2 + fSin * srcY2;
	dstY2 = -fSin * srcX2 + fCos * srcY2;
	dstX3 = fCos * srcX3 + fSin * srcY3;
	dstY3 = -fSin * srcX3 + fCos * srcY3;
	dstX4 = fCos * srcX4 + fSin * srcY4;
	dstY4 = -fSin * srcX4 + fCos * srcY4;
	LONG dstWidth = (LONG)max(max(dstX1, dstX2), max(dstX3, dstX4)) - (LONG)min(min(dstX1, dstX2), min(dstX3, dstX4));//计算旋转后图像的宽度
	LONG dstHeight = (LONG)max(max(dstY1, dstY2), max(dstY3, dstY4)) - (LONG)min(min(dstY1, dstY2), min(dstY3, dstY4));//计算旋转后图像的高度
	//计算新位图大小
	LPBYTE lpTempData, lpSrcBmpData;
	LPBYTE lpDstBmpData;
	LONG dstI, dstJ;
	float varFloat2, varFloat1;
	varFloat1 = (float)(-0.5 * (float)(dstWidth - 1) * fCos - 0.5 * (float)(dstHeight - 1) * fSin + 0.5 * (float)(width - 1));
	varFloat2 = (float)(0.5 * (float)(dstWidth -1) * fSin - 0.5 * (float)(dstHeight - 1) * fCos + 0.5 * (float)(height - 1));
	if (IsGrade())
	{
		lpSrcBmpData = createGradeBmp(dstWidth, dstHeight);
		long dstLineBytes = GetLineByte();
		for (i = 0; i < dstHeight; ++i)//行
		{
			for (j = 0; j < dstWidth; ++j)//列
			{
				//指向新图像第i行，第j列像素的指针
				lpDstBmpData = m_lpData + dstLineBytes * (dstHeight - 1 - i) + j;
				//计算该像素在源DIB中的坐标
				dstI = (long)(-((float)j) * fSin + ((float)i) * fCos + varFloat2 + 0.5);
				dstJ = (long)(((float)j) * fCos + ((float)i) * fSin + varFloat1 + 0.5);
				if ((dstI >= 0) && (dstI < height) && (dstJ >= 0) && (dstJ < width))//判断是否是在源图像内
				{
					*lpDstBmpData = *(lpSrcBmpData + lLineBytes * (height - 1 - dstI) + dstJ);

				}
				else
				{
					*lpDstBmpData = 255;
				}
			}
		}
		delete[] lpSrcBmpData;//删除创建的原始数据指针
	}
	else
	{
		lpSrcBmpData = createColorBmp(dstWidth, dstHeight);
		long dstLineBytes = GetLineByte();
		for (i = 0; i < dstHeight; ++i)//行
		{
			for (j = 0; j < dstWidth; ++j)//列
			{
				//指向新图像第i行，第j列像素的指针
				lpDstBmpData = m_lpData + dstLineBytes * (dstHeight - 1 - i) + 3 * j;
				//计算该像素在源DIB中的坐标
				dstI = (long)(-((float)j) * fSin + ((float)i) * fCos + varFloat2 + 0.5);
				dstJ = (long)(((float)j) * fCos + ((float)i) * fSin + varFloat1 + 0.5);
				if ((dstI >= 0) && (dstI < height) && (dstJ >= 0) && (dstJ < width))//判断是否是在源图像内
				{
					lpTempData = lpSrcBmpData + lLineBytes * (height - 1 - dstI) + 3 * dstJ;
					*lpDstBmpData = *lpTempData;
					*(lpDstBmpData + 1) = *(lpTempData + 1);
					*(lpDstBmpData + 2) = *(lpTempData + 2);
				}
				else
				{
					*lpDstBmpData = 255;
					*(lpDstBmpData + 1) = 255;
					*(lpDstBmpData + 2) = 255;
				}
			}
		}
		delete[] lpSrcBmpData;//删除创建的原始数据指针
	}
}

//=============================================
//函数功能：新建空白灰度位图
//输入参数：位图文件头指针，位图指针，图像高度，图像宽度
//返回值：原图像数据数据指针
//==============================================
LPBYTE CDib::createGradeBmp(LONG width, LONG height)
{
	//将原图像数据备份
	LONG lInitSize = GetHeight() * GetLineByte();
	LPBYTE initData = (LPBYTE) new BYTE[lInitSize];
	memcpy(initData, m_lpData, lInitSize);
	//获取源位图信息
	LONG lHeight = height;
	LONG lWidth = width;
	//计算灰度位图数据所需空间
	UINT uGradeBmpLineByte = (lWidth * 8 / 8 + 3) / 4 * 4;//((width * (depth * 8)) + 31) / 32 * 4;
	DWORD dwGradeBmpDataSize = uGradeBmpLineByte * lHeight;
	//计算灰度位图所需空间
	DWORD dwGradeBmpSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + dwGradeBmpDataSize;
	//设置灰度位图文件头
	LPBITMAPFILEHEADER lpGradeBmpFileHeader = (LPBITMAPFILEHEADER) new BYTE[sizeof(BITMAPFILEHEADER)];
	memset(lpGradeBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	lpGradeBmpFileHeader->bfType = 0x4d42;
	lpGradeBmpFileHeader->bfSize = sizeof(BITMAPFILEHEADER) + dwGradeBmpSize;
	lpGradeBmpFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
	lpGradeBmpFileHeader->bfReserved1 = 0;
	lpGradeBmpFileHeader->bfReserved2 = 0;
	LPBYTE lpGradeBmp = (LPBYTE)new BYTE[dwGradeBmpSize];//位灰度位图分配空间
	memset(lpGradeBmp, 0, dwGradeBmpSize);//初始化为0
	//设置灰度位图信息头
	LPBITMAPINFOHEADER lpGradeBmpInfoHeader = (LPBITMAPINFOHEADER)(lpGradeBmp);
	lpGradeBmpInfoHeader->biBitCount = 8;
	lpGradeBmpInfoHeader->biClrImportant = 0;
	lpGradeBmpInfoHeader->biClrUsed = 256;
	lpGradeBmpInfoHeader->biCompression = BI_RGB;
	lpGradeBmpInfoHeader->biHeight = lHeight;
	lpGradeBmpInfoHeader->biPlanes = 1;
	lpGradeBmpInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
	lpGradeBmpInfoHeader->biSizeImage = dwGradeBmpDataSize;
	lpGradeBmpInfoHeader->biWidth = lWidth;
	lpGradeBmpInfoHeader->biXPelsPerMeter = 0;
	lpGradeBmpInfoHeader->biYPelsPerMeter = 0;
	//设置灰度位图颜色表
	LPRGBQUAD lpGradeBmpRgbQuad = (LPRGBQUAD)(lpGradeBmp + sizeof(BITMAPINFOHEADER));
	//初始化8位灰度图的调色板信息
	LPRGBQUAD lpRgbQuad;
	for (int k = 0; k < 256; ++k)
	{
		lpRgbQuad = (LPRGBQUAD)(lpGradeBmpRgbQuad + k);
		lpRgbQuad->rgbBlue = k;
		lpRgbQuad->rgbGreen = k;
		lpRgbQuad->rgbRed = k;
		lpRgbQuad->rgbReserved = 0;
	}
	BYTE r, g, b;
	LPBYTE lpGradeBmpData = (LPBYTE)(lpGradeBmp + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);//灰度位图数据处理
	Empty(FALSE);//释放原有位图空间
	//重新设定原位图指针指向
	m_lpBmpFileHeader = lpGradeBmpFileHeader;
	m_lpDib = lpGradeBmp;
	m_lpBmpInfo = (LPBITMAPINFO)(lpGradeBmp);
	m_lpBmpInfoHeader = (LPBITMAPINFOHEADER)(lpGradeBmp);
	m_lpRgbQuad = lpGradeBmpRgbQuad;
	m_lpData = lpGradeBmpData;
	m_bHasRgbQuad = TRUE;//设置颜色表标志
	MakePalette();//生成调色板
	m_bValid = TRUE;//设置位图有效标志
	return initData;
}
//=============================================
//函数功能：新建空白彩色位图（需要将原图像数据备份）
//输入参数：位图文件头指针，位图指针，图像高度，图像宽度
//返回值：原图像数据指针
//==============================================
LPBYTE CDib::createColorBmp( LONG width, LONG height)
{
	//将原图像数据备份
	LONG lInitSize= GetHeight()* GetLineByte();
	LPBYTE initData= (LPBYTE) new BYTE[lInitSize];
	memcpy(initData, m_lpData, lInitSize);
	//获取源位图信息
	LONG lHeight = height;
	LONG lWidth = width;
	//计算彩色位图数据所需空间
	UINT uColorBmpLineByte = (lWidth * 24 / 8 + 3) / 4 * 4;
	DWORD  dwColorBmpDataSize = uColorBmpLineByte * lHeight;
	//计算彩色位图所需空间
	DWORD dwColorBmpSize = sizeof(BITMAPINFOHEADER) + dwColorBmpDataSize;
	//设置彩色位图文件头
	LPBITMAPFILEHEADER lpColorBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
	memset(lpColorBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	lpColorBmpFileHeader->bfType = 0x4d42;
	lpColorBmpFileHeader->bfSize = sizeof(BITMAPFILEHEADER) + dwColorBmpSize;
	lpColorBmpFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPFILEHEADER);
	lpColorBmpFileHeader->bfReserved1 = 0;
	lpColorBmpFileHeader->bfReserved2 = 0;
	//为彩色位图分配空间，并初始化为0
	LPBYTE lpColorBmp = (LPBYTE) new BYTE[dwColorBmpSize];
	memset(lpColorBmp, 0, dwColorBmpSize);
	//设置彩色位图信息头
	LPBITMAPINFOHEADER lpColorBmpInfoHeader = (LPBITMAPINFOHEADER)(lpColorBmp);
	lpColorBmpInfoHeader->biBitCount = 24;
	lpColorBmpInfoHeader->biClrImportant = 0;
	lpColorBmpInfoHeader->biClrUsed = 0;
	lpColorBmpInfoHeader->biCompression = BI_RGB;
	lpColorBmpInfoHeader->biHeight = lHeight;
	lpColorBmpInfoHeader->biPlanes = 1;
	lpColorBmpInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
	lpColorBmpInfoHeader->biSizeImage = dwColorBmpDataSize;
	lpColorBmpInfoHeader->biWidth = lWidth;
	lpColorBmpInfoHeader->biXPelsPerMeter = m_lpBmpInfoHeader->biXPelsPerMeter;
	lpColorBmpInfoHeader->biYPelsPerMeter = m_lpBmpInfoHeader->biYPelsPerMeter;
	//彩色位图数据模式
	LPBYTE lpColorBmpData = (LPBYTE)(lpColorBmp + sizeof(BITMAPINFOHEADER));
	Empty(FALSE);//释放原有位图空间
	//重新设定源位图指针指向
	m_lpBmpFileHeader = lpColorBmpFileHeader;
	m_lpDib = lpColorBmp;
	m_lpBmpInfo = (LPBITMAPINFO)(lpColorBmp);
	m_lpBmpInfoHeader = lpColorBmpInfoHeader;
	m_lpRgbQuad = nullptr;
	m_lpData = lpColorBmpData;
	m_bHasRgbQuad = FALSE;//设置颜色表标志
	m_bValid = TRUE;//设置位图有效标志
	return initData;
}



