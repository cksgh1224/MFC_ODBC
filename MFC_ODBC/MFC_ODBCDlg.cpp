
// MFC_ODBCDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFC_ODBC.h"
#include "MFC_ODBCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCODBCDlg 대화 상자



CMFCODBCDlg::CMFCODBCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_ODBC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCODBCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCODBCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Login_btn, &CMFCODBCDlg::OnBnClickedLoginbtn)
	ON_BN_CLICKED(IDC_CreateAccount_btn, &CMFCODBCDlg::OnBnClickedCreateaccountbtn)
	ON_BN_CLICKED(IDC_FindID_btn, &CMFCODBCDlg::OnBnClickedFindidbtn)
	ON_BN_CLICKED(IDC_FindPW_btn, &CMFCODBCDlg::OnBnClickedFindpwbtn)
	ON_BN_CLICKED(IDC_AllSelect_btn, &CMFCODBCDlg::OnBnClickedAllselectbtn)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMFCODBCDlg 메시지 처리기

BOOL CMFCODBCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// mysql 서버 연결
	if (my_odbc.Connect(L"socket_user", L"root", L"0000", this))
	{
		//MessageBox(L"서버 접송 성공", L"MySql", MB_OK);
	}
	else
	{
		MessageBox(L"서버 접송 실패", L"MySql", MB_OK);
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCODBCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCODBCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// 다이얼로그가 종료될때 서버접속 해제
void CMFCODBCDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	my_odbc.Disconnect(); // 서버접속 해제
}




// 로그인 버튼 클릭
void CMFCODBCDlg::OnBnClickedLoginbtn()
{
	CString id, pw, query;
	GetDlgItemText(IDC_ID_EDIT, id); // IDC_ID_EDIT에 입력된 텍스트를 id에 저장
	GetDlgItemText(IDC_PW_EDIT, pw);
	
	if(id == "" || pw == "")
	{
		MessageBox(L"아이디, 비밀번호를 입력해 주세요", NULL, MB_OK);
		return;
	}

	query.Format(L" select mid, mpw, mname from user where mid='%s' and mpw='%s' ", id, pw); // CString::Format -> CString 문자열에 형식 지정자 사용
	
	if (my_odbc.ExecQuery(query, sizeof(User), SetRecordInfo, ResultRecord, 1)) // query 실행
	{
		// 로그인 성공
		MessageBox(L"로그인 성공", NULL, MB_OK);
	}
	else
	{
		MessageBox(L"입력하신 정보가 일치하지 않습니다", NULL, MB_OK);
	}

}


// 아이디 찾기 버튼 클릭
void CMFCODBCDlg::OnBnClickedFindidbtn()
{
	// 새로운 다이얼로그 띄우기
	FindIdDlg findIdDlg(my_odbc);
	findIdDlg.DoModal();
}


// 비밀번호 찾기 버튼 클릭
void CMFCODBCDlg::OnBnClickedFindpwbtn()
{
	FindPwDlg findPwDlg(my_odbc);
	findPwDlg.DoModal();
}


// 전체 사용자 버튼 클릭
void CMFCODBCDlg::OnBnClickedAllselectbtn()
{
	CString query = L" select mid, mpw, mname from user ";
	my_odbc.ExecQuery(query, sizeof(User), SetRecordInfo, ResultRecord, 0);
}



// 회원가입 버튼 클릭
void CMFCODBCDlg::OnBnClickedCreateaccountbtn()
{
	CreateAccountDlg createAccountDlg(my_odbc);
	createAccountDlg.DoModal();
}