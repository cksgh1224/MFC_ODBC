#pragma once


// CreateAccountDlg 대화 상자

class CreateAccountDlg : public CDialogEx
{
private:
	MyOdbc my_odbc;

	DECLARE_DYNAMIC(CreateAccountDlg)

public:
	CreateAccountDlg(MyOdbc odbc, CWnd* pParent = nullptr);   // 표준 생성자입니다.
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
};
