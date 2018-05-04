#pragma once
#include "gdal_priv.h"
#include "afxcmn.h"

// CDlgImgInfo 对话框

class CDlgImgInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImgInfo)

public:
	CDlgImgInfo(CWnd* pParent = NULL, GDALDataset* poDataset = NULL);   // 标准构造函数
	virtual ~CDlgImgInfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_IMG_INFO };
#endif


private:
	GDALDataset*	m_poDataset;

protected:
	void InitImgInfo();
	void InitImgListCtrl();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lstImgInfoCtrl;
	CString m_strImgInfoTxt;
};
