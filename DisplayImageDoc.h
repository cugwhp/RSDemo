
// DisplayImageDoc.h : CDisplayImageDoc ��Ľӿ�
//


#pragma once
#include "gdal_priv.h"
#include "cpl_conv.h"

class CDisplayImageDoc : public CDocument
{
protected: // �������л�����
	CDisplayImageDoc();
	DECLARE_DYNCREATE(CDisplayImageDoc)

// ����
public:
	GDALDataset*	GetDataset() { return m_poDataset; }

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CDisplayImageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	GDALDataset*	m_poDataset;	//Dataset

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnViewImageInfo();


	virtual void DeleteContents();

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnUpdateViewImageInfo(CCmdUI *pCmdUI);
};
