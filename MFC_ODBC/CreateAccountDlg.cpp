// CreateAccountDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFC_ODBC.h"
#include "CreateAccountDlg.h"
#include "afxdialogex.h"


// CreateAccountDlg 대화 상자

IMPLEMENT_DYNAMIC(CreateAccountDlg, CDialogEx)

CreateAccountDlg::CreateAccountDlg(MyOdbc odbc, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CREATEACCOUNT_DIALOG, pParent)
{
	my_odbc = odbc;
}

CreateAccountDlg::~CreateAccountDlg()
{
}

void CreateAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CreateAccountDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CreateAccountDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CheckId_btn, &CreateAccountDlg::OnBnClickedCheckidbtn)
END_MESSAGE_MAP()


// CreateAccountDlg 메시지 처리기



// 회원가입
void CreateAccountDlg::OnBnClickedButton1()
{
	//MessageBox(L"회원가입", NULL, MB_OK);






}


// 아이디 중복확인
void CreateAccountDlg::OnBnClickedCheckidbtn()
{
	MessageBox(L"아이디 중복확인", NULL, MB_OK);
}
