// oneChat.cpp: 实现文件
//

#include "intConnect.h"
#include "pch.h"
#include "clamclam.h"
#include "oneChat.h"
#include "afxdialogex.h"
#include <iostream>
#include <afxpriv.h>


using namespace std;

CString one_msg;
CString m_one_cstrSendRecord = _T("");
int sendMessage(char* buff);
string receiveMessage();
int addFriend(char* name);

// oneChat 对话框

UINT oneChat::ThreadFunc(LPVOID pParam)
{
	oneChat* dlg = (oneChat*)pParam;
	dlg->process();
	return 0;
}

IMPLEMENT_DYNAMIC(oneChat, CDialogEx)

oneChat::oneChat(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG6, pParent)
{

}

oneChat::~oneChat()
{
}

void oneChat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(oneChat, CDialogEx)
	ON_BN_CLICKED(IDOK, &oneChat::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &oneChat::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &oneChat::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &oneChat::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT4, &oneChat::OnEnChangeEdit4)
END_MESSAGE_MAP()


// oneChat 消息处理程序

BOOL oneChat::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CWinThread* pThread = (CWinThread*)AfxBeginThread(ThreadFunc, this);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void oneChat::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_one_cstrSendMsg = _T("");
	((CEdit*)GetDlgItem(IDC_EDIT1))->GetWindowTextW(m_one_cstrSendMsg);
	((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowTextW(TEXT(""));//清空
	USES_CONVERSION;
	char* message = T2A(m_one_cstrSendMsg);
	sendMessage(message);
}

void oneChat::process() {
	string one_message;
	while (1) {
		one_message = receiveMessage();
		//CString test;                                    //测试是否收到消息
		//test = one_message.c_str();
		//MessageBox(test);
		if (one_message[0] != 0) {
			one_msg = one_message.c_str();
			m_one_cstrSendRecord += (one_msg + TEXT("\r\n\r\n"));
			((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowTextW(m_one_cstrSendRecord);
			((CEdit*)GetDlgItem(IDC_EDIT2))->LineScroll(((CEdit*)GetDlgItem(IDC_EDIT2))->GetLineCount());
		}
	}
}

void oneChat::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_one_cstrSendMsg = _T("");
	((CEdit*)GetDlgItem(IDC_EDIT4))->GetWindowTextW(m_one_cstrSendMsg);
	((CEdit*)GetDlgItem(IDC_EDIT4))->SetWindowTextW(TEXT(""));
	USES_CONVERSION;
	char* friendName = T2A(m_one_cstrSendMsg);
	int getFriend = addFriend(friendName);
	if (getFriend == 0) {
		CString tip = _T("添加好友成功");
		MessageBox(tip);
	}
	else {
		CString tip = _T("添加好友失败");
		MessageBox(tip);
	}
}

void oneChat::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void oneChat::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void oneChat::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
