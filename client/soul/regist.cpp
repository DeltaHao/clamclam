// regist.cpp: 实现文件
//


#include "pch.h"
#include "clamclam.h"
#include "regist.h"
#include "afxdialogex.h"
#include "choose.h"
#include "intConnect.h"
#include <afxpriv.h>


// regist 对话框

IMPLEMENT_DYNAMIC(regist, CDialogEx)

int Register(char* _name, char* _password);

regist::regist(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

regist::~regist()
{
}

void regist::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(regist, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &regist::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &regist::OnBnClickedOk)
	ON_BN_CLICKED(IDOK_REG, &regist::OnBnClickedReg)
END_MESSAGE_MAP()


// regist 消息处理程序


void regist::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void regist::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
}

void regist::OnBnClickedReg()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_rAccount("");
	CString m_rPassword("");
	((CEdit*)GetDlgItem(IDC_REG_ACCOUNT))->GetWindowTextW(m_rAccount);
	((CEdit*)GetDlgItem(IDC_REG_PASS))->GetWindowTextW(m_rPassword);
	USES_CONVERSION;
	char* rAccount = T2A(m_rAccount);
	char* rPassword = T2A(m_rPassword);

	int l = Register(rAccount, rPassword);
	if (l == -1) {
		CString logTest("用户名已存在");
		MessageBox(logTest);
	}
	else if (l == 0) {
		CString logTest("注册成功");
		MessageBox(logTest);
		choose dlg;
		dlg.DoModal();
	}
}
