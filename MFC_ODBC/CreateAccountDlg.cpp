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
	id_check = false;
}

CreateAccountDlg::~CreateAccountDlg()
{
}

void CreateAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CreateAccountDlg, CDialogEx)
	ON_BN_CLICKED(IDC_Create_btn, &CreateAccountDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CheckId_btn, &CreateAccountDlg::OnBnClickedCheckidbtn)
END_MESSAGE_MAP()


// CreateAccountDlg 메시지 처리기


BOOL CreateAccountDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 버튼 비활성화
	//GetDlgItem(IDC_Create_btn)->EnableWindow(FALSE);

	return TRUE;
}



// 회원가입
void CreateAccountDlg::OnBnClickedButton1()
{
	//MessageBox(L"회원가입", NULL, MB_OK);


	if (!id_check) // 중복확인을 하지 않았으면
	{
		MessageBox(L"아이디 중복확인을 해주세요", NULL, MB_OK);
		return;
	}


	CString id, pw, name, nickname, ip, query;
	GetDlgItemText(IDC_ID_EDIT, id);
	GetDlgItemText(IDC_PW_EDIT, pw);
	GetDlgItemText(IDC_NAME_EDIT, name);
	GetDlgItemText(IDC_NICKNAME_EDIT, nickname);
	GetDlgItemText(IDC_IPADDRESS_EDIT, ip);

	if (id == "" || pw == "" || name == "" || nickname == "", ip == "")
	{
		MessageBox(L"모두 입력해주세요", NULL, MB_OK);
		return;
	}


	query.Format(L" insert into user (mid,mpw,mname,mip,mnickname) values ('%s','%s','%s','%s','%s') ", id, pw, name, ip, nickname);
	//MessageBox(query, NULL, MB_OK);


	if (my_odbc.ExecQuery(query))
	{
		MessageBox(L"회원가입 성공", NULL, MB_OK);
		::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL); // 대화상자 종료
	}
	else
	{
		MessageBox(L"회원가입 실패", NULL, MB_OK);
	}

}


// 아이디 중복확인
void CreateAccountDlg::OnBnClickedCheckidbtn()
{
	CString id, query;
	GetDlgItemText(IDC_ID_EDIT, id);

	if (id == "")
	{
		MessageBox(L"아이디를 입력해 주세요", NULL, MB_OK);
		return;
	}

	// 중복확인 이후 아이디 다시입력
	if (id_check)
	{
		id_check = false;
		GetDlgItem(IDC_ID_EDIT)->EnableWindow(TRUE); // 버튼 활성화
		GetDlgItem(IDC_ID_EDIT)->SetWindowTextW(L""); // 빈문자열로 초기화
		GetDlgItem(IDC_CheckId_btn)->SetWindowTextW(L"중복확인"); // 대화상자 caption 변경
		return;
	}

	query.Format(L" select mid from user where mid='%s' ", id);
	//MessageBox(query, NULL, MB_OK);

	if (!my_odbc.ExecQuery(query, sizeof(User), SetRecordInfo, ResultRecord, 4))
	{
		id += L" 사용 가능한 아이디 입니다";
		MessageBox(id, NULL, MB_OK);
		id_check = true;

		GetDlgItem(IDC_ID_EDIT)->EnableWindow(FALSE); // 버튼 비활성화

		GetDlgItem(IDC_CheckId_btn)->SetWindowTextW(L"다시입력"); // 대화상자 caption 변경
	}
	else
	{
		id_check = false;
	}

}



