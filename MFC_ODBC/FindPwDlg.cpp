// FindPwDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFC_ODBC.h"
#include "FindPwDlg.h"
#include "afxdialogex.h"


// FindPwDlg 대화 상자

IMPLEMENT_DYNAMIC(FindPwDlg, CDialogEx)

FindPwDlg::FindPwDlg(MyOdbc odbc, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FINDPW_DIALOG, pParent)
{
	my_odbc = odbc; // 객체 생성시 MyOdbc 객체를 인자로 받아 my_odbc에 대입한다
}

FindPwDlg::~FindPwDlg()
{
}

void FindPwDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(FindPwDlg, CDialogEx)
	ON_BN_CLICKED(IDC_FindPW_btn, &FindPwDlg::OnBnClickedFindpwbtn)
END_MESSAGE_MAP()


// FindPwDlg 메시지 처리기


void FindPwDlg::OnBnClickedFindpwbtn()
{
	CString id, ip, query;
	GetDlgItemText(IDC_ID_EDIT, id);
	GetDlgItemText(IDC_IP_EDIT, ip);

	if (id == "" || ip == "")
	{
		MessageBox(L"아이디, IP주소를 입력해 주세요", NULL, MB_OK);
		return;
	}

	query.Format(L" select mid, mpw, mname from user where mid='%s' and mip='%s' ", id, ip);
	
	if (my_odbc.ExecQuery(query, sizeof(User), SetRecordInfo, ResultRecord, 3))
	{

	}
	else
	{
		MessageBox(L"입력하신 정보가 일치하지 않습니다", L"실패", MB_OK);
	}
}
