#pragma once


// frdList 对话框

class frdList : public CDialogEx
{
	DECLARE_DYNAMIC(frdList)

public:
	frdList(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~frdList();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl FRD_LIST;
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
};
