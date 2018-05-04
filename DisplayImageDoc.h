
// DisplayImageDoc.h : CDisplayImageDoc 类的接口
//


#pragma once
#include "gdal_priv.h"
#include "cpl_conv.h"

class CDisplayImageDoc : public CDocument
{
protected: // 仅从序列化创建
	CDisplayImageDoc();
	DECLARE_DYNCREATE(CDisplayImageDoc)

// 特性
public:
	GDALDataset*	GetDataset() { return m_poDataset; }

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CDisplayImageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	GDALDataset*	m_poDataset;	//Dataset

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnViewImageInfo();


	virtual void DeleteContents();

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnUpdateViewImageInfo(CCmdUI *pCmdUI);
};
