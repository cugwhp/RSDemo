#pragma once
#include "gdal_priv.h"
#include "afxcmn.h"

// CDlgImgInfo �Ի���

class CDlgImgInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImgInfo)

public:
	CDlgImgInfo(CWnd* pParent = NULL, GDALDataset* poDataset = NULL);   // ��׼���캯��
	virtual ~CDlgImgInfo();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_IMG_INFO };
#endif


private:
	GDALDataset*	m_poDataset;

protected:
	void InitImgInfo();
	void InitImgListCtrl();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lstImgInfoCtrl;
	CString m_strImgInfoTxt;
};
