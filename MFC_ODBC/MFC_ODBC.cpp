
// MFC_ODBC.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "MFC_ODBC.h"
#include "MFC_ODBCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCODBCApp

BEGIN_MESSAGE_MAP(CMFCODBCApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMFCODBCApp 생성

CMFCODBCApp::CMFCODBCApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CMFCODBCApp 개체입니다.

CMFCODBCApp theApp;


// CMFCODBCApp 초기화

BOOL CMFCODBCApp::InitInstance()
{
	CWinApp::InitInstance();

	CMFCODBCDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	
	return FALSE;
}

