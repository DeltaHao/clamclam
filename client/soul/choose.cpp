// choose.cpp: 实现文件
//

#include "intConnect.h"
#include "pch.h"
#include "clamclam.h"
#include "choose.h"
#include "afxdialogex.h"
#include "chat.h"
#include "frdList.h"


// choose 对话框

IMPLEMENT_DYNAMIC(choose, CDialogEx)

int choosePattern(int stutas);

choose::choose(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

choose::~choose()
{
}

void choose::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(choose, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &choose::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &choose::OnBnClickedButton1)
END_MESSAGE_MAP()


// choose 消息处理程序

void choose::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	choosePattern(1);
	chat dlg;
	dlg.DoModal();
}

void choose::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	choosePattern(2);
	frdList dlg;
	dlg.DoModal();
}