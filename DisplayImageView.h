
// DisplayImageView.h : CDisplayImageView 类的接口
//

#pragma once


class CDisplayImageView : public CView
{
protected: // 仅从序列化创建
	CDisplayImageView();
	DECLARE_DYNCREATE(CDisplayImageView)

// 特性
public:
	CDisplayImageDoc* GetDocument() const;

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

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnInitialUpdate();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DisplayImageView.cpp 中的调试版本
inline CDisplayImageDoc* CDisplayImageView::GetDocument() const
   { return reinterpret_cast<CDisplayImageDoc*>(m_pDocument); }
#endif

