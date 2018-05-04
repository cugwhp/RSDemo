
// DisplayImageDoc.cpp : CDisplayImageDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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


// CDisplayImageDoc ����/����

CDisplayImageDoc::CDisplayImageDoc()
{
	// TODO: �ڴ����һ���Թ������
	m_poDataset = NULL;
}

CDisplayImageDoc::~CDisplayImageDoc()
{
}

BOOL CDisplayImageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

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


// CDisplayImageDoc ���л�

void CDisplayImageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CDisplayImageDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
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

// ������������֧��
void CDisplayImageDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
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

// CDisplayImageDoc ���

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


// CDisplayImageDoc ����
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
