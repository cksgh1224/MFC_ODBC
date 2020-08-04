#pragma once

#ifndef _MYODBC_H_
#define _MYODBC_H_


// ODBC API�� ����ϱ� ���� ��� ���� �߰�
#include <odbcinst.h>
#include <sqlext.h>

// ODBC API�� ����ϱ� ���� ���̺귯�� �߰�
#pragma comment(lib, "odbc32.lib")


// ���̺귯��
//#ifdef _DEBUG
//	#pragma comment(lib, "DSH_TWAPI_ODBC.lib")
//#else
//	#pragma comment(lib, "RST_TWAPI_ODBC.lib")
//#endif



// typedef �Լ� ������ : ��ȯ��(*�Լ��̸�)(�Ű�����)
// SET_RECORD_INFO    : Query�� ����� �ش��ϴ� �����͸� ���� �޸�(����� ���� ��ü)�� �����ϱ� ���� �ڵ� ����
// SQL_RESULT_RECORD  : SQL ��ɹ��� ���ؼ� ������ �����͵��� �ڽ��� ���ϴ� ���·� ��ȯ�ϰų� ����,ó���ϴ� �۾�
// ������ ȣ��Ǵ� �Լ��� ������ �۾��� �ϴ�(MyOdbc�� ����ϴ�) cpp ���Ͽ� �����ؼ� ���
typedef void(*SET_RECORD_INFO)(void* ap_owner, HSTMT ah_statement, void* ap_data);
typedef int(*SQL_RESULT_RECORD)(void* ap_owner, int a_step_index, void* ap_data, ULONG a_count, unsigned short* ap_state, int option);



class MyOdbc
{
private:
	SQLHANDLE mh_environment;  // ODBC ����� ����ϱ� ���� ȯ�� ����
	SQLHDBC mh_odbc;           // ODBC �Լ��� ����ϱ� ���� ����
	int m_connect_flag;        // ������ ���� ���θ� ������ ���� (1:����)
	void* mp_owner;            // ������ �ڵ��� ������ ����

public:
	MyOdbc();
	virtual ~MyOdbc();

	inline char IsConnected() { return m_connect_flag; }
	inline SQLHANDLE GetEnvHandle() { return mh_environment; }
	inline SQLHDBC GetOdbcHandle() { return mh_odbc; }


	// ������� �ʱ�ȭ
	void InitObject();

	// ���� ���� (����: 1��ȯ)
	int Connect(const wchar_t* ap_dsn, const wchar_t* ap_id, const wchar_t* ap_pw, void* ap_owner = 0);

	// ���� ���� ����
	void Disconnect();

	// ��ɹ� ���� (insert, update, delete) (������� Ư���ϰ� ó������ �ʴ´�) (����: 1��ȯ)
	int ExecQuery(const wchar_t* ap_query);

	// ��ɹ� ���� (select) (�˻��� ������ ó��) (����: 1��ȯ)
	// a_record_size : sizeof(������), a_record_count_per_step : �����͸� �������� �ִ� ����
	// set_record_info, sql_result_record : typedef �Լ� ������
	// option: 0-��ü ������ �˻�, 1-�α���, 2-���̵� ã��, 3-��й�ȣ ã��, 4-���̵� �ߺ�Ȯ��
	int ExecQuery(const wchar_t* ap_query, int a_record_size, SET_RECORD_INFO set_record_info, SQL_RESULT_RECORD sql_result_record, 
		int option, int a_record_count_per_step = 100);
	
};



#endif