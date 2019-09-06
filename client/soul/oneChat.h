#pragma once


// oneChat 对话框

class oneChat : public CDialogEx
{
	DECLARE_DYNAMIC(oneChat)

public:
	oneChat(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~oneChat();
	static UINT ThreadFunc(LPVOID pParam);
	void  process();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit1();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit4();
};
