#pragma once


// choose 对话框

class choose : public CDialogEx
{
	DECLARE_DYNAMIC(choose)

public:
	choose(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~choose();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
};
