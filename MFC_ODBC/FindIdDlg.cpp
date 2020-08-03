// FindIdDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFC_ODBC.h"
#include "FindIdDlg.h"
#include "afxdialogex.h"


// FindIdDlg 대화 상자

IMPLEMENT_DYNAMIC(FindIdDlg, CDialogEx)

FindIdDlg::FindIdDlg(MyOdbc odbc, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FINDID_DIALOG, pParent)
{
	my_odbc = odbc;
}

FindIdDlg::~FindIdDlg()
{
}

void FindIdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(FindIdDlg, CDialogEx)
	ON_BN_CLICKED(IDC_FindID_btn, &FindIdDlg::OnBnClickedFindidbtn)
END_MESSAGE_MAP()


// FindIdDlg 메시지 처리기



void FindIdDlg::OnBnClickedFindidbtn()
{
	CString name, nickname, query;
	GetDlgItemText(IDC_NAME_EDIT, name);
	GetDlgItemText(IDC_NICKNAME_EDIT, nickname);

	query.Format(L" select mid, mpw, mname from user where mname='%s' and mnickname='%s' ", name, nickname);

	if (my_odbc.ExecQuery(query, sizeof(User), SetRecordInfo, ResultRecord, 2))
	{
		
	}
	else
	{
		MessageBox(L"입력하신 정보가 일치하지 않습니다", L"실패", MB_OK);
	}
}
