// frdList.cpp: 实现文件
//

#include "intConnect.h"
#include "pch.h"
#include "clamclam.h"
#include "frdList.h"
#include "afxdialogex.h"
#include "oneChat.h"
#include<iostream>
//#include<string.h>

using namespace std;
int chooseFriend(int Cur);
// frdList 对话框
int m_Row = -1, m_Col = -1;
IMPLEMENT_DYNAMIC(frdList, CDialogEx)

frdList::frdList(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{

}

frdList::~frdList()
{
}

void frdList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, FRD_LIST);
}


BEGIN_MESSAGE_MAP(frdList, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &frdList::OnLvnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &frdList::OnDblclkList1)
END_MESSAGE_MAP()


// frdList 消息处理程序
string receiveFriendList();
BOOL frdList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	FRD_LIST.InsertColumn(0, _T("好友名"), LVCFMT_LEFT, 200);
	string flag;
	flag = receiveFriendList();
	char* omg;
	omg = (char*)flag.data();
	char* delim = ";";
	//char* p;
	char* m_friend;
	m_friend = strtok(omg, delim);
		
	int i = 1;
	while (m_friend) {
		CString s;
		s = m_friend;
		LPCTSTR   m = (LPCTSTR)s;
		FRD_LIST.InsertItem(i, s);
		m_friend = strtok(NULL, delim);
		i++;
		if (m_friend == NULL) break;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void frdList::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CRect rc;
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	int m_nCurrentSel = pList->GetSelectedCount();
	if (m_nCurrentSel > 0) {
		int Cur = FRD_LIST.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
		
		chooseFriend(Cur);
		oneChat dlg;
		dlg.DoModal();
	}
	else
		MessageBox(_T("为空白处，没有点中某一行！"));
	
	//m_Row = pNMListView->iItem;//获得选中的行  
	//m_Col = pNMListView->iSubItem;//获得选中列 
	
	
		//m_strDiskId = dlg.m_strDiskId;

		
	*pResult = 0;
}

void frdList::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	

	*pResult = 0;
}