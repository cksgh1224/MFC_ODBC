#pragma once


// CreateAccountDlg 대화 상자

class CreateAccountDlg : public CDialogEx
{
private:
	MyOdbc my_odbc; 
	bool id_check;  // 사용 가능한 아이디인지 체크할 변수 (아이디 중복확인)

	DECLARE_DYNAMIC(CreateAccountDlg)

public:
	CreateAccountDlg(MyOdbc odbc, CWnd* pParent = nullptr); // 객체 생성시 MyOdbc 객체를 인자로 받아 my_odbc에 대입한다
	virtual ~CreateAccountDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATEACCOUNT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCheckidbtn();
	virtual BOOL OnInitDialog();
};
