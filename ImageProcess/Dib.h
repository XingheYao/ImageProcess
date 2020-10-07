#pragma once
//=======================================================
//���ݣ��豸�޹�λͼ
//���ܣ�
//      λͼ�ļ����뱣��
//		λͼ����Ϣ��ȡ
//		λͼ�����ݻ�ȡ
//		λͼ����ʾ
//      λͼ��ת��
//		λͼ������ж�
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
	CDib(void);											//���캯������ʼ�����ݳ�Ա
	CDib(CDib& rhs);                                    //���캯������ʼ�����ݳ�Ա
	~CDib(void);										//�����������ͷ��ڴ�ռ�	
	BOOL LoadFromFile(CString lpszPath);				//���ļ�����λͼ
	BOOL SaveToFile(CString lpszPath);					//��λͼ���浽�ļ�
	CString GetFileName();								//��ȡλͼ�ļ���
	LONG GetWidth();									//��ȡλͼ���
	LONG GetHeight();									//��ȡλͼ�߶�
	CSize GetDimension();								//��ȡλͼ�Ŀ�Ⱥ͸߶�
	DWORD GetSize();									//��ȡλͼ�Ĵ�С
	WORD GetBitCount();									//��ȡ����������ռλ��
	UINT GetLineByte();									//��ȡÿ��������ռλ��
	DWORD GetNumOfColor();								//��ȡλͼ��ɫ��
	LPRGBQUAD GetRgbQuad();								//��ȡλͼ��ɫ��
	LPBYTE GetData();									//��ȡλͼ����
	BOOL Draw(CDC* pDC, CPoint origin, CSize size);		//��ʾλͼ
	BOOL RgbToGrade();									//24λ��ɫλͼת8λ�Ҷ�λͼ
	BOOL GradeToRgb();									//8λ�Ҷ�λͼת24λ��ɫλͼ
	BOOL HasRgbQuad();									//�ж��Ƿ�����ɫ��
	BOOL IsGrade();										//�ж��Ƿ��ǻҶ�ͼ
	BOOL IsValid();										//�ж�λͼ�Ƿ���Ч
	CDib& operator=(const CDib& rhs);                   //��=�����������
	void Empty(BOOL bFlag = TRUE);						//����ռ�
protected:
	DWORD CalcRgbQuadLength();							//����λͼ��ɫ����
	BOOL MakePalette();									//������ɫ�����ɵ�ɫ��
	
public:
	CString m_title;                                    //�ļ���ʾ����
private:
	
	CString m_fileName;							        //λͼ�ļ���
	LPBITMAPFILEHEADER m_lpBmpFileHeader;				//λͼ�ļ�ͷָ�룬�趯̬������ͷ�
	LPBYTE m_lpDib;										//λͼָ�룬������λͼ�ļ�ͷ����������
														//�趯̬������ͷ�
	LPBITMAPINFO m_lpBmpInfo;							//λͼ��Ϣָ��
	LPBITMAPINFOHEADER m_lpBmpInfoHeader;				//λͼ��Ϣͷָ��
	LPRGBQUAD m_lpRgbQuad;								//λͼ��ɫ��ָ��
	LPBYTE m_lpData;									//λͼ����ָ��
	HPALETTE m_hPalette;								//��ɫ����
	BOOL m_bHasRgbQuad;									//�Ƿ�����ɫ��
	BOOL m_bValid;										//λͼ�Ƿ���Ч
};