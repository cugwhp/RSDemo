
// DisplayImageView.h : CDisplayImageView ��Ľӿ�
//

#pragma once


class CDisplayImageView : public CView
{
protected: // �������л�����
	CDisplayImageView();
	DECLARE_DYNCREATE(CDisplayImageView)

// ����
public:
	CDisplayImageDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CDisplayImageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void	DrawImage(CDC* pDC);
	void	DrawImgInfo(CDC* pDC);
	void	ReadRaster();
	bool	InitBuffer(double dRatio = 0.);

protected:
	BYTE*	m_pbyRGB;
	void*	m_pRBuff;
	void*	m_pGBuff;
	void*	m_pBBuff;
	int		m_nBuffWidth;
	int		m_nBuffHeight;
	int		m_nRGBWidth;
	int		m_nRGBHeight;
	double	m_dRatio;
	int		m_nRBandIdx;
	int		m_nGBandIdx;
	int		m_nBBandIdx;
	BITMAP	m_bitmap;
	HBITMAP	m_hBitmap;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnInitialUpdate();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DisplayImageView.cpp �еĵ��԰汾
inline CDisplayImageDoc* CDisplayImageView::GetDocument() const
   { return reinterpret_cast<CDisplayImageDoc*>(m_pDocument); }
#endif

