
// DisplayImageDoc.cpp : CDisplayImageDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DisplayImage.h"
#endif

#include "DisplayImageDoc.h"
#include "DlgImgInfo.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDisplayImageDoc

IMPLEMENT_DYNCREATE(CDisplayImageDoc, CDocument)

BEGIN_MESSAGE_MAP(CDisplayImageDoc, CDocument)
	ON_COMMAND(ID_IMAGE_INFO, OnViewImageInfo)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_INFO, &CDisplayImageDoc::OnUpdateViewImageInfo)
END_MESSAGE_MAP()


// CDisplayImageDoc 构造/析构

CDisplayImageDoc::CDisplayImageDoc()
{
	// TODO: 在此添加一次性构造代码
	m_poDataset = NULL;
}

CDisplayImageDoc::~CDisplayImageDoc()
{
}

BOOL CDisplayImageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}

BOOL CDisplayImageDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	//
	USES_CONVERSION;
	m_poDataset = (GDALDataset*)GDALOpen(W2A(lpszPathName), GA_ReadOnly);
	if (m_poDataset == NULL)
	{
		AfxMessageBox(_T("Open Image File Failed!!"));

		return FALSE;
	}

	return TRUE;
}


// CDisplayImageDoc 序列化

void CDisplayImageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CDisplayImageDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CDisplayImageDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CDisplayImageDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDisplayImageDoc 诊断

#ifdef _DEBUG
void CDisplayImageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDisplayImageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDisplayImageDoc 命令
void CDisplayImageDoc::DeleteContents()
{
	CDocument::DeleteContents();
	if (m_poDataset)
	{
		delete m_poDataset;
		m_poDataset = NULL;
	}
}

void CDisplayImageDoc::OnViewImageInfo()
{
	CDlgImgInfo		dlg(NULL, m_poDataset);
	dlg.DoModal();
}

void CDisplayImageDoc::OnUpdateViewImageInfo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_poDataset != NULL);
}
