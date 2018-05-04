// DlgImgInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "DisplayImage.h"
#include "DlgImgInfo.h"
#include "afxdialogex.h"


// CDlgImgInfo 对话框

IMPLEMENT_DYNAMIC(CDlgImgInfo, CDialogEx)

CDlgImgInfo::CDlgImgInfo(CWnd* pParent /*=NULL*/, GDALDataset* poDataset)
	: CDialogEx(IDD_DLG_IMG_INFO, pParent), m_poDataset(poDataset)
	, m_strImgInfoTxt(_T(""))
{

}

CDlgImgInfo::~CDlgImgInfo()
{
}

void CDlgImgInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IMG_INFO, m_lstImgInfoCtrl);
	DDX_Text(pDX, IDC_IMG_INFO_TXT, m_strImgInfoTxt);
}


BEGIN_MESSAGE_MAP(CDlgImgInfo, CDialogEx)
END_MESSAGE_MAP()


// CDlgImgInfo 消息处理程序
BOOL CDlgImgInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitImgInfo();

	InitImgListCtrl();

	return TRUE;
}

void CDlgImgInfo::InitImgInfo()
{
	if (m_poDataset == NULL)
		return;

	USES_CONVERSION;

	UpdateData(TRUE);
	m_strImgInfoTxt = _T("");

	CString		strTxt = _T("");
	GDALRasterBand*	poBand = NULL;
	double			dMin, dMax, dMean, dStdDev;
	for (int i = 0; i<m_poDataset->GetRasterCount(); ++i)
	{
		GDALRasterBand*	poBand = m_poDataset->GetRasterBand(i + 1);
		if (poBand == NULL)	continue;

		poBand->GetStatistics(0, 1, &dMin, &dMax, &dMean, &dStdDev);

		strTxt.Format(_T("波段 %d 统计\r\n最小值:\t\t%.6f\r\n最大值:\t\t%.6f\r\n平均值:\t\t%.6f\r\n方差:\t\t%.6f\r\n"), i + 1, dMin, dMax, dMean, dStdDev);
		m_strImgInfoTxt += strTxt;
	}

	UpdateData(FALSE);
}

void CDlgImgInfo::InitImgListCtrl()
{
	USES_CONVERSION;

	DWORD	dwStyle;
	dwStyle = m_lstImgInfoCtrl.GetExtendedStyle();
	dwStyle |= (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstImgInfoCtrl.SetExtendedStyle(dwStyle);

	m_lstImgInfoCtrl.InsertColumn(0, _T("项目"), 0, 80);
	m_lstImgInfoCtrl.InsertColumn(1, _T("值"), 0, 400);


	if (m_poDataset == NULL)	return;


	CString		strTxt;
	m_lstImgInfoCtrl.InsertItem(0, _T("文件地址"));
	m_lstImgInfoCtrl.SetItemText(0, 1, A2W(m_poDataset->GetDescription()));

	m_lstImgInfoCtrl.InsertItem(1, _T("波段数"));
	strTxt.Format(_T("%d"), m_poDataset->GetRasterCount());
	m_lstImgInfoCtrl.SetItemText(1, 1, strTxt);

	m_lstImgInfoCtrl.InsertItem(2, _T("宽度"));
	strTxt.Format(_T("%d"), m_poDataset->GetRasterXSize());
	m_lstImgInfoCtrl.SetItemText(2, 1, strTxt);

	m_lstImgInfoCtrl.InsertItem(3, _T("高度"));
	strTxt.Format(_T("%d"), m_poDataset->GetRasterYSize());
	m_lstImgInfoCtrl.SetItemText(3, 1, strTxt);

	m_lstImgInfoCtrl.InsertItem(4, _T("投影参考"));
	strTxt.Format(_T("%s"), A2W(m_poDataset->GetProjectionRef()));
	m_lstImgInfoCtrl.SetItemText(4, 1, strTxt);

	double	dszGeoTrans[6];
	m_poDataset->GetGeoTransform(dszGeoTrans);
	m_lstImgInfoCtrl.InsertItem(5, _T("Geo X0"));
	strTxt.Format(_T("%.6f"), dszGeoTrans[0]);
	m_lstImgInfoCtrl.SetItemText(5, 1, strTxt);

	m_lstImgInfoCtrl.InsertItem(6, _T("Geo Y0"));
	strTxt.Format(_T("%.6f"), dszGeoTrans[3]);
	m_lstImgInfoCtrl.SetItemText(6, 1, strTxt);

	m_lstImgInfoCtrl.InsertItem(7, _T("GSD X"));
	strTxt.Format(_T("%.6f"), dszGeoTrans[1]);
	m_lstImgInfoCtrl.SetItemText(7, 1, strTxt);

	m_lstImgInfoCtrl.InsertItem(8, _T("GSD Y"));
	strTxt.Format(_T("%.6f"), fabs(dszGeoTrans[5]));
	m_lstImgInfoCtrl.SetItemText(8, 1, strTxt);
}