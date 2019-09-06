#pragma once


// chat 对话框

class chat : public CDialogEx
{
	DECLARE_DYNAMIC(chat)

public:
	chat(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~chat();
	static UINT ThreadFunc(LPVOID pParam);
	void  process();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnEnChangeEditSend();
	afx_msg void OnEnChangeEdit1();
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit3();
};
