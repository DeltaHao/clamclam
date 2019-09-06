// login.cpp: 实现文件
//

#include "intConnect.h"
#include "pch.h"
#include "clamclam.h"
#include "login.h"
#include "afxdialogex.h"
#include "choose.h"
#include <afxpriv.h>


// login 对话框

IMPLEMENT_DYNAMIC(login, CDialogEx)

int Login(char* _name, char* _password);

login::login(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

login::~login()
{
}

void login::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(login, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &login::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &login::OnBnClickedOk)
	ON_BN_CLICKED(IDOK_logIN, &login::OnBnClickedlogin)
	ON_EN_CHANGE(IDC_LOGIN_PASS, &login::OnEnChangeLoginPass)
	ON_EN_CHANGE(IDC_LOGIN_ACCOUNT, &login::OnEnChangeLoginAccount)
END_MESSAGE_MAP()


// login 消息处理程序


void login::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	CDialogEx::OnOK();
}


void login::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	choose Dlg;
	Dlg.DoModal();
	CDialogEx::OnOK();
}


void login::OnBnClickedlogin()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_account("");
	CString m_password("");
	((CEdit*)GetDlgItem(IDC_LOGIN_ACCOUNT))->GetWindowTextW(m_account);
	((CEdit*)GetDlgItem(IDC_LOGIN_PASS))->GetWindowTextW(m_password);
	USES_CONVERSION;
	char* account = T2A(m_account);
	char* password = T2A(m_password);
	
	int l = Login(account, password);
	if (l == -1) {
		CString logTest("用户名或密码错误");
		MessageBox(logTest);
	}
	else if (l == 0) {
		CString logTest("登录成功");
		MessageBox(logTest);
		choose dlg;
		dlg.DoModal();
	}
}


void login::OnEnChangeLoginPass()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void login::OnEnChangeLoginAccount()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}