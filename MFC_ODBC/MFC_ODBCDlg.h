﻿
// MFC_ODBCDlg.h: 헤더 파일
//

#pragma once


#include "FindIdDlg.h"          // 아이디 찾기
#include "FindPwDlg.h"          // 비밀번호 찾기
#include "CreateAccountDlg.h"   // 회원가입


// CMFCODBCDlg 대화 상자
class CMFCODBCDlg : public CDialogEx
{
private:
	MyOdbc my_odbc; // ODBC를 사용하기 위한 객체
	
	
// 생성입니다.
public:
	CMFCODBCDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_ODBC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoginbtn();
	afx_msg void OnBnClickedCreateaccountbtn();
	afx_msg void OnBnClickedFindidbtn();
	afx_msg void OnBnClickedFindpwbtn();
	afx_msg void OnBnClickedAllselectbtn();
	afx_msg void OnDestroy();
};
