#pragma once


// regist 对话框

class regist : public CDialogEx
{
	DECLARE_DYNAMIC(regist)

public:
	regist(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~regist();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedReg();
};
