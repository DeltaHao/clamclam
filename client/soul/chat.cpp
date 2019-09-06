// chat.cpp: 实现文件
//

#include "intConnect.h"
#include "pch.h"
#include "clamclam.h"
#include "chat.h"
#include "afxdialogex.h"
#include <iostream>
#include <afxpriv.h>

using namespace std;
// chat 对话框

CString msg;
CString m_cstrSendRecord = _T("");
int sendMessage(char* buff);
string receiveMessage();
int addFriend(char* name);

UINT chat::ThreadFunc(LPVOID pParam)
{
	chat* dlg = (chat*)pParam;
	dlg->process();
	return 0;
}


IMPLEMENT_DYNAMIC(chat, CDialogEx)

BOOL chat::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CWinThread* pThread = (CWinThread*)AfxBeginThread(ThreadFunc, this);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


chat::chat(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

chat::~chat()
{
}

void chat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(chat, CDialogEx)
	ON_BN_CLICKED(IDOK, &chat::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON1, &chat::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &chat::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &chat::OnEnChangeEditSend)
	ON_EN_CHANGE(IDC_EDIT3, &chat::OnEnChangeEdit3)
END_MESSAGE_MAP()

// chat 消息处理程序


void chat::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_cstrSendMsg = _T("");
	((CEdit*)GetDlgItem(IDC_EDIT1))->GetWindowTextW(m_cstrSendMsg);
	((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowTextW(TEXT(""));
	USES_CONVERSION;
	char* message = T2A(m_cstrSendMsg);
	sendMessage(message);
}

void chat::process() {
	string message;
	while (1) {
		message = receiveMessage();
		if (message[0] != 0) {
			msg = message.c_str();
			m_cstrSendRecord += (msg + TEXT("\r\n\r\n"));
			((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowTextW(m_cstrSendRecord);
			((CEdit*)GetDlgItem(IDC_EDIT2))->LineScroll(((CEdit*)GetDlgItem(IDC_EDIT2))->GetLineCount());
		}
	}
}

void chat::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_cstrSendMsg = _T("");
	((CEdit*)GetDlgItem(IDC_EDIT3))->GetWindowTextW(m_cstrSendMsg);
	((CEdit*)GetDlgItem(IDC_EDIT3))->SetWindowTextW(TEXT(""));
	USES_CONVERSION;
	char* friendName = T2A(m_cstrSendMsg);
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


void chat::OnEnChangeEditSend()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

}

void chat::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void chat::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
