#pragma once


// FindPwDlg 대화 상자

class FindPwDlg : public CDialogEx
{
private:
	MyOdbc my_odbc;

	DECLARE_DYNAMIC(FindPwDlg)

public:
	FindPwDlg(MyOdbc odbc, CWnd* pParent = nullptr); // 객체 생성시 MyOdbc 객체를 인자로 받아 my_odbc에 대입한다
	virtual ~FindPwDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINDPW_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFindpwbtn();
};
