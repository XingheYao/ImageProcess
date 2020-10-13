//#include "StdAfx.h"

#include "pch.h"

#include "Dib.h"

//================================
//�������ܣ����캯������ʼ�����ݳ�Ա
//================================
CDib::CDib(void)
{
	//���ݳ�Ա��ʼ��
	m_title = "ԭʼͼ��";
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
	//Empty(TRUE);//����ռ�
	//Ϊλͼ�ļ�ͷ����ռ䣬����ʼ��Ϊ0
	m_lpBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
	memset(m_lpBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	memcpy(m_lpBmpFileHeader, rhs.m_lpBmpFileHeader, sizeof(BITMAPFILEHEADER));//����λͼ�ļ�ͷ
	if (m_lpBmpFileHeader->bfType == 0x4d42)//�жϴ��ļ��Ƿ�λͼ��0x4d42����BMP
	{

		int dwDibSize = m_lpBmpFileHeader->bfSize - sizeof(BITMAPFILEHEADER);
		m_lpDib = (LPBYTE)new BYTE[dwDibSize];
		memset(m_lpDib, 0, dwDibSize);
		memcpy(m_lpDib, rhs.m_lpDib, dwDibSize);

		m_lpBmpInfo = (LPBITMAPINFO)m_lpDib;//����λͼ��Ϣָ��
		m_lpBmpInfoHeader = (LPBITMAPINFOHEADER)m_lpDib;//����λͼ��Ϣͷָ��
		m_lpRgbQuad = (LPRGBQUAD)(m_lpDib + m_lpBmpInfoHeader->biSize);//��ɫ��ָ��
		if (m_lpBmpInfoHeader->biClrUsed == 0)
		{
			m_lpBmpInfoHeader->biClrUsed = GetNumOfColor();
		}
		DWORD dwRgbQuadLength = CalcRgbQuadLength();//������ɫ����

		m_lpData = m_lpDib + m_lpBmpInfoHeader->biSize + dwRgbQuadLength;//����ָ��
		if (m_lpRgbQuad == (LPRGBQUAD)m_lpData)
		{
			m_lpRgbQuad = nullptr;//��λͼ��ɫ��ָ���ÿ�
			m_bHasRgbQuad = FALSE;//����ɫ��
		}
		else
		{
			m_bHasRgbQuad = TRUE;//����ɫ��
			MakePalette();//������ɫ�����ɵ�ɫ��
		}
		m_lpBmpInfoHeader->biSizeImage = GetSize();//����λͼ��С���ܶ��ļ���δ���ô���
		m_bValid = TRUE;//λͼ��Ч
	}
	m_fileName = rhs.m_fileName;
}
//====================================
//�������ܣ������������ͷ��ڴ�ռ�
//====================================
CDib::~CDib(void)
{
	Empty();

}
//=====================================
//�������ܣ����ļ�����bmpλͼ
//���뺯����LPCTSTR lpszPath��ʾ������λͼ�ļ�·��
//����ֵ��BOOL-TRUE��ʾ�ɹ���FALSE��ʾʧ��
//=====================================
BOOL CDib::LoadFromFile(CString lpszPath)
{
	m_fileName = lpszPath;
	CFile dibFile;
	//�Զ�ģʽ��λͼ�ļ�
	if (!dibFile.Open(/*(LPCTSTR)*/m_fileName,CFile::modeRead|CFile::shareDenyWrite))
	{
		return FALSE;
	}
	Empty(FALSE);//����ռ�
	//Ϊλͼ�ļ�ͷ����ռ䣬����ʼ��Ϊ0
	m_lpBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
	memset(m_lpBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	//��ȡλͼ�ļ�ͷ
	int nCount = dibFile.Read((void*)m_lpBmpFileHeader, sizeof(BITMAPFILEHEADER));
	if(nCount != sizeof(BITMAPFILEHEADER))
	{
		return FALSE;
	}
	if (m_lpBmpFileHeader->bfType == 0x4d42)//�жϴ��ļ��Ƿ�λͼ��0x4d42����BMP
	{
		//����λͼ�ļ��������λͼ�ļ�ͷ�Ŀռ��С������ռ䲢��ʼ��Ϊ0
		DWORD dwDibSize = dibFile.GetLength() - sizeof(BITMAPFILEHEADER);
		m_lpDib = (LPBYTE)new BYTE[dwDibSize];
		memset(m_lpDib, 0, dwDibSize);
		dibFile.Read(m_lpDib, dwDibSize);//��ȡ��λͼ�ļ�ͷ����������
		dibFile.Close();//�ر�λͼ�ļ�
		m_lpBmpInfo = (LPBITMAPINFO)m_lpDib;//����λͼ��Ϣָ��
		m_lpBmpInfoHeader = (LPBITMAPINFOHEADER)m_lpDib;//����λͼ��Ϣͷָ��
		m_lpRgbQuad = (LPRGBQUAD)(m_lpDib + m_lpBmpInfoHeader->biSize);//��ɫ��ָ��
		if (m_lpBmpInfoHeader->biClrUsed == 0)
		{
			m_lpBmpInfoHeader->biClrUsed = GetNumOfColor();
		}
		DWORD dwRgbQuadLength = CalcRgbQuadLength();//������ɫ����

		m_lpData = m_lpDib + m_lpBmpInfoHeader->biSize + dwRgbQuadLength;//����ָ��
		if (m_lpRgbQuad == (LPRGBQUAD)m_lpData)
		{
			m_lpRgbQuad = nullptr;//��λͼ��ɫ��ָ���ÿ�
			m_bHasRgbQuad = FALSE;//����ɫ��
		}
		else
		{
			m_bHasRgbQuad = TRUE;//����ɫ��
			MakePalette();//������ɫ�����ɵ�ɫ��
		}
		m_lpBmpInfoHeader->biSizeImage = GetSize();//����λͼ��С���ܶ��ļ���δ���ô���
		m_bValid = TRUE;//λͼ��Ч
		return TRUE;
	}
	else
	{
		m_bValid = FALSE;//����λͼ�ļ�
		return FALSE;
	}
}
//=====================================
//�������ܣ����ļ�����bmpλͼ
//���뺯����LPCTSTR lpszPath��ʾ������λͼ�ļ�·��
//����ֵ��BOOL-TRUE��ʾ�ɹ���FALSE��ʾʧ��
//=====================================
BOOL CDib::LoadJPGFromFile(CString lpszPath)
{
	//��JPG�ļ�
	m_fileName = lpszPath;
	FILE* JPGFile;
	//�Զ�ģʽ��λͼ�ļ�
	errno_t err;
	if ((err = _tfopen_s(&JPGFile, lpszPath, _T("rb")))!= 0)
	{
		return FALSE;
	}
	Empty(FALSE);//����ռ�

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPARRAY buffer;
	LPBYTE src_buff;
	LPBYTE point;

	cinfo.err = jpeg_std_error(&jerr);    //һ��Ϊlibjpeg����������ο�����ĵ�
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, JPGFile);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	unsigned long width = cinfo.output_width;
	unsigned long height = cinfo.output_height;
	unsigned short depth = cinfo.output_components;
	unsigned long headersize;
	unsigned long filesize;
	DWORD dwDibSize;

	src_buff = new BYTE[width * height * depth];
	memset(src_buff, 0, sizeof(unsigned char) * width * height * depth);

	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr)&cinfo, JPOOL_IMAGE, width * depth, 1);

	point = src_buff;
	while (cinfo.output_scanline < height)
	{
		jpeg_read_scanlines(&cinfo, buffer, 1);    //��ȡһ��jpgͼ�����ݵ�buffer
		memcpy(point, *buffer, width * depth);    //��buffer�е��������и�src_buff
		point += width * depth;            //һ�θı�һ��
	}

	//д��bmp_Header
	if (depth == 1)
	{
		headersize = 14 + 40 + 256 * 4;
		filesize = headersize + width * height;
	}

	if (depth == 3)
	{
		headersize = 14 + 40;
		filesize = headersize + width * height * depth;
	}
	//����λͼ�ļ�ͷ
	m_lpBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
	memset(m_lpBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	m_lpBmpFileHeader->bfType = 0x4D42;
	m_lpBmpFileHeader->bfSize = filesize;
	m_lpBmpFileHeader->bfOffBits = headersize;
	//�����λͼ�ļ�ͷ�Ŀռ��С������ռ䲢��ʼ��Ϊ0
	dwDibSize = filesize - 14;
	m_lpDib = (LPBYTE)new BYTE[dwDibSize];
	memset(m_lpDib, 0, dwDibSize);

	m_lpBmpInfo = (LPBITMAPINFO)m_lpDib;//����λͼ��Ϣָ��
	m_lpBmpInfoHeader = (LPBITMAPINFOHEADER)m_lpDib;//����λͼ��Ϣͷָ��
	m_lpBmpInfoHeader->biSize = 40;
	m_lpBmpInfoHeader->biWidth = width;
	m_lpBmpInfoHeader->biHeight = height;
	m_lpBmpInfoHeader->biPlanes = 1;
	m_lpBmpInfoHeader->biBitCount = (unsigned short)depth * 8;
	m_lpBmpInfoHeader->biSizeImage = width * height * depth;
	//m_lpBmpInfoHeader->biXPelsPerMeter = 3780;
	//m_lpBmpInfoHeader->biYPelsPerMeter = 3780;


	if (depth == 1)//�Ҷ�ͼ��Ҫ��ӵ�ɫ��
	{
		CString seedBmp("seed.bmp");
		CFile seedFile;
		//�Զ�ģʽ��λͼ�ļ�
		if (!seedFile.Open(seedBmp, CFile::modeRead | CFile::shareDenyWrite))
		{
			return FALSE;
		}
		//����λͼ�ļ��������λͼ�ļ�ͷ�Ŀռ��С������ռ䲢��ʼ��Ϊ0
		DWORD dwDibSize = seedFile.GetLength() - sizeof(BITMAPFILEHEADER);
		LPBYTE lpSeedDib = (LPBYTE)new BYTE[dwDibSize];
		memset(lpSeedDib, 0, dwDibSize);
		seedFile.Read(lpSeedDib, dwDibSize);//��ȡ��λͼ�ļ�ͷ����������
		seedFile.Close();//�ر�λͼ�ļ�
		LPBITMAPINFOHEADER lpSeedInfoHeader = (LPBITMAPINFOHEADER)lpSeedDib;//����λͼ��Ϣͷָ��
		m_lpRgbQuad = (LPRGBQUAD)(lpSeedDib + lpSeedInfoHeader->biSize);//��ɫ��ָ��
		if (lpSeedInfoHeader->biClrUsed == 0)
		{
			lpSeedInfoHeader->biClrUsed = GetNumOfColor();
		}
		if (m_lpRgbQuad == (LPRGBQUAD)lpSeedDib)
		{
			m_lpRgbQuad = nullptr;//��λͼ��ɫ��ָ���ÿ�
			m_bHasRgbQuad = FALSE;//����ɫ��
		}
		else
		{
			m_bHasRgbQuad = TRUE;//����ɫ��
			MakePalette();//������ɫ�����ɵ�ɫ��
		}
		delete [] lpSeedDib;
	}
	else
	{
		m_lpRgbQuad = nullptr;//��λͼ��ɫ��ָ���ÿ�
		m_bHasRgbQuad = FALSE;//����ɫ��
	}
	DWORD dwRgbQuadLength = CalcRgbQuadLength();//������ɫ����
	//д��bmp_Data
	m_lpData = m_lpDib + 40 + dwRgbQuadLength;//����ָ��
	point = src_buff + width * depth * (height - 1);    //����д���ݣ�bmp��ʽ�ǵ��ģ�jpg������
	for (unsigned long i = 0; i < height; i++)
	{
		for (unsigned long j = 0; j < width * depth; j += depth)
		{
			if (depth == 1)        //����Ҷ�ͼ
			{
				m_lpData[j] = point[j];
			}

			if (depth == 3)        //�����ɫͼ
			{
				m_lpData[j + 2] = point[j + 0];//R
				m_lpData[j + 1] = point[j + 1];//G
				m_lpData[j + 0] = point[j + 2];//B
			}
		}
		point -= width * depth;
	}
	m_bValid = TRUE;//λͼ��Ч
	
	//�����ʱ����
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	delete[] src_buff;
	fclose(JPGFile);

	return TRUE;
}
//============================================
//�������ܣ���λͼ���浽�ļ�
//���������LPCTSTR lpszPath��ʾλͼ�ļ�����·��
//����ֵ��BOOL-TRUE��ʾ�ɹ���FALSE��ʾʧ��
//============================================
BOOL CDib::SaveToFile(CString lpszPath)
{
	CFile dibFile;//��дģʽ���ļ�
	if (!dibFile.Open(lpszPath, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive))
	{
		return FALSE;
	}
	m_fileName=lpszPath;
	dibFile.Write(m_lpBmpFileHeader, sizeof(BITMAPFILEHEADER));//���ļ�ͷ�ṹд���ļ�
	dibFile.Write(m_lpBmpInfoHeader, sizeof(BITMAPINFOHEADER));//����Ϣ�ṹͷ�ļ�д���ļ�
	DWORD dwRgbQuadLength = CalcRgbQuadLength();
	if (dwRgbQuadLength != 0)
	{
		dibFile.Write(m_lpRgbQuad, dwRgbQuadLength);
	}
	DWORD dwDataSize = GetLineByte() * GetHeight();
	dibFile.Write(m_lpData, dwDataSize);//��λͼ����д���ļ�
	dibFile.Close();//�ر��ļ�
	return TRUE;
}
//=============================================
//�������ܣ���ȡλͼ�ļ���
//�����������
//����ֵ��LPCTSTR��ʾλͼ�ļ���
//==============================================
CString CDib::GetFileName()
{
	return m_fileName;
}
//=============================================
//�������ܣ���ȡλͼ���
//�����������
//����ֵ��LONG ��ʾλͼ���
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
//�������ܣ���ȡλͼ�߶�
//�����������
//����ֵ��LONG��ʾλͼ�߶�
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
//�������ܣ���ȡλͼ�ĸ߶ȺͿ��
//�����������
//����ֵ��CSize��ʾλͼ�ĸ߶ȺͿ��
//==============================================
CSize CDib::GetDimension()
{
	return CSize(GetWidth(), GetHeight());
}
//=============================================
//�������ܣ���ȡλͼ��С
//����������� 
//����ֵ��DWORD��ʾλͼ��С
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
//�������ܣ���ȡ����������ռλ��
//�����������
//����ֵ��WORD��ʾ����������ռλ��
//==============================================
WORD CDib::GetBitCount()
{
	return m_lpBmpInfoHeader->biBitCount;
}
//=============================================
//�������ܣ���ȡÿ��������ռ�ֽ���
//�����������
//����ֵ��UINT��ʾÿ��������ռ�ֽ���
//==============================================
UINT CDib::GetLineByte()
{
	return (GetWidth() * GetBitCount() / 8 + 3) / 4 * 4;
}
//=============================================
//�������ܣ���ȡλͼ��ɫ��
//�����������
//����ֵ��DWORD��ʾλͼ��ɫ��
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
//�������ܣ�����λͼ��ɫ����
//�����������
//����ֵ��DWORD��ʾλͼ��ɫ����
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
//�������ܣ���ȡλͼ��ɫ��
//�����������
//����ֵ��LPRGBQUAD��ʾλͼ��ɫ��ָ��
//==============================================
LPRGBQUAD CDib::GetRgbQuad()
{
	return m_lpRgbQuad;
}
//=============================================
//�������ܣ���ȡλͼ����
//�����������
//����ֵ��LPBYTE��ʾλͼ����ָ��
//==============================================
LPBYTE CDib::GetData()
{
	return m_lpData;
}
//=============================================
//�������ܣ�������ɫ�����ɵ�ɫ��
//�����������
//����ֵ��BOOL-TRUE��ʾ�ɹ���FALSE��ʾʧ��
//==============================================
BOOL CDib::MakePalette()
{
	DWORD dwRgbQuadLength = CalcRgbQuadLength();//������ɫ����
	if (dwRgbQuadLength == 0)
	{
		return FALSE;//�������Ϊ0���������߼���ɫ��
	}
	if (m_hPalette != nullptr)//ɾ���ɵĵ�ɫ�����
	{
		DeleteObject(m_hPalette);
		m_hPalette = nullptr;
	}
	//���뻺��������ʼ��Ϊ0
	DWORD dwNumOfColor = GetNumOfColor();
	DWORD dwSize = 2 * sizeof(WORD) + dwNumOfColor * sizeof(PALETTEENTRY);
	LPLOGPALETTE lpLogPalette = (LPLOGPALETTE) new BYTE[dwSize];
	memset(lpLogPalette, 0, dwSize);
	//�����߼���ɫ��
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
	m_hPalette = CreatePalette(lpLogPalette);//�����߼���ɫ��
	delete [] lpLogPalette;//�ͷŻ�����
	return TRUE;
}
//=============================================
//�������ܣ���ʾλͼ
//���������
//		CDC *pDC��ʾ�豸����ָ��
//		CPoint origin ��ʾ��ʾ������������Ͻ�
//		CSize size��ʾ��ʾ��������ĳߴ�
//����ֵ��
//		BOOL-TRUE��ʾ�ɹ���FALSE��ʾʧ��
//==============================================
BOOL CDib::Draw(CDC* pDc, CPoint origin, CSize size)
{
	if (!IsValid()) return FALSE;//λͼ��Ч���޷����ƣ����ش���
	HPALETTE holdPalette = nullptr;//�ɵĵ�ɫ����
	if (m_lpDib == nullptr) return FALSE;//���λͼָ��Ϊ�գ��򷵻�FALSE
	if (m_hPalette != nullptr)  //���λͼ�е�ɫ�壬��ѡ���豸������
	{
		holdPalette = SelectPalette(pDc->GetSafeHdc(), m_hPalette, TRUE);
	}
	pDc->SetStretchBltMode(COLORONCOLOR);//����λͼ����ģʽ
	//��λͼ��pDc��ָ���豸�Ͻ�����ʾ
	StretchDIBits(pDc->GetSafeHdc(),origin.x,origin.y,size.cx,size.cy,0,0,
		GetWidth(),GetHeight(),m_lpData,m_lpBmpInfo,DIB_RGB_COLORS,SRCCOPY);
	pDc->TextOut(origin.x+0.3*size.cx, size.cy+5, m_title);
	if (holdPalette != nullptr)
	{
		SelectPalette(pDc->GetSafeHdc(), holdPalette, TRUE);
	}
	return TRUE;
}
//=============================================
//�������ܣ�24λ��ɫλͼת8λ�Ҷ�λͼ
//�����������
//����ֵ��BOOL-TRUE��ʾ�ɹ���FALSE��ʾʧ��
//==============================================
BOOL CDib::RgbToGrade()
{
	if (!IsValid())
	{
		return FALSE;//λͼ��Ч�򷵻�ʧ��
	}
	if (GetBitCount() != 24)
	{
		return FALSE;//����24λλͼ������ʧ��
	}
	if (m_lpBmpInfoHeader->biCompression != BI_RGB)
	{
		return FALSE;//ѹ��λͼʧ�ܷ���
	}
	if (!IsGrade())//������ǻҶ�λͼ������Ҫת��
	{
		//��ȡԴλͼ��Ϣ
		LONG lHeight = GetHeight();
		LONG lWidth = GetWidth();
		UINT uLineByte = GetLineByte();
		//����Ҷ�λͼ��������ռ�
		UINT uGradeBmpLineByte = (lWidth + 3) / 4 * 4;
		DWORD dwGradeBmpDataSize = uGradeBmpLineByte * lHeight;
		//����Ҷ�λͼ����ռ�
		DWORD dwGradeBmpSize = sizeof(LPBITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 +
			dwGradeBmpDataSize;
		//���ûҶ�λͼ�ļ�ͷ
		LPBITMAPFILEHEADER lpGradeBmpFileHeader = (LPBITMAPFILEHEADER) new BYTE[sizeof(BITMAPFILEHEADER)];
		memset(lpGradeBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
		lpGradeBmpFileHeader->bfType = 0x4d42;
		lpGradeBmpFileHeader->bfSize = sizeof(BITMAPFILEHEADER) + dwGradeBmpSize;
		lpGradeBmpFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
		lpGradeBmpFileHeader->bfReserved1 = 0;
		lpGradeBmpFileHeader->bfReserved2 = 0;
		LPBYTE lpGradeBmp = (LPBYTE)new BYTE[dwGradeBmpSize];//λ�Ҷ�λͼ����ռ�
		memset(lpGradeBmp, 0, dwGradeBmpSize);//��ʼ��Ϊ0
		//���ûҶ�λͼ��Ϣͷ
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
		lpGradeBmpInfoHeader->biXPelsPerMeter = m_lpBmpInfoHeader->biXPelsPerMeter;
		lpGradeBmpInfoHeader->biYPelsPerMeter = m_lpBmpInfoHeader->biYPelsPerMeter;
		//���ûҶ�λͼ��ɫ��
		LPRGBQUAD lpGradeBmpRgbQuad = (LPRGBQUAD)(lpGradeBmp + sizeof(BITMAPINFOHEADER));
		//��ʼ��8λ�Ҷ�ͼ�ĵ�ɫ����Ϣ
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
		LPBYTE lpGradeBmpData = (LPBYTE)(lpGradeBmp + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);//�Ҷ�λͼ���ݴ���
		for (int i = 0; i < lHeight; ++i)
		{
			for (int j = 0; j < lWidth; ++j)
			{
				b = m_lpData[i * uLineByte + 3 * j];
				g = m_lpData[i * uLineByte + 3 * j + 1];
				r = m_lpData[i * uLineByte + 3 * j + 2];
				lpGradeBmpData[i * uGradeBmpLineByte + j] = (BYTE)(0.299 * r + 0.587 * g + 0.114 * b);
			}
		}
		this->Empty(FALSE);//�ͷ�ԭ��λͼ�ռ�
		//�����趨ԭλͼָ��ָ��
		m_lpBmpFileHeader = lpGradeBmpFileHeader;
		m_lpDib = lpGradeBmp;
		m_lpBmpInfo = (LPBITMAPINFO)(lpGradeBmp);
		m_lpBmpInfoHeader = (LPBITMAPINFOHEADER)(lpGradeBmp);
		m_lpRgbQuad = lpGradeBmpRgbQuad;
		m_lpData = lpGradeBmpData;//������ɫ���־
		m_bHasRgbQuad = TRUE;//����λͼ��Ч��־
		MakePalette();//���ɵ�ɫ��
	}
	return TRUE;
}
//=============================================
//�������ܣ�8Ϊ�Ҷ�λͼת24λ��ɫλͼ
//�����������
//����ֵ��BOOL-TRUE��ʾ�ɹ���FALSE��ʾʧ��
//==============================================
BOOL CDib::GradeToRgb()
{
	if (!IsValid()) return FALSE;//λͼ��Чʧ���˳�
	if (GetBitCount() != 8) return FALSE;//����8λλͼʧ���˳�
	if (m_lpBmpInfoHeader->biCompression != BI_RGB) return FALSE;//ѹ��λͼʧ�ܷ���
	if (IsGrade())//�ǻҶ�ͼʱ������ת��
	{
		//��ȡԴλͼ��Ϣ
		LONG lHeight = GetHeight();
		LONG lWidth = GetWidth();
		UINT uLineByte = GetLineByte();
		//�����ɫλͼ��������ռ�
		UINT uColorBmpLineByte = (lWidth * 24 / 8 + 3) / 4 * 4;
		DWORD  dwColorBmpDataSize= uColorBmpLineByte * lHeight;
		//�����ɫλͼ����ռ�
		DWORD dwColorBmpSize = sizeof(BITMAPINFOHEADER) + dwColorBmpDataSize;
		//���ò�ɫλͼ�ļ�ͷ
		LPBITMAPFILEHEADER lpColorBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
		memset(lpColorBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
		lpColorBmpFileHeader->bfType = 0x4d42;
		lpColorBmpFileHeader->bfSize = sizeof(BITMAPFILEHEADER) + dwColorBmpSize;
		lpColorBmpFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPFILEHEADER);
		lpColorBmpFileHeader->bfReserved1 = 0;
		lpColorBmpFileHeader->bfReserved2 = 0;
		//Ϊ��ɫλͼ����ռ䣬����ʼ��Ϊ0��
		LPBYTE lpColorBmp = (LPBYTE) new BYTE[dwColorBmpSize];
		memset(lpColorBmp, 0, dwColorBmpSize);
		//���ò�ɫλͼ��Ϣͷ
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
		//��ɫλͼ����ģʽ
		LPBYTE lpColorBmpData = (LPBYTE)(lpColorBmp + sizeof(BITMAPINFOHEADER));
		for (int i = 0; i < lWidth; ++i)
		{
			for (int j = 0; j < lHeight; ++j)
			{
				BYTE btValue = m_lpData[i * uLineByte + j];
				lpColorBmpData[i * uColorBmpLineByte + 3 * j] = btValue;
				lpColorBmpData[i * uColorBmpLineByte + 3 * j + 1] = btValue;
				lpColorBmpData[i * uColorBmpLineByte + 3 * j + 2] = btValue;
			}
		}
		this->Empty(FALSE);//�ͷ�ԭ��λͼ�ռ�
		//�����趨Դλͼָ��ָ��
		m_lpBmpFileHeader = lpColorBmpFileHeader;
		m_lpDib = lpColorBmp;
		m_lpBmpInfo = (LPBITMAPINFO)(lpColorBmp);
		m_lpBmpInfoHeader = lpColorBmpInfoHeader;
		m_lpRgbQuad = nullptr;
		m_lpData = lpColorBmpData;
		m_bHasRgbQuad = FALSE;//������ɫ���־
		m_bValid = TRUE;//����λͼ��Ч��־
	}
	return TRUE;
}
//=============================================
//�������ܣ��ж��Ƿ�����ɫ��
//�����������
//����ֵ��TRUE��ʾ������ɫ��FALSE��ʾ������ɫ��
//==============================================
BOOL CDib::HasRgbQuad()
{
	return m_bHasRgbQuad;
}
//=============================================
//�������ܣ��ж��Ƿ��ǻҶ�ͼ
//�����������
//����ֵ��TRUE��ʾ�ǻҶ�ͼ��FALSE��ʾ�ǲ�ɫͼ
//==============================================
BOOL CDib::IsGrade()
{
	return (GetBitCount() < 9 && GetBitCount() > 0);
}
//=============================================
//�������ܣ��ж��Ƿ���Ч
//�����������
//����ֵ��TRUE��ʾλͼ��Ч��FALSE��ʾλͼ��Ч
//==============================================
BOOL CDib::IsValid()
{
	return m_bValid;
}
//=============================================
//�������ܣ�����ռ�
//���������BOOL bFlag����TRUE��ʾȫ����գ�FALSE��ʾ�������
//����ֵ����
//==============================================
void CDib::Empty(BOOL bFlag)
{
	if (bFlag) {
		m_fileName = "";//�ļ������
		m_title = "";//���������
	}
	if (m_lpBmpFileHeader != nullptr)
	{
		delete[] m_lpBmpFileHeader;
		m_lpBmpFileHeader = nullptr;
	}//�ͷ�λͼ�ļ�ͷָ��ռ�
	if (m_lpDib != nullptr)
	{
		delete[] m_lpDib;
		m_lpDib = nullptr;
		m_lpBmpInfo = nullptr;
		m_lpBmpInfoHeader = nullptr;
		m_lpRgbQuad = nullptr;
		m_lpData = nullptr;
	}//�ͷ�λͼָ��ռ�
	if (m_hPalette != nullptr)
	{
		DeleteObject(m_hPalette);
		m_hPalette = nullptr;
	}//�ͷŵ�ɫ��
	m_bHasRgbQuad = FALSE;//���ò�����ɫ��
	m_bValid = FALSE;//����λͼ��Ч

}

//=============================================
//�������ܣ�"="���������
//���������
//����ֵ����
//==============================================
CDib& CDib::operator=(const CDib& rhs) 
{
	Empty(TRUE);//����ռ�
	//Ϊλͼ�ļ�ͷ����ռ䣬����ʼ��Ϊ0
	m_lpBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
	memset(m_lpBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	memcpy(m_lpBmpFileHeader, rhs.m_lpBmpFileHeader, sizeof(BITMAPFILEHEADER));//����λͼ�ļ�ͷ
	if (m_lpBmpFileHeader->bfType == 0x4d42)//�жϴ��ļ��Ƿ�λͼ��0x4d42����BMP
	{
		
		int dwDibSize = m_lpBmpFileHeader->bfSize - sizeof(BITMAPFILEHEADER);
		m_lpDib = (LPBYTE)new BYTE[dwDibSize];
		memset(m_lpDib, 0, dwDibSize);
		memcpy(m_lpDib, rhs.m_lpDib, dwDibSize);

		m_lpBmpInfo = (LPBITMAPINFO)m_lpDib;//����λͼ��Ϣָ��
		m_lpBmpInfoHeader = (LPBITMAPINFOHEADER)m_lpDib;//����λͼ��Ϣͷָ��
		m_lpRgbQuad = (LPRGBQUAD)(m_lpDib + m_lpBmpInfoHeader->biSize);//��ɫ��ָ��
		if (m_lpBmpInfoHeader->biClrUsed == 0)
		{
			m_lpBmpInfoHeader->biClrUsed = GetNumOfColor();
		}
		DWORD dwRgbQuadLength = CalcRgbQuadLength();//������ɫ����

		m_lpData = m_lpDib + m_lpBmpInfoHeader->biSize + dwRgbQuadLength;//����ָ��
		if (m_lpRgbQuad == (LPRGBQUAD)m_lpData)
		{
			m_lpRgbQuad = nullptr;//��λͼ��ɫ��ָ���ÿ�
			m_bHasRgbQuad = FALSE;//����ɫ��
		}
		else
		{
			m_bHasRgbQuad = TRUE;//����ɫ��
			MakePalette();//������ɫ�����ɵ�ɫ��
		}
		m_lpBmpInfoHeader->biSizeImage = GetSize();//����λͼ��С���ܶ��ļ���δ���ô���
		m_bValid = TRUE;//λͼ��Ч
	}
	m_fileName = rhs.m_fileName;
	return *this;
}







