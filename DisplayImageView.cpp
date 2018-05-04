
// DisplayImageView.cpp : CDisplayImageView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DisplayImage.h"
#endif

#include "DisplayImageDoc.h"
#include "DisplayImageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDisplayImageView

IMPLEMENT_DYNCREATE(CDisplayImageView, CView)

BEGIN_MESSAGE_MAP(CDisplayImageView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDisplayImageView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_MESSAGE_VOID(WM_INITIALUPDATE, CDisplayImageView::OnInitialUpdate)
END_MESSAGE_MAP()

// CDisplayImageView 构造/析构

CDisplayImageView::CDisplayImageView()
{
	// TODO: 在此处添加构造代码
	m_pbyRGB = NULL;
	m_pRBuff = NULL;
	m_pGBuff = NULL;
	m_pBBuff = NULL;
	m_nBuffWidth = NULL;
	m_nBuffHeight = NULL;
	m_nRGBWidth = 0;
	m_nRGBHeight = 0;
	m_dRatio = 1.0f;
	m_nRBandIdx = m_nGBandIdx = m_nBBandIdx = 0;
	m_hBitmap = NULL;
	memset(&m_bitmap, 0, sizeof(m_bitmap));
}

CDisplayImageView::~CDisplayImageView()
{
	if (m_pbyRGB) { delete m_pbyRGB; m_pbyRGB = NULL; }
	if (m_pRBuff) { CPLFree(m_pRBuff);m_pRBuff = NULL; }
	if (m_pGBuff) { CPLFree(m_pGBuff);	m_pGBuff = NULL; }
	if (m_pBBuff) { CPLFree(m_pGBuff);	m_pGBuff = NULL; }

	if (m_hBitmap) { DeleteObject(m_hBitmap); m_hBitmap = NULL; }
}

BOOL CDisplayImageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDisplayImageView 绘制

void CDisplayImageView::OnDraw(CDC* pDC)
{
	CDisplayImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	DrawImgInfo(pDC);

	DrawImage(pDC);
}

void CDisplayImageView::DrawImage(CDC* pDC)
{
	if (m_pbyRGB == NULL)	return;

	//	CDC MemDC;
	//	MemDC.CreateCompatibleDC(pDC);

	//Preparing Bitmap Info
	BITMAPINFO bmInfo;
	memset(&bmInfo.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth = m_bitmap.bmWidth;
	bmInfo.bmiHeader.biHeight = m_bitmap.bmHeight;
	bmInfo.bmiHeader.biPlanes = 1;
	bmInfo.bmiHeader.biBitCount = 24;


	SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, m_bitmap.bmWidth, m_bitmap.bmHeight, 0, 0, 0, m_bitmap.bmHeight, m_pbyRGB, &bmInfo, DIB_RGB_COLORS);
}


bool CDisplayImageView::InitBuffer(double dRatio)
{
	if (m_pbyRGB) { delete m_pbyRGB; m_pbyRGB = NULL; }
	if (m_pRBuff) { CPLFree(m_pRBuff);m_pRBuff = NULL; }
	if (m_pGBuff) { CPLFree(m_pGBuff);	m_pGBuff = NULL; }
	if (m_pBBuff) { CPLFree(m_pGBuff);	m_pGBuff = NULL; }

	CDisplayImageDoc*	pDoc = GetDocument();
	if (pDoc == NULL)	return false;

	GDALDataset*	poDataset = pDoc->GetDataset();
	if (poDataset == NULL)	return false;

	int		nRasterXSize = poDataset->GetRasterXSize();
	int		nRasterYSize = poDataset->GetRasterYSize();
	int		nBandCount = poDataset->GetRasterCount();
	RECT	rectView;

	if (m_nRBandIdx<1 || m_nGBandIdx<1 || m_nBBandIdx<1)
	{
		m_nRBandIdx = 1;
		m_nGBandIdx = nBandCount<3 ? 1 : 2;
		m_nBBandIdx = nBandCount<3 ? 1 : 3;
	}

	GetClientRect(&rectView);
	if (dRatio < 1e-6)
	{
		m_dRatio = min(1., (rectView.right - rectView.left) / (nRasterXSize * 1.));
		m_dRatio = min(m_dRatio, (rectView.bottom - rectView.top) / (nRasterYSize * 1.));
	}
	else
	{
		m_dRatio = dRatio;
	}

	m_nBuffWidth = (int)ceil(nRasterXSize * m_dRatio);
	m_nBuffHeight = (int)ceil(nRasterYSize * m_dRatio);

	m_pRBuff = CPLMalloc(GDALGetDataTypeSize(GDT_Float64)*m_nBuffHeight*m_nBuffWidth);
	m_pGBuff = CPLMalloc(GDALGetDataTypeSize(GDT_Float64)*m_nBuffHeight*m_nBuffWidth);
	m_pBBuff = CPLMalloc(GDALGetDataTypeSize(GDT_Float64)*m_nBuffHeight*m_nBuffWidth);

	m_nRGBWidth = (m_nBuffWidth * 24L + 31L) / 32L * 4;
	m_nRGBHeight = m_nBuffHeight;

	m_pbyRGB = new BYTE[m_nRGBWidth*m_nRGBHeight];
	memset(m_pbyRGB, 0, m_nRGBWidth*m_nRGBHeight);

	if (!m_pRBuff || !m_pGBuff || !m_pBBuff || !m_pbyRGB)
		return false;

	m_bitmap.bmBits = (LPVOID)m_pbyRGB;
	m_bitmap.bmBitsPixel = 24;
	m_bitmap.bmHeight = m_nBuffHeight;
	m_bitmap.bmPlanes = 0;
	m_bitmap.bmType = 0;
	m_bitmap.bmWidth = m_nBuffWidth;
	m_bitmap.bmWidthBytes = m_nRGBWidth;

	return true;
}

void CDisplayImageView::ReadRaster()
{
	CDisplayImageDoc*	pDoc = GetDocument();
	if (pDoc == NULL)	return;

	GDALDataset*	poDataset = pDoc->GetDataset();
	if (poDataset == NULL)	return;

	int		nRasterXSize = poDataset->GetRasterXSize();
	int		nRasterYSize = poDataset->GetRasterYSize();
	int		nBandCount = poDataset->GetRasterCount();
	GDALRasterBand*	poBand = NULL;

	poBand = poDataset->GetRasterBand(m_nRBandIdx);
	poBand->RasterIO(GF_Read, 0, 0, nRasterXSize, nRasterYSize, m_pRBuff, m_nBuffWidth, m_nBuffHeight, GDT_Float64, 0, 0);

	if (m_nGBandIdx == m_nRBandIdx)
		memcpy(m_pGBuff, m_pRBuff, m_nBuffHeight*m_nBuffWidth*GDALGetDataTypeSize(GDT_Float64));
	else
	{
		poBand = poDataset->GetRasterBand(m_nGBandIdx);
		poBand->RasterIO(GF_Read, 0, 0, nRasterXSize, nRasterYSize, m_pGBuff, m_nBuffWidth, m_nBuffHeight, GDT_Float64, 0, 0);
	}

	if (m_nBBandIdx == m_nRBandIdx)
		memcpy(m_pBBuff, m_pRBuff, m_nBuffHeight*m_nBuffWidth*GDALGetDataTypeSize(GDT_Float64));
	else
	{
		poBand = poDataset->GetRasterBand(m_nBBandIdx);
		poBand->RasterIO(GF_Read, 0, 0, nRasterXSize, nRasterYSize, m_pBBuff, m_nBuffWidth, m_nBuffHeight, GDT_Float64, 0, 0);
	}

	double*	ptrR = (double*)m_pRBuff;
	double*	ptrB = (double*)m_pBBuff;
	double*	ptrG = (double*)m_pGBuff;
	for (int i = 0; i<m_nBuffHeight; ++i)
	{
		for (int j = 0; j<m_nBuffWidth; ++j)
		{
			m_pbyRGB[i*m_nRGBWidth + 3 * j + 0] = ptrB[(m_nBuffHeight - i - 1)*m_nBuffWidth + j];
			m_pbyRGB[i*m_nRGBWidth + 3 * j + 1] = ptrG[(m_nBuffHeight - i - 1)*m_nBuffWidth + j];
			m_pbyRGB[i*m_nRGBWidth + 3 * j + 2] = ptrR[(m_nBuffHeight - i - 1)*m_nBuffWidth + j];
		}
	}

	if (m_hBitmap) { DeleteObject(m_hBitmap); m_hBitmap = NULL; }
	m_hBitmap = CreateBitmapIndirect(&m_bitmap);
}

void CDisplayImageView::DrawImgInfo(CDC* pDC)
{
	USES_CONVERSION;

	CDisplayImageDoc*	pDoc = GetDocument();
	GDALDataset* poDataset = pDoc->GetDataset();
	if (poDataset == NULL)
		return;

	pDC->TextOut(10, 10, A2W(poDataset->GetDescription()));
}

// CDisplayImageView 打印

void CDisplayImageView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDisplayImageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDisplayImageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDisplayImageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CDisplayImageView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

//void CDisplayImageView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
//{
//#ifndef SHARED_HANDLERS
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
//#endif
//}


// CDisplayImageView 诊断

#ifdef _DEBUG
void CDisplayImageView::AssertValid() const
{
	CView::AssertValid();
}

void CDisplayImageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDisplayImageDoc* CDisplayImageView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDisplayImageDoc)));
	return (CDisplayImageDoc*)m_pDocument;
}
#endif //_DEBUG


// CDisplayImageView 消息处理程序
void CDisplayImageView::OnInitialUpdate()
{
	if (InitBuffer())
		ReadRaster();

	CView::OnInitialUpdate();
}