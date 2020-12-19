//#include "StdAfx.h"

#include "pch.h"

#include "Dib.h"

//================================
//�������ܣ����캯������ʼ�����ݳ�Ա
//================================
CDib::CDib(void)
{
	//���ݳ�Ա��ʼ��
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
	m_lpDData = nullptr;
}
CDib::CDib(CDib& rhs)
{
	m_lpDData = nullptr;
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
//=====================================
//�������ܣ����ļ�����bmpλͼ
//���뺯����LPCTSTR lpszPath��ʾ������λͼ�ļ�·��
//����ֵ��BOOL-TRUE��ʾ�ɹ���FALSE��ʾʧ��
//=====================================
BOOL CDib::LoadFromFile(CString lpszPath)
{
	m_title = "ԭʼͼ��";
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
	m_title = "ԭʼͼ��";
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

	struct jpeg_decompress_struct cinfo;//����JPEG�ļ��Ľ�ѹ��Ϣ
	struct jpeg_error_mgr jerr;//����JPEG�ļ��Ĵ�����Ϣ
	JSAMPARRAY buffer;	//���建����
	LPBYTE lpJPGData;
	LPBYTE lpDataPoint;

	cinfo.err = jpeg_std_error(&jerr);    //libjpeg����������ο�����ĵ�
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
	double A = 1.00;
	buffer = (*cinfo.mem->alloc_sarray)
	((j_common_ptr)&cinfo, JPOOL_IMAGE, uJPGLineByte, 1);

	lpDataPoint = lpJPGData;


	while (cinfo.output_scanline < height)
	{
		jpeg_read_scanlines(&cinfo, buffer, 1);    //��ȡһ��jpgͼ�����ݵ�buffer
		memcpy(lpDataPoint, *buffer, uJPGLineByte);    //��buffer�е��������и�lpJPGData
		lpDataPoint += uJPGLineByte;            //һ�θı�һ��
	}

	//д��bmp_Header
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
	//����λͼ�ļ�ͷ
	m_lpBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
	memset(m_lpBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	m_lpBmpFileHeader->bfType = 0x4D42;
	m_lpBmpFileHeader->bfSize = fileSize;
	m_lpBmpFileHeader->bfOffBits = headerSize;
	//�����λͼ�ļ�ͷ�Ŀռ��С������ռ䲢��ʼ��Ϊ0
	dwDibSize = fileSize - 14;

	m_lpDib = (LPBYTE)new BYTE[dwDibSize];
	memset(m_lpDib, 0, dwDibSize);
	//����λͼ��Ϣͷ
	m_lpBmpInfo = (LPBITMAPINFO)m_lpDib;//����λͼ��Ϣָ��
	m_lpBmpInfoHeader = (LPBITMAPINFOHEADER)m_lpDib;//����λͼ��Ϣͷָ��
	m_lpBmpInfoHeader->biSize = 40;
	m_lpBmpInfoHeader->biWidth = width;
	m_lpBmpInfoHeader->biHeight = height;
	m_lpBmpInfoHeader->biPlanes = 1;
	m_lpBmpInfoHeader->biBitCount = depth * 8;
	m_lpBmpInfoHeader->biSizeImage = dwBMPDataSize;
	m_lpBmpInfoHeader->biXPelsPerMeter = 0;
	m_lpBmpInfoHeader->biYPelsPerMeter = 0;


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
			m_hPalette = nullptr;
			m_bHasRgbQuad = TRUE;//����ɫ��
			MakePalette();//������ɫ�����ɵ�ɫ��
		}
		delete[] lpSeedDib;
	}
	else
	{
		m_lpRgbQuad = nullptr;//��λͼ��ɫ��ָ���ÿ�
		m_bHasRgbQuad = FALSE;//����ɫ��
	}
	DWORD dwRgbQuadLength = CalcRgbQuadLength();//������ɫ����
	//д��bmp_Data
	m_lpData = m_lpDib + 40 + dwRgbQuadLength;//����ָ��
	DWORD startPosition = uJPGLineByte * (height - 1);
	lpDataPoint = lpJPGData + startPosition;    //����д���ݣ�bmp��ʽ�ǵ��ģ�jpg������
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width ; j++)
		{
			if (depth == 1)        //����Ҷ�ͼ
			{
				m_lpData[i * uBmpLineByte + j] = lpDataPoint[j];
			}

			if (depth == 3)        //�����ɫͼ
			{
				m_lpData[i * uBmpLineByte + 3 * j + 2] = lpDataPoint[3 * j + 0];//R
				m_lpData[i * uBmpLineByte + 3 * j + 1] = lpDataPoint[3 * j + 1];//G
				m_lpData[i * uBmpLineByte + 3 * j + 0] = lpDataPoint[3 * j + 2];//B
			}
		}
		lpDataPoint -= uJPGLineByte;
	}
	m_bValid = TRUE;//λͼ��Ч
	
	//�����ʱ����
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	delete[] lpJPGData;
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
//�������ܣ���ȡm_lpBmpFileHeader
//�����������
//����ֵ��LPBITMAPFILEHEADER��ʾm_lpBmpFileHeader
//==============================================
LPBITMAPFILEHEADER CDib::GetBmpFileHeader()
{
	return m_lpBmpFileHeader;
}
//=============================================
//�������ܣ���ȡm_lpBmpFileHeader
//�����������
//����ֵ��������ɫ��� �Ҷ�Ϊ1��rgbΪ3
//==============================================
LONG CDib::GetDepth()
{
	if (IsGrade())
		return 1;
	return 3;
}
//=============================================
//�������ܣ���ȡm_lpDib
//�����������
//����ֵ��LPBYTE��ʾm_lpDib
//==============================================
LPBYTE CDib::GetlpDib()
{
	return m_lpDib;
}
//=============================================
//�������ܣ���ȡdouble��������ָ��
//�����������
//����ֵ��LPBITMAPINFO��ʾm_lpBmpInfo
//==============================================
double* CDib::GetDData()
{
	return m_lpDData;
}
//=============================================
//�������ܣ���ȡλͼ����
//�����������
//����ֵ��LPBITMAPINFO��ʾm_lpBmpInfo
//==============================================
LPBITMAPINFO CDib::GetlpBmpInfo()
{
	return m_lpBmpInfo;
}
//=============================================
//�������ܣ���ȡλͼ����
//�����������
//����ֵ��LPBITMAPINFOHEADER��ʾm_lpBmpInfoHeader
//==============================================
LPBITMAPINFOHEADER CDib::GetlpBmpInfoHeader()
{
	return m_lpBmpInfoHeader;
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
	delete[] lpLogPalette;//�ͷŻ�����
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
	pDc->TextOut(origin.x + 0.3 * size.cx, origin.y + size.cy + 5, m_title);
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
		BYTE r, g, b;
		LPBYTE lpInitBmpData = createGradeBmp(lWidth, lHeight);//�Ҷ�λͼ���ݴ���
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
		delete[] lpInitBmpData;//ɾ��������ԭʼ����ָ��
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
		
		//��ɫλͼ����ģʽ
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
		delete[] lpInitBmpData;//ɾ��������ԭʼ����ָ��
	}
	return TRUE;
}
//=============================================
//�������ܣ�RGBתXYZ
//�����������
//����ֵ��BOOL-TRUE��ʾ�ɹ���FALSE��ʾʧ��
//==============================================
BOOL CDib::RgbToXYZ(const BYTE sR, const BYTE  sG, const BYTE sB, double& X, double& Y, double& Z)
{
	double R, G, B, r, g, b;
	R = sR / 255.0;
	G = sG / 255.0;
	B = sB / 255.0;
	if (R <= 0.04045)	r = R / 12.92;
	else				r = pow((R + 0.055) / 1.055, 2.4);
	if (G <= 0.04045)	g = G / 12.92;
	else				g = pow((G + 0.055) / 1.055, 2.4);
	if (B <= 0.04045)	b = B / 12.92;
	else				b = pow((B + 0.055) / 1.055, 2.4);
	X = r * 0.4124564 + g * 0.3575761 + b * 0.1804375;
	Y = r * 0.2126729 + g * 0.7151522 + b * 0.0721750;
	Z = r * 0.0193339 + g * 0.1191920 + b * 0.9503041;
	return TRUE;
}
//=============================================
//�������ܣ�RGBתLAB��ʽ
//�����������
//����ֵ��BOOL-TRUE��ʾ�ɹ���FALSE��ʾʧ��
//==============================================
BOOL CDib::RgbToLab()
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
		double r, g, b, R, G, B, sR, sG, sB;
		double X, Y, Z;
		double lval, aval, bval;
		if (m_lpDData)
		{
			delete[] m_lpDData;
			m_lpDData = nullptr;
		}
		//LPBYTE lpInitBmpData = createColorBmp(lWidth, lHeight);//������ɫλͼ���ݴ������õ�����ͼ������ֵ
		UINT uGradeBmpLineByte = GetLineByte();
		m_lpDData = new double[3 * lHeight * lWidth];
		memset(m_lpDData, 0, sizeof(double) * 3 * lHeight * lWidth);
		for (int i = 0; i < lHeight; ++i)
		{
			for (int j = 0; j < lWidth; ++j)
			{

				sB = m_lpData[i * uLineByte + 3 * j];
				sG = m_lpData[i * uLineByte + 3 * j + 1];
				sR = m_lpData[i * uLineByte + 3 * j + 2];
				RgbToXYZ(sR, sG, sB, X, Y, Z);
				//------------------------
				// XYZ to LAB conversion
				//------------------------
				double epsilon = 0.008856;	//actual CIE standard
				double kappa = 903.3;		//actual CIE standard

				double Xr = 0.950456;	//reference white
				double Yr = 1.0;		//reference white
				double Zr = 1.088754;	//reference white

				double xr = X / Xr;
				double yr = Y / Yr;
				double zr = Z / Zr;

				double fx, fy, fz;
				if (xr > epsilon)	fx = pow(xr, 1.0 / 3.0);
				else				fx = (kappa * xr + 16.0) / 116.0;
				if (yr > epsilon)	fy = pow(yr, 1.0 / 3.0);
				else				fy = (kappa * yr + 16.0) / 116.0;
				if (zr > epsilon)	fz = pow(zr, 1.0 / 3.0);
				else				fz = (kappa * zr + 16.0) / 116.0;

				lval = 116.0 * fy - 16.0;
				aval = 500.0 * (fx - fy);
				bval = 200.0 * (fy - fz);

				m_lpDData[i * 3 * lWidth + 3 * j] = lval;
				m_lpDData[i * 3 * lWidth + 3 * j + 1] = aval;
				m_lpDData[i * 3 * lWidth + 3 * j + 2] = bval;
			}
		}
		//delete[]lpInitBmpData;//ɾ��������ԭʼ����ָ��
	}
	return TRUE;
}
//=============================================
//�������ܣ�LABתRGB��ʽ
//�����������
//����ֵ��BOOL-TRUE��ʾ�ɹ���FALSE��ʾʧ��
//==============================================
float gamma_XYZ2RGB(double x)
{
	return x > 0.0031308 ? (1.055f * powf(x, (1 / 2.4f)) - 0.055) : (x * 12.92);
};
BOOL CDib::XYZToRgb(BYTE& R, BYTE& G, BYTE& B,
	const double& X, const double& Y, const double& Z)
{
	float RR, GG, BB;
	RR = 3.2404542f * X - 1.5371385f * Y - 0.4985314f * Z;
	GG = -0.9692660f * X + 1.8760108f * Y + 0.0415560f * Z;
	BB = 0.0556434f * X - 0.2040259f * Y + 1.0572252f * Z;

	RR = gamma_XYZ2RGB(RR);
	GG = gamma_XYZ2RGB(GG);
	BB = gamma_XYZ2RGB(BB);

	RR = RR * 255.0 + 0.5;
	if (RR > 255)
		RR = 255;
	else if (RR < 0)
		RR = 0;
	GG = GG * 255.0 + 0.5;
	if (GG > 255)
		GG = 255;
	else if (GG < 0)
		GG = 0;
	BB = BB * 255.0 + 0.5;
	if (BB > 255)
		BB = 255;
	else if (BB < 0)
		BB = 0;

	R = (BYTE)RR;
	G = (BYTE)GG;
	B = (BYTE)BB;
	return TRUE;
}
//=============================================
//�������ܣ�LABתRGB��ʽ
//�����������
//����ֵ��BOOL-TRUE��ʾ�ɹ���FALSE��ʾʧ��
//==============================================
BOOL CDib::LabToRgb()
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

		const double param_16116 = 16.0f / 116.0f;
		LONG lHeight = GetHeight();
		LONG lWidth = GetWidth();
		UINT uLineByte = GetLineByte();
		double r, g, b, sR, sG, sB;
		double X, Y, Z;
		BYTE R, G, B;
		double lval, aval, bval;
		//LPBYTE lpInitBmpData = createColorBmp(lWidth, lHeight);//������ɫλͼ���ݴ������õ�����ͼ������ֵ
		UINT uBmpLineByte = GetLineByte();
		for (int i = 0; i < lHeight; ++i)
		{
			for (int j = 0; j < lWidth; ++j)
			{
				lval = m_lpDData[i * 3 * lWidth + 3 * j];
				aval = m_lpDData[i * 3 * lWidth + 3 * j + 1];
				bval = m_lpDData[i * 3 * lWidth + 3 * j + 2];
				double dX, dY, dZ;

				dY = (lval + 16.0f) / 116.0;
				dX = aval / 500.0f + dY;
				dZ = dY - bval / 200.0f;
				if (powf(dY, 3.0) > 0.008856)
					Y = powf(dY, 3.0);
				else
					Y = (dY - param_16116) / 7.787f;

				if (powf(dX, 3.0) > 0.008856)
					X = dX * dX * dX;
				else
					X = (dX - param_16116) / 7.787f;

				if (powf(dZ, 3.0) > 0.008856)
					Z = dZ * dZ * dZ;
				else
					Z = (dZ - param_16116) / 7.787f;

				XYZToRgb(R, G, B, X, Y, Z);
				m_lpData[i * uBmpLineByte + 3 * j] = B;
				m_lpData[i * uBmpLineByte + 3 * j + 1] = G;
				m_lpData[i * uBmpLineByte + 3 * j + 2] = R;
			}
		}
		if (m_lpDData)
			delete[] m_lpDData;
		m_lpDData = nullptr;
		//delete[] lpInitBmpData;//ɾ��������ԭʼ����ָ��
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
	if (m_lpDData)
	{
		delete[] m_lpDData;
		m_lpDData = nullptr;
	}
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
		m_lpBmpInfo = nullptr;
		m_lpBmpInfoHeader = nullptr;
		m_lpRgbQuad = nullptr;
		m_lpData = nullptr;
		delete[] m_lpDib;
		m_lpDib = nullptr;
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
//�������ܣ�ͼ����ת
//���������float fAngle  ��ת�Ƕ�
//����ֵ��TRUE or FALSE���Ƿ���ת�ɹ���
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
	//����ԭͼ���ĸ�������
	srcX1 = (float)(-(width - 1) / 2);
	srcY1 = (float)((height - 1) / 2);
	srcX2 = (float)((width - 1) / 2);
	srcY2 = (float)((height - 1) / 2);
	srcX3 = (float)(-(width - 1) / 2);
	srcY3 = (float)(-(height - 1) / 2);
	srcX4 = (float)((width - 1) / 2);
	srcY4 = (float)(-(height - 1) / 2);
	//������ͼ���ĸ�������
	dstX1 = fCos * srcX1 + fSin * srcY1;
	dstY1 = -fSin * srcX1 + fCos * srcY1;
	dstX2 = fCos * srcX2 + fSin * srcY2;
	dstY2 = -fSin * srcX2 + fCos * srcY2;
	dstX3 = fCos * srcX3 + fSin * srcY3;
	dstY3 = -fSin * srcX3 + fCos * srcY3;
	dstX4 = fCos * srcX4 + fSin * srcY4;
	dstY4 = -fSin * srcX4 + fCos * srcY4;
	LONG dstWidth = (LONG)max(max(dstX1, dstX2), max(dstX3, dstX4)) - (LONG)min(min(dstX1, dstX2), min(dstX3, dstX4));//������ת��ͼ��Ŀ��
	LONG dstHeight = (LONG)max(max(dstY1, dstY2), max(dstY3, dstY4)) - (LONG)min(min(dstY1, dstY2), min(dstY3, dstY4));//������ת��ͼ��ĸ߶�
	//������λͼ��С
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
		for (i = 0; i < dstHeight; ++i)//��
		{
			for (j = 0; j < dstWidth; ++j)//��
			{
				//ָ����ͼ���i�У���j�����ص�ָ��
				lpDstBmpData = m_lpData + dstLineBytes * (dstHeight - 1 - i) + j;
				//�����������ԴDIB�е�����
				dstI = (long)(-((float)j) * fSin + ((float)i) * fCos + varFloat2 + 0.5);
				dstJ = (long)(((float)j) * fCos + ((float)i) * fSin + varFloat1 + 0.5);
				if ((dstI >= 0) && (dstI < height) && (dstJ >= 0) && (dstJ < width))//�ж��Ƿ�����Դͼ����
				{
					*lpDstBmpData = *(lpSrcBmpData + lLineBytes * (height - 1 - dstI) + dstJ);

				}
				else
				{
					*lpDstBmpData = 255;
				}
			}
		}
		delete[] lpSrcBmpData;//ɾ��������ԭʼ����ָ��
	}
	else
	{
		lpSrcBmpData = createColorBmp(dstWidth, dstHeight);
		long dstLineBytes = GetLineByte();
		for (i = 0; i < dstHeight; ++i)//��
		{
			for (j = 0; j < dstWidth; ++j)//��
			{
				//ָ����ͼ���i�У���j�����ص�ָ��
				lpDstBmpData = m_lpData + dstLineBytes * (dstHeight - 1 - i) + 3 * j;
				//�����������ԴDIB�е�����
				dstI = (long)(-((float)j) * fSin + ((float)i) * fCos + varFloat2 + 0.5);
				dstJ = (long)(((float)j) * fCos + ((float)i) * fSin + varFloat1 + 0.5);
				if ((dstI >= 0) && (dstI < height) && (dstJ >= 0) && (dstJ < width))//�ж��Ƿ�����Դͼ����
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
		delete[] lpSrcBmpData;//ɾ��������ԭʼ����ָ��
	}
}

//=============================================
//�������ܣ��½��հ׻Ҷ�λͼ
//���������λͼ�ļ�ͷָ�룬λͼָ�룬ͼ��߶ȣ�ͼ����
//����ֵ��ԭͼ����������ָ�루�ǵ��ͷ��ڴ�ռ䣩
//==============================================
LPBYTE CDib::createGradeBmp(LONG width, LONG height, bool IsBackup)
{
	//��ԭͼ�����ݱ���
	LPBYTE initData = nullptr;
	if (TRUE == IsBackup)
	{
		LONG lInitSize = GetHeight() * GetLineByte();
		initData = (LPBYTE) new BYTE[lInitSize];
		memcpy(initData, m_lpData, lInitSize);
	}
	
	//��ȡԴλͼ��Ϣ
	LONG lHeight = height;
	LONG lWidth = width;
	//����Ҷ�λͼ��������ռ�
	UINT uGradeBmpLineByte = (lWidth * 8 / 8 + 3) / 4 * 4;//((width * (depth * 8)) + 31) / 32 * 4;
	DWORD dwGradeBmpDataSize = uGradeBmpLineByte * lHeight;
	//����Ҷ�λͼ����ռ�
	DWORD dwGradeBmpSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + dwGradeBmpDataSize;
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
	lpGradeBmpInfoHeader->biXPelsPerMeter = 0;
	lpGradeBmpInfoHeader->biYPelsPerMeter = 0;
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
	Empty(FALSE);//�ͷ�ԭ��λͼ�ռ�
	//�����趨ԭλͼָ��ָ��
	m_lpBmpFileHeader = lpGradeBmpFileHeader;
	m_lpDib = lpGradeBmp;
	m_lpBmpInfo = (LPBITMAPINFO)(lpGradeBmp);
	m_lpBmpInfoHeader = (LPBITMAPINFOHEADER)(lpGradeBmp);
	m_lpRgbQuad = lpGradeBmpRgbQuad;
	m_lpData = lpGradeBmpData;
	m_bHasRgbQuad = TRUE;//������ɫ���־
	MakePalette();//���ɵ�ɫ��
	m_bValid = TRUE;//����λͼ��Ч��־
	return initData;
}
//=============================================
//�������ܣ��½��հײ�ɫλͼ����
//���������ͼ��߶ȣ�ͼ����  IsBackup�Ƿ񱸷�
//����ֵ���������򷵻�nullptr ���򷵻�ԭͼ�����ݱ���ָ�루�ǵ��ͷ��ڴ�ռ䣩
//==============================================
LPBYTE CDib::createColorBmp( LONG width, LONG height, bool IsBackup)
{
	//��ԭͼ�����ݱ���
	LONG lInitSize= GetHeight()* GetLineByte();
	LPBYTE initData = nullptr;
	if (TRUE == IsBackup)
	{
		initData = (LPBYTE) new BYTE[lInitSize];
		memcpy(initData, m_lpData, lInitSize);
	}
	//��ȡԴλͼ��Ϣ
	LONG lHeight = height;
	LONG lWidth = width;
	//�����ɫλͼ��������ռ�
	UINT uColorBmpLineByte = (lWidth * 24 / 8 + 3) / 4 * 4;
	DWORD  dwColorBmpDataSize = uColorBmpLineByte * lHeight;
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
	//Ϊ��ɫλͼ����ռ䣬����ʼ��Ϊ0
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
	Empty(FALSE);//�ͷ�ԭ��λͼ�ռ�
	//�����趨Դλͼָ��ָ��
	m_lpBmpFileHeader = lpColorBmpFileHeader;
	m_lpDib = lpColorBmp;
	m_lpBmpInfo = (LPBITMAPINFO)(lpColorBmp);
	m_lpBmpInfoHeader = lpColorBmpInfoHeader;
	m_lpRgbQuad = nullptr;
	m_lpData = lpColorBmpData;
	m_bHasRgbQuad = FALSE;//������ɫ���־
	m_bValid = TRUE;//����λͼ��Ч��־
	return initData;
}



